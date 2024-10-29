#include "Shader.hh"
#include "Utils.hh"
#include "pch.hh"
#include <fstream>

static GLenum shader_type_from_string(const std::string& type);

namespace sym_base
{
  Shader::Shader(const std::string& file_path)
  {
    std::string source  = read_file(file_path);
    auto shader_sources = preprocess(source);
    compile(shader_sources);
  }

  Shader::Shader(const std::string& vertex_src, const std::string& fragment_src)
  {
    std::unordered_map<GLenum, std::string> shader_sources;
    shader_sources[GL_VERTEX_SHADER]   = vertex_src;
    shader_sources[GL_FRAGMENT_SHADER] = fragment_src;
    compile(shader_sources);
  }

  Shader::~Shader() { glDeleteProgram(m_renderer_id); }

  void Shader::bind() const { glUseProgram(m_renderer_id); }

  void Shader::unbind() const { glUseProgram(0); }

  void Shader::upload_uniform_int(const std::string& name, int value)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1i(location, value);
  }

  void Shader::upload_uniform_int_array(const std::string& name, int* values, uint32_t count)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1iv(location, count, values);
  }

  void Shader::upload_uniform_float(const std::string& name, float value)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform1f(location, value);
  }

  void Shader::upload_uniform_float2(const std::string& name, const glm::vec2& value)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform2f(location, value.x, value.y);
  }

  void Shader::upload_uniform_float3(const std::string& name, const glm::vec3& value)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
  }

  void Shader::upload_uniform_float4(const std::string& name, const glm::vec4& value)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
  }

  void Shader::upload_uniform_mat3(const std::string& name, const glm::mat3& matrix)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void Shader::upload_uniform_mat4(const std::string& name, const glm::mat4& matrix)
  {
    GLint location = glGetUniformLocation(m_renderer_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  std::string Shader::read_file(const std::string& file_path)
  {
    std::string result;
    std::ifstream in;
    in.open(ASSETS_DIR + file_path, std::ios::in | std::ios::binary);
    if (in)
    {
      in.seekg(0, std::ios::end);
      result.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&result[0], result.size());
      in.close();
    }
    else { throw std::runtime_error("Could not open file '" + file_path + "'"); }

    return result;
  }

  std::unordered_map<GLenum, std::string> Shader::preprocess(const std::string& source)
  {
    std::unordered_map<GLenum, std::string> shader_sources;

    const char* type_token   = "#define type";
    size_t type_token_length = strlen(type_token);
    size_t pos               = source.find(type_token, 0);
    while (pos != std::string::npos)
    {
      size_t eol = source.find_first_of("\r\n", pos);
      ASSERT(eol != std::string::npos, "Unable to preprocess shader source, missing end of line");
      size_t begin     = pos + type_token_length + 1;
      std::string type = source.substr(begin, eol - begin);
      ASSERT(shader_type_from_string(type), "Unable to preprocess shader source, unsupported shader type ");

      size_t next_line_pos = source.find_first_of("\r\n", eol);
      pos                  = source.find(type_token, next_line_pos);
      shader_sources[shader_type_from_string(type)] =
          source.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? source.size() - 1 : next_line_pos));
    }

    return shader_sources;
  }

  void Shader::compile(const std::unordered_map<GLenum, std::string>& shader_sources)
  {
    GLuint program = glCreateProgram();
    std::vector<GLenum> gl_shader_ids;
    gl_shader_ids.reserve(shader_sources.size());

    for (auto& kv : shader_sources)
    {
      GLenum type               = kv.first;
      const std::string& source = kv.second;

      // Create an empty shader handle
      GLuint shader = glCreateShader(type);

      // Send the shader source code to GL
      const GLchar* source_cstr = source.c_str();
      glShaderSource(shader, 1, &source_cstr, 0);

      // Compile the shader
      glCompileShader(shader);

      GLint is_compiled = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
      if (is_compiled == GL_FALSE)
      {
        GLint max_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        // The maxLength includes the NULL character
        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);

        // We don't need the shader anymore.
        glDeleteShader(shader);

        throw std::runtime_error("Shader compilation failure");
        break;
      }

      glAttachShader(program, shader);
      gl_shader_ids.push_back(shader);
    }

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint is_linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
    if (is_linked == GL_FALSE)
    {
      GLint max_length = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

      // The maxLength includes the NULL character
      std::vector<GLchar> info_log(max_length);
      glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

      // We don't need the program anymore.
      glDeleteProgram(program);
      // Don't leak shaders either.
      for (auto& id : gl_shader_ids)
      {
        glDeleteShader(id);
      }

      // Use the infoLog as you see fit.

      // In this simple program, we'll just leave
      return;
    }

    // Always detach shaders after a successful link.
    for (auto& id : gl_shader_ids)
    {
      glDetachShader(program, id);
    }

    m_renderer_id = program;
  }
} // namespace sym_base

static GLenum shader_type_from_string(const std::string& type)
{
  if (type == "vertex") { return GL_VERTEX_SHADER; }
  if (type == "fragment") { return GL_FRAGMENT_SHADER; }

  return 0;
}