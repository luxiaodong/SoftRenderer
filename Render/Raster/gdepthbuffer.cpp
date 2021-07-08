#include "gdepthbuffer.h"

GDepthBuffer::GDepthBuffer(int width, int height):m_width(width), m_height(height)
{
    m_data = new float[width*height];
}

GDepthBuffer::~GDepthBuffer()
{
    delete []m_data;
}
