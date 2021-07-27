#include "gvertexattribute.h"

GVertexAttribute::GVertexAttribute()
{

}

GVertexAttribute::GVertexAttribute(QVector3D p, QVector2D u, QVector3D n):m_uv(u),m_normal(n)
{
    m_vertex = QVector4D(p, 1.0f);
}
