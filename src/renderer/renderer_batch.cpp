#include "renderer_batch.h"

#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "scene/sprite.h"
#include "math/transform.h"

BatchRenderer2D::BatchRenderer2D() :
  m_Vao(0),
  m_Vbo(0),
  m_Ibo(0),
  m_Shader(std::make_unique<Shader>("assets/shaders/sprite.vert", "assets/shaders/sprite.frag")),
  m_View(1.0f),
  m_Projection(1.0f),
  m_VertexBufferBase(nullptr),
  m_VertexBufferPtr(nullptr),
  m_QuadCount(0),
  m_ActiveTexture(nullptr),
  m_Stats{} {

  std::vector<unsigned int> indices(k_MaxIndices);
  unsigned int offset = 0;
  for (unsigned int i = 0; i < k_MaxIndices; i += 6) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;
    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;

    offset += 4;
  }

  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glGenBuffers(1, &m_Vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferData(GL_ARRAY_BUFFER, k_MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

  glGenBuffers(1, &m_Ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texCoord));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, colour));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_VertexBufferBase = new QuadVertex[k_MaxVertices];
}

BatchRenderer2D::~BatchRenderer2D() {
  delete[] m_VertexBufferBase;
  m_VertexBufferBase = nullptr;
  m_VertexBufferPtr = nullptr;

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

void BatchRenderer2D::BeginScene(const Camera& camera) {
  m_View = camera.GetView();
  m_Projection = camera.GetProjection();

  BeginBatch();
}

void BatchRenderer2D::BeginBatch() {
  m_QuadCount = 0;
  m_VertexBufferPtr = m_VertexBufferBase;
  m_ActiveTexture = nullptr;
}

void BatchRenderer2D::EndBatch() {
  const GLsizeiptr dataSize =
      reinterpret_cast<std::byte*>(m_VertexBufferPtr) - reinterpret_cast<std::byte*>(m_VertexBufferBase);

  glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, m_VertexBufferBase);
}

void BatchRenderer2D::Flush() {
  if (m_QuadCount == 0 || m_ActiveTexture == nullptr) {
    return;
  }

  m_Shader->Bind();
  m_Shader->SetMat4("u_View", m_View);
  m_Shader->SetMat4("u_Projection", m_Projection);
  m_Shader->SetInt("u_Texture", 0);

  glActiveTexture(GL_TEXTURE0);
  m_ActiveTexture->Bind();

  glBindVertexArray(m_Vao);
  glDrawElements(GL_TRIANGLES, m_QuadCount * 6, GL_UNSIGNED_INT, nullptr);

  m_Stats.IncrementDrawCalls();
  m_Stats.IncrementQuadCount(m_QuadCount);
}

void BatchRenderer2D::DrawSprite(const Sprite& sprite, const Texture& texture) {
  if (m_QuadCount >= k_MaxQuads) {
    EndBatch();
    Flush();
    BeginBatch();
  }

  if (m_ActiveTexture == nullptr) {
    m_ActiveTexture = &texture;
  } else if (m_ActiveTexture != &texture) {
    EndBatch();
    Flush();
    BeginBatch();
    m_ActiveTexture = &texture;
  }

  const float x = sprite.transform.x;
  const float y = sprite.transform.y;
  const float width = sprite.transform.width;
  const float height = sprite.transform.height;
  const float rotation = sprite.transform.rotation;

  const glm::vec2 center = {x + width * 0.5f, y + height * 0.5f};

  const float radians = glm::radians(rotation);
  const float cosTheta = std::cos(radians);
  const float sinTheta = std::sin(radians);

  const glm::mat2 rotationMatrix = glm::mat2(cosTheta, -sinTheta, sinTheta, cosTheta);

  const glm::vec2 local0 = {-width * 0.5f, height * 0.5f};
  const glm::vec2 local1 = {width * 0.5f, height * 0.5f};
  const glm::vec2 local2 = {width * 0.5f, -height * 0.5f};
  const glm::vec2 local3 = {-width * 0.5f, -height * 0.5f};

  const glm::vec2 world0 = center + rotationMatrix * local0;
  const glm::vec2 world1 = center + rotationMatrix * local1;
  const glm::vec2 world2 = center + rotationMatrix * local2;
  const glm::vec2 world3 = center + rotationMatrix * local3;

  m_VertexBufferPtr->position = {world0.x, world0.y, 0.0f};
  m_VertexBufferPtr->texCoord = {sprite.uvMin.x, sprite.uvMax.y};
  m_VertexBufferPtr->colour = sprite.colour;
  m_VertexBufferPtr++;

  m_VertexBufferPtr->position = {world1.x, world1.y, 0.0f};
  m_VertexBufferPtr->texCoord = {sprite.uvMax.x, sprite.uvMax.y};
  m_VertexBufferPtr->colour = sprite.colour;
  m_VertexBufferPtr++;

  m_VertexBufferPtr->position = {world2.x, world2.y, 0.0f};
  m_VertexBufferPtr->texCoord = {sprite.uvMax.x, sprite.uvMin.y};
  m_VertexBufferPtr->colour = sprite.colour;
  m_VertexBufferPtr++;

  m_VertexBufferPtr->position = {world3.x, world3.y, 0.0f};
  m_VertexBufferPtr->texCoord = {sprite.uvMin.x, sprite.uvMin.y};
  m_VertexBufferPtr->colour = sprite.colour;
  m_VertexBufferPtr++;

  m_QuadCount++;
}

void BatchRenderer2D::EndScene() {
  EndBatch();
  Flush();
}

const RendererStats& BatchRenderer2D::GetStats() const {
  return m_Stats;
}

void BatchRenderer2D::ResetStats() {
  m_Stats.Reset();
}