#include "gframebuffer.h"
#include <QDebug>

GFrameBuffer::GFrameBuffer(int width, int height, int depth):m_width(width),m_height(height),m_depth(depth)
{
    m_data = new int[width*height*depth];
}

void GFrameBuffer::clearColor(QColor color)
{
    for(int j=0; j < m_height; ++j)
    {
        for(int i=0; i < m_width; ++i)
        {
            this->setPixel(i,j,color);
        }
    }
}

void GFrameBuffer::setPixel(int i, int j, QColor color)
{
    int r = color.red();
    int g = color.green();
    int b = color.blue();
    m_data[(j*m_width + i)*m_depth] = r;
    m_data[(j*m_width + i)*m_depth + 1] = g;
    m_data[(j*m_width + i)*m_depth + 2] = b;
}

QColor GFrameBuffer::pixel(int i, int j)
{
    int r = m_data[(j*m_width + i)*m_depth];
    int g = m_data[(j*m_width + i)*m_depth + 1];
    int b = m_data[(j*m_width + i)*m_depth + 2];
    return QColor(r,g,b);
}

GFrameBuffer::~GFrameBuffer()
{
    delete []m_data;
}
