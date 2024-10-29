#include "Texture.hh"
#include "Utils.hh"
#include "pch.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace sym_base
{
  Texture2D::Texture2D(const std::string& file_path) : m_path{ ASSETS_DIR + file_path }
  {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(m_path.c_str(), &width, &height, &channels, 0);
    ASSERT(data, "Failed to load image");
    m_width  = width;
    m_height = height;

    GLenum internal_format = 0, data_format = 0;
    if (channels == 4)
    {
      internal_format = GL_RGBA8;
      data_format     = GL_RGBA;
    }
    else if (channels == 3)
    {
      internal_format = GL_RGB8;
      data_format     = GL_RGB;
    }
    ASSERT(internal_format & data_format, "Format not supported");

    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);

    int mip_levels = 1 + std::floor(std::log2(std::max(width, height)));
    glTextureStorage2D(m_renderer_id, mip_levels, internal_format, m_width, m_height);

    glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenerateTextureMipmap(m_renderer_id);

    stbi_image_free(data);
  }

  Texture2D::~Texture2D() { glDeleteTextures(1, &m_renderer_id); }

  void Texture2D::bind(uint32_t slot) { glBindTextureUnit(slot, m_renderer_id); }
} // namespace sym_base