#pragma once

#include "pch.h"
#include "renderer_stats.h"

class Camera;
class Shader;
class Texture;
struct Sprite;

class Renderer2D {
public:
  Renderer2D();
  ~Renderer2D();

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

private:
  unsigned int m_Vao;
  unsigned int m_Vbo;
  unsigned int m_Ibo;

  std::unique_ptr<Shader> m_Shader;

  glm::mat4 m_View;
  glm::mat4 m_Projection;

  RendererStats m_Stats;
};