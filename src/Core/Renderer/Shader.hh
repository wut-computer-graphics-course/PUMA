#ifndef SYM_BASE_SHADER_HH
#define SYM_BASE_SHADER_HH

namespace sym_base
{
  class Shader
  {
   public:
    // Shader(const std::string& file_path);
    Shader(const std::string& vertex_src, const std::string& fragment_src);
    ~Shader();

    void bind() const;
    void unbind() const;

    void upload_uniform_int(const std::string& name, int value);
    void upload_uniform_int_array(const std::string& name, int* values, uint32_t count);
    void upload_uniform_float(const std::string& name, float value);
    void upload_uniform_float2(const std::string& name, const glm::vec2& value);
    void upload_uniform_float3(const std::string& name, const glm::vec3& value);
    void upload_uniform_float4(const std::string& name, const glm::vec4& value);
    void upload_uniform_mat3(const std::string& name, const glm::mat3& matrix);
    void upload_uniform_mat4(const std::string& name, const glm::mat4& matrix);

   private:
    uint32_t m_renderer_id;
  };
} // namespace sym_base

#endif // SYM_BASE_SHADER_HH
