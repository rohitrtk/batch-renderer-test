#include "random_util.h"

namespace Utils {

  std::mt19937 Random::s_Engine{std::random_device{}()};

  void Random::Init() {
    s_Engine.seed(std::random_device{}());
  }

  int Random::Int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(s_Engine);
  }

  float Random::Float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(s_Engine);
  }

  double Random::Double(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(s_Engine);
  }

  glm::vec2 Random::Vec2(const glm::vec2& min, const glm::vec2& max) {
    float x = Float(min.x, max.x);
    float y = Float(min.y, max.y);
    return glm::vec2(x, y);
  }

  glm::vec3 Random::Vec3(const glm::vec3& min, const glm::vec3& max) {
    float x = Float(min.x, max.x);
    float y = Float(min.y, max.y);
    float z = Float(min.z, max.z);
    return glm::vec3(x, y, z);
  }

  glm::vec4 Random::Vec4(const glm::vec4& min, const glm::vec4& max) {
    float x = Float(min.x, max.x);
    float y = Float(min.y, max.y);
    float z = Float(min.z, max.z);
    float w = Float(min.w, max.w);
    return glm::vec4(x, y, z, w);
  }

  bool Random::Bool() {
    return Int(0, 1) == 1;
  }

  int Random::Sign() {
    return Bool() ? 1 : -1;
  }
} // namespace Utils