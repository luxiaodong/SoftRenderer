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

void GRaster::setVertexAttribute(int attId, GVertexAttributeBuffer* vBuffer)
{
    if (m_vertexs.contains(attId))
    {
        GVertexAttributeBuffer* p = m_vertexs.value(attId);
        delete []p;
        m_vertexs.remove(attId);
    }

    m_vertexs.insert(attId, vBuffer);
}

float* GRaster::doRendering()
{
    m_frameBuffer->clearColor(m_color.redF(), m_color.greenF(), m_color.blueF());

    //顶点着色器
    QMap<int, GVertexAttributeBuffer*>::const_iterator i = m_vertexs.constBegin();
    while( i != m_vertexs.constEnd())
    {
        m_shader.vertex(i.value());
        ++i;
    }

    return m_frameBuffer->m_data;
}
