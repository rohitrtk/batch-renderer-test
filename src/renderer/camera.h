#pragma once

#include <glm/glm.hpp>

class Camera {
public:
  Camera(float left, float right, float bottom, float top);

  void SetPosition(const glm::vec3& position);
  const glm::vec3& GetPosition() const;

  const glm::mat4& GetProjection() const;
  const glm::mat4& GetView() const;
  glm::mat4 GetViewProjection() const;

  void RecalculateViewMatrix();

  float GetLeft() const;
  float GetRight() const;
  float GetBottom() const;
  float GetTop() const;

private:
  glm::mat4 m_Projection;
  glm::mat4 m_View;
  glm::vec3 m_Position;

  float m_Left;
  float m_Right;
  float m_Bottom;
  float m_Top;
};