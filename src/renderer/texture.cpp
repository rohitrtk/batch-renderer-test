#include "texture.h"
#include "pch.h"
#include <stb_image.h>

Texture::Texture(const std::string& filePath) :
  m_ID(0),
  m_Width(0),
  m_Height(0),
  m_Channels(0) {
  stbi_set_flip_vertically_on_load(true);
  const char* filePathSrc = filePath.c_str();
  unsigned char* data = stbi_load(filePathSrc, &m_Width, &m_Height, &m_Channels, 0);
  if (!data) {
    std::printf("Failed to read texture from file: %s\n", filePathSrc);
    return;
  }

  glGenTextures(1, &m_ID);
  glBindTexture(GL_TEXTURE_2D, m_ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned int format = GL_RGB;
  if (m_Channels == 1)
    format = GL_RED;
  else if (m_Channels == 3)
    format = GL_RGB;
  else if (m_Channels == 4)
    format = GL_RGBA;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

Texture::~Texture() {
  if (m_ID != 0) {
    glDeleteTextures(1, &m_ID);
    m_ID = 0;
  }
}

void Texture::Bind() const {
  glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetID() const {
  return m_ID;
}

const int Texture::GetWidth() const {
  return m_Width;
}

const int Texture::GetHeight() const {
  return m_Height;
}

const int Texture::GetChannels() const {
  return m_Channels;
}