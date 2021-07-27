#include "gdepthbuffer.h"

GDepthBuffer::GDepthBuffer(int width, int height):m_width(width), m_height(height)
{
    m_data = new float[width*height];
}

void GDepthBuffer::clear()
{
    for(int j=0; j < m_height; ++j)
    {
        for(int i=0; i < m_width; ++i)
        {
            m_data[(j*m_width + i)] = 1.0f;
        }
    }
}

void GDepthBuffer::setDepth(int x, int y, float depth)
{
    m_data[(y*m_width + x)] = depth;
}

float GDepthBuffer::depth(int x, int y)
{
    return m_data[(y*m_width + x)];
}

GDepthBuffer::~GDepthBuffer()
{
    delete []m_data;
}
