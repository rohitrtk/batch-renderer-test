#include "application.h"
#include "pch.h"
#include "renderer/renderer_basic.h"
#include "renderer/renderer_batch.h"
#include "renderer/camera.h"
#include "renderer/texture.h"
#include "renderer/texture_atlas.h"
#include "scene/sprite.h"
#include "utils/random_util.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
  if (app) {
    app->OnFramebufferSizeChanged(width, height);
  }
}

void MouseCallback(GLFWwindow* window, double x, double y) {
  auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
  if (app) {
    app->OnMouseMoved(x, y);
  }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
  if (app) {
    app->OnKeyChanged(key, scancode, action, mods);
  }
}

Application::Application() :
  m_Name("Batch Renderer Test"),
  m_Window(nullptr),
  m_Renderer(nullptr),
  m_Camera(nullptr),
  m_Texture(nullptr),
  m_TextureAtlas(nullptr),
  m_Sprites(100) {
  if (!glfwInit()) {
    throw std::runtime_error("GLFW failed to initialize.");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_Window = glfwCreateWindow(k_WindowWidth, k_WindowHeight, m_Name.c_str(), nullptr, nullptr);
  if (!m_Window) {
    glfwTerminate();
    throw std::runtime_error("GLFW failed to create window.");
  }

  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  if (!monitor) {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    throw std::runtime_error("GLFW failed to retrieve the primary monitor.");
  }

  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  if (!mode) {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    throw std::runtime_error("GLFW failed to retrieve the video mode.");
  }

  glfwSetWindowPos(m_Window, static_cast<int>((mode->width - k_WindowWidth) * 0.5f),
                   static_cast<int>((mode->height - k_WindowHeight) * 0.5f));
  glfwMakeContextCurrent(m_Window);

  if (!gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress))) {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    throw std::runtime_error("GLAD failed to load.");
  }

  glViewport(0, 0, k_WindowWidth, k_WindowHeight);
  glfwSetWindowUserPointer(m_Window, this);
  glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);
  glfwSetCursorPosCallback(m_Window, MouseCallback);
  glfwSetKeyCallback(m_Window, KeyCallback);

  m_Renderer = std::make_unique<BatchRenderer2D>();
  m_Camera =
      std::make_unique<Camera>(0.0f, static_cast<float>(k_WindowWidth), 0.0f, static_cast<float>(k_WindowHeight));
  m_Texture = std::make_unique<Texture>("assets/textures/atlas.png");
  m_TextureAtlas =
      std::make_unique<GridTextureAtlas>(m_Texture->GetWidth(), m_Texture->GetHeight(), 64.0f, 64.0f, 64.0f, 64.0f);

  for (auto& sprite : m_Sprites) {
    glm::vec2 pos(
        Utils::Random::Vec2({0.0f, 0.0f}, {static_cast<float>(k_WindowWidth), static_cast<float>(k_WindowHeight)}));
    // glm::vec3 col(Utils::Random::Vec3({0.1f, 0.1f, 0.1f}, {1.0f, 1.0f, 1.0f}));
    glm::vec3 col(1.0f);

    sprite.transform.x = pos.x;
    sprite.transform.y = pos.y;
    sprite.transform.width = 64.0f;
    sprite.transform.height = 64.0f;
    sprite.colour = glm::vec4(col.x, col.y, col.z, 1.0f);

    // sprite.uvMin = {0.0f, 0.0f};
    // sprite.uvMax = {1.0f, 1.0f};
    auto region = m_TextureAtlas->GetRegion(0, 0);
    sprite.uvMin = region.uvMin;
    sprite.uvMax = region.uvMax;
  }
}

Application::~Application() {
  if (m_Window) {
    glfwDestroyWindow(m_Window);
    m_Window = nullptr;
  }

  glfwTerminate();
}

void Application::Run() {
  float lastTime = static_cast<float>(glfwGetTime());
  float timer = 0.0f;
  int frameCount = 0;
  int updateCount = 0;

  while (!glfwWindowShouldClose(m_Window)) {
    const float currentTime = static_cast<float>(glfwGetTime());
    const float dt = currentTime - lastTime;
    lastTime = currentTime;

    glfwPollEvents();

    Update(dt);
    updateCount++;

    m_Renderer->ResetStats();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Render();
    frameCount++;

    timer += dt;
    if (timer >= 1.0f) {
      const auto& stats = m_Renderer->GetStats();
      std::printf("FPS: %d | UPS: %d | DrawCalls: %u | Quads: %u\n", frameCount, updateCount, stats.GetDrawCalls(),
                  stats.GetQuadCount());

      frameCount = 0;
      updateCount = 0;
      timer = 0.0f;
    }

    glfwSwapBuffers(m_Window);
  }
}

void Application::Update(float dt) {
  for (auto& sprite : m_Sprites) {
    sprite.transform.rotation += 45.0f * dt;
  }
}

void Application::Render() {
  m_Renderer->BeginScene(*m_Camera);
  for (auto& sprite : m_Sprites) {
    m_Renderer->DrawSprite(sprite, *m_Texture);
  }
  m_Renderer->EndScene();
}

void Application::OnFramebufferSizeChanged(int width, int height) {
  glViewport(0, 0, width, height);
}

void Application::OnMouseMoved(double x, double y) {}

void Application::OnKeyChanged(int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(m_Window, true);
  }
}