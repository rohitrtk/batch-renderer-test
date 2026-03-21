#pragma once

class RendererStats {
public:
  RendererStats();

  void Reset();
  void IncrementDrawCalls();
  void IncrementQuadCount();

  unsigned int GetDrawCalls() const;
  unsigned int GetQuadCount() const;

private:
  unsigned int m_DrawCalls;
  unsigned int m_QuadCount;
};