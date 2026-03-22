#pragma once

#include "pch.h"
#include "scene/sprite.h"

struct GLFWwindow;

class Renderer2D;
class BatchRenderer2D;
class Camera;
class Texture;
class GridTextureAtlas;

constexpr int k_WindowWidth = 800;
constexpr int k_WindowHeight = 600;

class Application {
public:
  Application();
  ~Application();

  void Run();

private:
  void Update(float dt);
  void Render();

  void OnFramebufferSizeChanged(int width, int height);
  void OnMouseMoved(double x, double y);
  void OnKeyChanged(int key, int scancode, int action, int mods);

  friend void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
  friend void MouseCallback(GLFWwindow* window, double x, double y);
  friend void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
  std::string m_Name;
  GLFWwindow* m_Window;

  std::unique_ptr<BatchRenderer2D> m_Renderer;
  std::unique_ptr<Camera> m_Camera;
  std::unique_ptr<Texture> m_Texture;
  std::unique_ptr<GridTextureAtlas> m_TextureAtlas;

  std::vector<Sprite> m_Sprites;
};