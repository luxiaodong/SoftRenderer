#ifndef GMESH_H
#define GMESH_H

#include <QList>
#include <QVector3D>
#include <QColor>

class GVertexIndex
{
public:
    GVertexIndex(){}
    GVertexIndex(int v, int u, int n):m_vertexIndex(v),m_uvIndex(u),m_normalIndex(n){}

public:
    int m_vertexIndex;
    int m_uvIndex;
    int m_normalIndex;
};

class GMesh
{
public:
    GMesh();

    QList<QVector2D> m_uvs;
    QList<QVector3D> m_vertexs;
    QList<QVector3D> m_normals;
    QList<GVertexIndex> m_indexs;
};

#endif // GMESH_H
