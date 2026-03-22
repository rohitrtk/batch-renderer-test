#include "renderer_stats.h"

RendererStats::RendererStats() :
  m_DrawCalls(0),
  m_QuadCount(0) {}

void RendererStats::Reset() {
  m_DrawCalls = 0;
  m_QuadCount = 0;
}

void RendererStats::IncrementDrawCalls(int amount) {
  m_DrawCalls += amount;
}

void RendererStats::IncrementQuadCount(int amount) {
  m_QuadCount += amount;
}

unsigned int RendererStats::GetDrawCalls() const {
  return m_DrawCalls;
}

unsigned int RendererStats::GetQuadCount() const {
  return m_QuadCount;
}