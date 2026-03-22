#pragma once

#include "pch.h"
#include "renderer_stats.h"

class Camera;
class Shader;
class Texture;
struct Sprite;

class BatchRenderer2D {
public:
  BatchRenderer2D();
  ~BatchRenderer2D();

  void BeginScene(const Camera& camera);
  void EndScene();

  void DrawSprite(const Sprite& sprite, const Texture& texture);

  const RendererStats& GetStats() const;
  void ResetStats();

private:
  struct QuadVertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec4 colour;
  };

  static constexpr unsigned int k_MaxQuads = 1000;
  static constexpr unsigned int k_MaxVertices = k_MaxQuads * 4;
  static constexpr unsigned int k_MaxIndices = k_MaxQuads * 6;

private:
  void BeginBatch();
  void Flush();
  void EndBatch();

private:
  unsigned int m_Vao;
  unsigned int m_Vbo;
  unsigned int m_Ibo;

  std::unique_ptr<Shader> m_Shader;

  glm::mat4 m_View;
  glm::mat4 m_Projection;

  QuadVertex* m_VertexBufferBase;
  QuadVertex* m_VertexBufferPtr;

  unsigned int m_QuadCount;
  const Texture* m_ActiveTexture;

  RendererStats m_Stats;
};