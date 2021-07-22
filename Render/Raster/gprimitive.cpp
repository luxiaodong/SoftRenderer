#include "gprimitive.h"
#include <QDebug>

GPrimitive::GPrimitive()
{

}

GPrimitive::GPrimitive(GVertexAttribute& a, GVertexAttribute& b, GVertexAttribute& c)
{
    m_triangle[0] = a;
    m_triangle[1] = b;
    m_triangle[2] = c;
}

QList<GVertexCullingRatio> GPrimitive::culling() const
{
    QList<GVertexCullingRatio> list;
    list.append(GVertexCullingRatio(m_triangle[0].m_vertex, QVector3D(1.0, 0.0, 0.0)));
    list.append(GVertexCullingRatio(m_triangle[1].m_vertex, QVector3D(0.0, 1.0, 0.0)));
    list.append(GVertexCullingRatio(m_triangle[2].m_vertex, QVector3D(0.0, 0.0, 1.0)));

    list = this->cullingSinglePlane(list, 0);
    list = this->cullingSinglePlane(list, 1);
    list = this->cullingSinglePlane(list, 2);
    list = this->cullingSinglePlane(list, 3);
    list = this->cullingSinglePlane(list, 4);
    list = this->cullingSinglePlane(list, 5);

    return list;
}


// 一条直线和一个平面只有一个交点
QList<GVertexCullingRatio> GPrimitive::cullingSinglePlane(QList<GVertexCullingRatio>& inList, int plane) const
{
    QList<GVertexCullingRatio> outList;

    for(int i=0; i< inList.size(); ++i)
    {
        int next = i+1;
        if(next == inList.size()) next = 0;

        QVector4D curr_pos = inList.at(i).m_point;
        QVector4D next_pos = inList.at(next).m_point;
        QVector3D curr_rat = inList.at(i).m_ratio;
        QVector3D next_rat = inList.at(next).m_ratio;

        if(this->isPointInSinglePlane(curr_pos, plane))
        {
            outList.append(GVertexCullingRatio(curr_pos, curr_rat));
        }

        float percent = this->calculateCullingPercent(curr_pos, next_pos, plane);
        if(percent > 0.00001 && percent < 0.99999)
        {
            QVector4D new_pos = curr_pos*(1-percent) + next_pos*percent;
            QVector3D new_rat = curr_rat*(1-percent) + next_rat*percent;
            outList.append(GVertexCullingRatio(new_pos, new_rat));
        }
    }

    return outList;
}

bool GPrimitive::isTriangleInFrustum() const
{
    for(int i = 0; i < 2; ++i)
    {
        if (!this->isPointInFrustum(m_triangle[i].m_vertex))
        {
            return false;
        }
    }

    return true;
}

bool GPrimitive::isPointInFrustum(QVector4D p) const
{
    if( p.w() < 0.0001 ) return false;
    if( !this->isPointInSinglePlane(p, 0) ) return false;
    if( !this->isPointInSinglePlane(p, 1) ) return false;
    if( !this->isPointInSinglePlane(p, 2) ) return false;
    if( !this->isPointInSinglePlane(p, 3) ) return false;
    if( !this->isPointInSinglePlane(p, 4) ) return false;
    if( !this->isPointInSinglePlane(p, 5) ) return false;
    return true;
}

bool GPrimitive::isPointInSinglePlane(QVector4D p, int plane) const
{
    if(plane == 0) // w=-x
    {
        return p.x() >= -p.w();
    }
    else if(plane == 1) // w=x
    {
        return p.x() <= p.w();
    }
    else if(plane == 2) // w=-y
    {
        return p.y() >= -p.w();
    }
    else if(plane == 3) // w=y
    {
        return p.y() <= p.w();
    }
    else if(plane == 4) // w=-z
    {
        return p.z() >= -p.w();
    }
    else if(plane == 5) // w=z
    {
        return p.z() <= p.w();
    }

    return false;
}


// p = p1 + t*(p2 - p1).
float GPrimitive::calculateCullingPercent(QVector4D p1, QVector4D p2, int plane) const
{
    if(plane == 0) // w=-x
    {
        return this->calculateCullingPercent(p1.x(), p2.x(), p1.w(), p2.w(), -1);
    }
    else if(plane == 1) // w=x
    {
        return this->calculateCullingPercent(p1.x(), p2.x(), p1.w(), p2.w(), 1);
    }
    else if(plane == 2) // w=-y
    {
        return this->calculateCullingPercent(p1.y(), p2.y(), p1.w(), p2.w(), -1);
    }
    else if(plane == 3) // w=y
    {
        return this->calculateCullingPercent(p1.y(), p2.y(), p1.w(), p2.w(), 1);
    }
    else if(plane == 4) // w=-z
    {
        return this->calculateCullingPercent(p1.z(), p2.z(), p1.w(), p2.w(), -1);
    }
    else if(plane == 5) // w=z
    {
        return this->calculateCullingPercent(p1.z(), p2.z(), p1.w(), p2.w(), 1);
    }

    return 0.0f;
}

float GPrimitive::calculateCullingPercent(float x1, float x2, float w1, float w2, int sign) const
{
    float numerator = sign*w1 - x1;
    float denominator = (x2-x1) - sign*(w2-w1);
    return numerator/denominator;
}

bool GPrimitive::isDiscardCullingSuccess() const
{
    QVector4D a = m_triangle[0].m_vertex;
    QVector4D b = m_triangle[1].m_vertex;
    QVector4D c = m_triangle[2].m_vertex;
    float value = (b.x() - a.x())*(c.y() - a.y()) - (b.y() - a.y())*(c.x() - a.x());
    return value < 0.0f;
}

void GPrimitive::homogeneousDiv()
{
    for(int i = 0; i < 2; ++i)
    {
        QVector4D p = m_triangle[i].m_vertex;
        p = QVector4D(p.x()/p.w(), p.y()/p.w(), p.z()/p.w(), 1.0f);
        m_triangle[i].m_vertex = p;
    }
}

GVertexAttribute GPrimitive::interpolationAttribute(QVector3D ratio)
{
    float alpha = ratio.x();
    float beta  = ratio.y();
    float gamma = ratio.z();

    QVector4D vertex = alpha*m_triangle[0].m_vertex + beta*m_triangle[1].m_vertex + gamma*m_triangle[2].m_vertex;
    QVector2D uv     = alpha*m_triangle[0].m_uv     + beta*m_triangle[1].m_uv     + gamma*m_triangle[2].m_uv;
    QVector3D normal = alpha*m_triangle[0].m_normal + beta*m_triangle[1].m_normal + gamma*m_triangle[2].m_normal;

    return GVertexAttribute(vertex, uv, normal);
}
