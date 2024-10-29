#ifndef SYM_BASE_TEXTURE_HH
#define SYM_BASE_TEXTURE_HH

namespace sym_base
{
  class Texture
  {
   public:
    virtual uint32_t get_width() const  = 0;
    virtual uint32_t get_height() const = 0;
    virtual uint32_t get_id() const     = 0;

    virtual void bind(uint32_t slot = 0) = 0;
  };

  class Texture2D : public Texture
  {
   public:
    Texture2D(uint32_t width, uint32_t height);
    Texture2D(const std::string& file_path);
    ~Texture2D();

    uint32_t get_width() const override { return m_width; }
    uint32_t get_height() const override { return m_height; }
    uint32_t get_id() const override { return m_renderer_id; }

    void bind(uint32_t slot) override;

   private:
    uint32_t m_renderer_id;
    std::string m_path;
    uint32_t m_width;
    uint32_t m_height;
  };
} // namespace sym_base

#endif // SYM_BASE_TEXTURE_HH
