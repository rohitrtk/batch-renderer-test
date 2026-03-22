#include "shader.h"

#include "utils/file_util.h"

constexpr int k_ErrorInfoLogSize = 512;

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
  std::string vertexCodeStr = Utils::LoadFileToString(vertexPath);
  std::string fragmentCodeStr = Utils::LoadFileToString(fragmentPath);

  if (vertexCodeStr.empty()) {
    throw std::runtime_error("Failed to load vertex shader: " + vertexPath);
  }

  if (fragmentCodeStr.empty()) {
    throw std::runtime_error("Failed to load fragment shader: " + fragmentPath);
  }

  const char* vertexShaderSrc = vertexCodeStr.c_str();
  const char* fragmentShaderSrc = fragmentCodeStr.c_str();

  int success;
  char infoLog[k_ErrorInfoLogSize] = {};

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, k_ErrorInfoLogSize, nullptr, infoLog);
    std::printf("Compilation of the vertex shader failed. Error: %s\n", infoLog);
  }

  unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fragmentShaderSrc, nullptr);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragShader, k_ErrorInfoLogSize, nullptr, infoLog);
    std::printf("Compilation of the fragment shader failed. Error: %s\n", infoLog);
  }

  m_ID = glCreateProgram();
  glAttachShader(m_ID, vertexShader);
  glAttachShader(m_ID, fragShader);
  glLinkProgram(m_ID);

  glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_ID, k_ErrorInfoLogSize, nullptr, infoLog);
    std::printf("Linking shaders failed. Error: %s\n", infoLog);
  }

  glUseProgram(m_ID);
  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);
}

void Shader::Bind() const {
  glUseProgram(m_ID);
}

void Shader::Unbind() const {
  glUseProgram(0);
}

unsigned int Shader::GetID() const {
  return m_ID;
}

void Shader::SetBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string& name, float x, float y) const {
  glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
  glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
  glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
  glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}