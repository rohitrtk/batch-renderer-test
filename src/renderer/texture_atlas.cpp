#include "texture_atlas.h"

GridTextureAtlas::GridTextureAtlas(float atlasWidth, float atlasHeight, float cellWidth, float cellHeight,
                                   float spriteWidth, float spriteHeight, float paddingLeft, float paddingBottom) :
  m_AtlasWidth(atlasWidth),
  m_AtlasHeight(atlasHeight),
  m_CellWidth(cellWidth),
  m_CellHeight(cellHeight),
  m_SpriteWidth(spriteWidth),
  m_SpriteHeight(spriteHeight),
  m_PaddingLeft(paddingLeft),
  m_PaddingBottom(paddingBottom) {}

TextureRegion GridTextureAtlas::GetRegion(int gridX, int gridY) const {
  const float pixelX = (gridX * m_CellWidth) + m_PaddingLeft;
  const float pixelY = (gridY * m_CellHeight) + m_PaddingBottom;

  TextureRegion region;
  region.uvMin = {pixelX / m_AtlasWidth, pixelY / m_AtlasHeight};
  region.uvMax = {(pixelX + m_SpriteWidth) / m_AtlasWidth, (pixelY + m_SpriteHeight) / m_AtlasHeight};

  return region;
}