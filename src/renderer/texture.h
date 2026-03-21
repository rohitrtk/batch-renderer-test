#pragma once

#include <string>

class Texture {
public:
  Texture(const std::string& filePath);
  ~Texture();

  void Bind() const;
  void Unbind() const;

  unsigned int GetID() const;
  const int GetWidth() const;
  const int GetHeight() const;
  const int GetChannels() const;

private:
  unsigned int m_ID;
  int m_Width;
  int m_Height;
  int m_Channels;
};