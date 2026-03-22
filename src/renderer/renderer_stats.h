#pragma once

class RendererStats {
public:
  RendererStats();

  void Reset();
  void IncrementDrawCalls(int amount = 1);
  void IncrementQuadCount(int amount = 1);

  unsigned int GetDrawCalls() const;
  unsigned int GetQuadCount() const;

private:
  unsigned int m_DrawCalls;
  unsigned int m_QuadCount;
};