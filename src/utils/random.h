#pragma once

#include <random>
#include <glm/glm.hpp>

class Random {
public:
  static void Init();

  static int Int(int min, int max);
  static float Float(float min, float max);
  static double Double(double min, double max);

  static glm::vec2 Vec2(const glm::vec2& min, const glm::vec2& max);
  static glm::vec3 Vec3(const glm::vec3& min, const glm::vec3& max);
  static glm::vec4 Vec4(const glm::vec4& min, const glm::vec4& max);

  static bool Bool();
  static int Sign();

private:
  static std::mt19937 s_Engine;
};