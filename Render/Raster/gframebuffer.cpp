#include "gframebuffer.h"
#include <QDebug>

GFrameBuffer::GFrameBuffer(int width, int height, int depth):m_width(width),m_height(height),m_depth(depth)
{
    m_data = new float[width*height*depth];
}

void GFrameBuffer::clearColor(float r, float g, float b)
{
    for(int j=0; j < m_height; ++j)
    {
        for(int i=0; i < m_width; ++i)
        {
            m_data[(j*m_width + i)*m_depth] = r;
            m_data[(j*m_width + i)*m_depth + 1] = g;
            m_data[(j*m_width + i)*m_depth + 2] = b;
        }
    }
}

GFrameBuffer::~GFrameBuffer()
{
    delete []m_data;
}
