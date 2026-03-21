#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float left, float right, float bottom, float top) :
  m_Projection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
  m_View(1.0f),
  m_Position(0.0f),
  m_Left(left),
  m_Right(right),
  m_Bottom(bottom),
  m_Top(top) {
  RecalculateViewMatrix();
}

void Camera::SetPosition(const glm::vec3& position) {
  m_Position = position;
  RecalculateViewMatrix();
}

const glm::vec3& Camera::GetPosition() const {
  return m_Position;
}

const glm::mat4& Camera::GetProjection() const {
  return m_Projection;
}

const glm::mat4& Camera::GetView() const {
  return m_View;
}

glm::mat4 Camera::GetViewProjection() const {
  return m_Projection * m_View;
}

void Camera::RecalculateViewMatrix() {
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
  m_View = glm::inverse(transform);
}

float Camera::GetLeft() const {
  return m_Left;
}

float Camera::GetRight() const {
  return m_Right;
}

float Camera::GetBottom() const {
  return m_Bottom;
}

float Camera::GetTop() const {
  return m_Top;
}