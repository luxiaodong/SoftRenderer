#include "gprimitive.h"

GPrimitive::GPrimitive()
{

}

void GPrimitive::setTriangle(QVector4D a, QVector4D b, QVector4D c)
{
    m_a = a;
    m_b = b;
    m_c = c;

    m_colorA = Qt::red;
    m_colorB = Qt::green;
    m_colorC = Qt::blue;
}

void GPrimitive::homogeneousDiv()
{
    //z in [-1,1]
    QVector4D a = QVector4D(m_a.x()/m_a.w(), m_a.y()/m_a.w(), m_a.z()/m_a.w(), 1.0f);
    QVector4D b = QVector4D(m_b.x()/m_a.w(), m_b.y()/m_b.w(), m_b.z()/m_b.w(), 1.0f);
    QVector4D c = QVector4D(m_c.x()/m_a.w(), m_c.y()/m_c.w(), m_c.z()/m_c.w(), 1.0f);

    m_a = a;
    m_b = b;
    m_c = c;
}

bool GPrimitive::isDiscardCullingSuccess()
{
    float value = (m_b.x() - m_a.x())*(m_c.y() - m_a.y()) - (m_b.y() - m_a.y())*(m_c.x() - m_a.x());
    return value < 0.0f;
}

bool GPrimitive::isTriangleInFrustum()
{
    if (!this->isPointInFrustum(m_a)) return false;
    if (!this->isPointInFrustum(m_b)) return false;
    if (!this->isPointInFrustum(m_c)) return false;
    return true;
}

bool GPrimitive::isPointInFrustum(QVector4D p)
{
    if( p.w() < 0.0001 ) return false;
    if( p.x() < -p.w() || p.x() > p.w() ) return false;
    if( p.y() < -p.w() || p.y() > p.w() ) return false;
    if( p.z() < -p.w() || p.z() > p.w() ) return false;
    return true;
}

QList<QVector4D> GPrimitive::culling()
{
    QList<QVector4D> list;
    list.append(m_a);
    list.append(m_b);
    list.append(m_c);
    list = this->cullingSinglePlane(list, 0);
    list = this->cullingSinglePlane(list, 1);
    list = this->cullingSinglePlane(list, 2);
    list = this->cullingSinglePlane(list, 3);
    list = this->cullingSinglePlane(list, 4);
    list = this->cullingSinglePlane(list, 5);
    return list;
}

// 一条直线和一个平面只有一个交点
QList<QVector4D> GPrimitive::cullingSinglePlane(QList<QVector4D>& inList, int plane)
{
    QList<QVector4D> outList;

    for(int i=0; i< inList.size(); ++i)
    {
        int next = i+1;
        if(next == inList.size()) next = 0;

        QVector4D curr_pos = inList.at(i);
        QVector4D next_pos = inList.at(next);

        if(this->isPointInFrustum(curr_pos))
        {
            outList.append(curr_pos);
        }

        float percent = this->calculateCullingPercent(curr_pos, next_pos, plane);

        if(percent > 0.00001 && percent < 0.99999)
        {
            QVector4D newPos = curr_pos + next_pos*percent;
            outList.append(newPos);
        }
    }

    return outList;
}

// p = p1 + t*(p2 - p1), and w = 1.
float GPrimitive::calculateCullingPercent(QVector4D p1, QVector4D p2, int plane)
{
    if(plane == 0) // w=-x
    {
        return calculateCullingPercent(p1.x(), p2.x(), p1.w(), p2.w(), -1);
    }
    else if(plane == 1) // w=x
    {
        return calculateCullingPercent(p1.x(), p2.x(), p1.w(), p2.w(), 1);
    }
    else if(plane == 2) // w=-y
    {
        return calculateCullingPercent(p1.y(), p2.y(), p1.w(), p2.w(), -1);
    }
    else if(plane == 3) // w=y
    {
        return calculateCullingPercent(p1.y(), p2.y(), p1.w(), p2.w(), 1);
    }
    else if(plane == 4) // w=-z
    {
        return calculateCullingPercent(p1.z(), p2.z(), p1.w(), p2.w(), -1);
    }
    else if(plane == 5) // w=z
    {
        return calculateCullingPercent(p1.z(), p2.z(), p1.w(), p2.w(), 1);
    }

    return 0.0f;
}

float GPrimitive::calculateCullingPercent(int x1, int x2, int w1, int w2, int sign)
{
    float numerator = sign*w1 - x1;
    float denominator = (x2-x1) - sign*(w2-w1);
    return numerator/denominator;
}
