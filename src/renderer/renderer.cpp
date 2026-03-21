#include "renderer.h"

#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "scene/sprite.h"

Renderer2D::Renderer2D() :
  m_Vao(0),
  m_Vbo(0),
  m_Ibo(0),
  m_Shader(nullptr),
  m_View(1.0f),
  m_Projection(1.0f) {
  const unsigned int indices[] = {0, 1, 2, 2, 3, 0};

  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glGenBuffers(1, &m_Vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * 4, nullptr, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &m_Ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                        reinterpret_cast<const void*>(offsetof(QuadVertex, position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                        reinterpret_cast<const void*>(offsetof(QuadVertex, texCoord)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                        reinterpret_cast<const void*>(offsetof(QuadVertex, colour)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_Shader = new Shader("assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
}

Renderer2D::~Renderer2D() {
  delete m_Shader;
  m_Shader = nullptr;

  if (m_Ibo != 0) {
    glDeleteBuffers(1, &m_Ibo);
    m_Ibo = 0;
  }

  if (m_Vbo != 0) {
    glDeleteBuffers(1, &m_Vbo);
    m_Vbo = 0;
  }

  if (m_Vao != 0) {
    glDeleteVertexArrays(1, &m_Vao);
    m_Vao = 0;
  }
}

void Renderer2D::BeginScene(const Camera& camera) {
  m_View = camera.GetView();
  m_Projection = camera.GetProjection();
}

void Renderer2D::DrawSprite(const Sprite& sprite, const Texture& texture) {
  const float x = sprite.transform.x;
  const float y = sprite.transform.y;
  const float width = sprite.transform.width;
  const float height = sprite.transform.height;
  const float rotation = sprite.transform.rotation;

  const glm::vec2 center = {x + width * 0.5f, y + height * 0.5f};

  glm::mat2 rotationMatrix = glm::mat2(std::cos(glm::radians(rotation)), -std::sin(glm::radians(rotation)),
                                       std::sin(glm::radians(rotation)), std::cos(glm::radians(rotation)));

  glm::vec2 local0 = {-width * 0.5f, height * 0.5f};
  glm::vec2 local1 = {width * 0.5f, height * 0.5f};
  glm::vec2 local2 = {width * 0.5f, -height * 0.5f};
  glm::vec2 local3 = {-width * 0.5f, -height * 0.5f};

  glm::vec2 world0 = center + rotationMatrix * local0;
  glm::vec2 world1 = center + rotationMatrix * local1;
  glm::vec2 world2 = center + rotationMatrix * local2;
  glm::vec2 world3 = center + rotationMatrix * local3;

  QuadVertex vertices[4] = {
      {{world0.x, world0.y, 0.0f}, {sprite.uvMin.x, sprite.uvMax.y}, sprite.colour},
      {{world1.x, world1.y, 0.0f}, {sprite.uvMax.x, sprite.uvMax.y}, sprite.colour},
      {{world2.x, world2.y, 0.0f}, {sprite.uvMax.x, sprite.uvMin.y}, sprite.colour},
      {{world3.x, world3.y, 0.0f}, {sprite.uvMin.x, sprite.uvMin.y}, sprite.colour},
  };

  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

  m_Shader->Bind();

  m_Shader->SetMat4("u_View", m_View);
  m_Shader->SetMat4("u_Projection", m_Projection);
  m_Shader->SetInt("u_Texture", 0);

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(m_Vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  m_Stats.IncrementDrawCalls();
  m_Stats.IncrementQuadCount();
}

void Renderer2D::EndScene() {}

const RendererStats& Renderer2D::GetStats() const {
  return m_Stats;
}

void Renderer2D::ResetStats() {
  m_Stats.Reset();
}