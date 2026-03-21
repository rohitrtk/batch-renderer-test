#include "renderer_stats.h"

RendererStats::RendererStats() :
  m_DrawCalls(0),
  m_QuadCount(0) {}

void RendererStats::Reset() {
  m_DrawCalls = 0;
  m_QuadCount = 0;
}

void RendererStats::IncrementDrawCalls() {
  m_DrawCalls++;
}

void RendererStats::IncrementQuadCount() {
  m_QuadCount++;
}

unsigned int RendererStats::GetDrawCalls() const {
  return m_DrawCalls;
}

unsigned int RendererStats::GetQuadCount() const {
  return m_QuadCount;
}