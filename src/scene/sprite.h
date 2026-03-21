#pragma once

#include "pch.h"
#include "math/transform.h"

struct Sprite {
  Transform transform;
  float xVelocity = 0.0f;
  float yVelocity = 0.0f;

  glm::vec2 uvMin{0.0f, 0.0f};
  glm::vec2 uvMax{1.0f, 1.0f};

  glm::vec4 colour{1.0f};

  float rotation = 0.0f;
  int layer = 0;
};