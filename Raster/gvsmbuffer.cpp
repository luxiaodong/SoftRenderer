#include "gvsmbuffer.h"
#include <QtMath>

GVSMBuffer::GVSMBuffer(int width, int height):m_width(width), m_height(height)
{
    m_depth = new float[width*height];
    m_depthSquare = new float[width*height];
    m_depthAverage = new float[width*height];
    m_depthSquareAverage = new float[width*height];
    m_radius = 3;
}

void GVSMBuffer::clear(float depth)
{
    for(int j=0; j < m_height; ++j)
    {
        for(int i=0; i < m_width; ++i)
        {
            this->setDepth(i, j, depth);
        }
    }
}

void GVSMBuffer::setDepth(int i, int j, float depth)
{
    m_depth[(j*m_width + i)] = depth;
    m_depthSquare[(j*m_width + i)] = depth*depth;
}

float GVSMBuffer::depth(int x, int y)
{
    return m_depth[(y*m_width + x)];
}

float GVSMBuffer::depthAverage(int x, int y)
{
    return m_depthAverage[(y*m_width + x)];
}

float GVSMBuffer::depthSquare(int x, int y)
{
    return m_depthSquare[(y*m_width + x)];
}

void GVSMBuffer::genAverageDepth()
{
    for(int j=0; j < m_height; ++j)
    {
        for(int i=0; i < m_width; ++i)
        {
            int wmin = qMin(i, m_width-1-i);
            int hmin = qMin(j, m_height-1-j);
            int range = qMin(wmin, hmin);
            range = qMin(range, m_radius);
            this->setAverageDepth(i, j, range);
        }
    }
}

void GVSMBuffer::setAverageDepth(int x, int y, int range)
{
    int count = 0;
    float depthTotal = 0;
    float depthSquareTotal = 0;
    for(int j=y-range; j <= y+range; ++j)
    {
        for(int i=x-range; i <= x+range; ++i)
        {
            depthTotal += this->depth(i,j);
            depthSquareTotal += this->depthSquare(x,y);
            count ++;
        }
    }

    m_depthAverage[(y*m_width + x)] = depthTotal/count;
    m_depthSquareAverage[(y*m_width + x)] = depthSquareTotal/count;
}

float GVSMBuffer::chebyshev(int x, int y, float t)
{
    float ex = m_depthAverage[(y*m_width + x)];
    float ex2 = m_depthSquareAverage[(y*m_width + x)];
    float vx = ex2 - ex*ex;
    float tex2 = (t-ex)*(t-ex);
//    if(t - 0.001 <= ex) return 0.0f;
    return vx/(vx + tex2);
}

GVSMBuffer::~GVSMBuffer()
{
    delete []m_depth;
    delete []m_depthSquare;
    delete []m_depthAverage;
    delete []m_depthSquareAverage;
}
