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

    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureStorage2D(m_renderer_id, 1, GL_RGB8, m_width, m_height); // TODO: mipmaps, blending

    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // TODO: expose to api
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // TODO: expose to api

    glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  Texture2D::~Texture2D() { glDeleteTextures(1, &m_renderer_id); }

  void Texture2D::bind(uint32_t slot) { glBindTextureUnit(slot, m_renderer_id); }
} // namespace sym_base