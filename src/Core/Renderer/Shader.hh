#ifndef SYM_BASE_SHADER_HH
#define SYM_BASE_SHADER_HH

namespace sym_base
{
  class Shader
  {
   public:
    Shader(const std::string& vertex_src, const std::string& fragment_src);
    ~Shader();

    void bind() const;
    void unbind() const;

   private:
    uint32_t m_renderer_id;
  };
} // namespace sym_base

#endif // SYM_BASE_SHADER_HH
