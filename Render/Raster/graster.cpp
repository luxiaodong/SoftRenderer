#include "graster.h"

GRaster::GRaster()
{
    m_frameBuffer = 0;
    m_depthBuffer = 0;
}

void GRaster::createBuffer()
{
    m_frameBuffer = new GFrameBuffer(m_size.width(), m_size.height(), 3);
    m_depthBuffer = new GDepthBuffer(m_size.width(), m_size.height());
}

float* GRaster::doRendering()
{
    m_frameBuffer->clearColor(m_color.redF(), m_color.greenF(), m_color.blueF());

    return m_frameBuffer->m_data;
}
