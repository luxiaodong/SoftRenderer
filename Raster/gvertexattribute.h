#ifndef GVERTEXATTRIBUTE_H
#define GVERTEXATTRIBUTE_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QDataStream>

// 一个顶点的信息
class GVertexAttribute
{
public:
    GVertexAttribute();
    GVertexAttribute(QVector3D p, QVector2D u, QVector3D n);
    GVertexAttribute(QVector4D p, QVector2D u, QVector3D n):m_vertex(p),m_uv(u),m_normal(n){}

    void printInfo();

public:
    QVector4D m_vertex;
    QVector2D m_uv;
    QVector3D m_normal;
};

#endif // GVERTEXATTRIBUTE_H
