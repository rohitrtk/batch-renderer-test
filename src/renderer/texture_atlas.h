#pragma once

#include "pch.h"

struct TextureRegion {
  glm::vec2 uvMin{0.0f, 0.0f};
  glm::vec2 uvMax{1.0f, 1.0f};
};

class GridTextureAtlas {
public:
  GridTextureAtlas(float atlasWidth, float atlasHeight, float cellWidth, float cellHeight, float spriteWidth,
                   float spriteHeight, float paddingLeft = 0.0f, float paddingBottom = 0.0f);

  TextureRegion GetRegion(int gridX, int gridY) const;

private:
  float m_AtlasWidth;
  float m_AtlasHeight;
  float m_CellWidth;
  float m_CellHeight;
  float m_SpriteWidth;
  float m_SpriteHeight;
  float m_PaddingLeft;
  float m_PaddingBottom;
};
