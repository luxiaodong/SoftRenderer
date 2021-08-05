#ifndef GPRIMITIVE_H
#define GPRIMITIVE_H

#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include <QList>
#include <QMap>
#include "Raster/gvertexattribute.h"

class GVertexCullingRatio
{
public:
    GVertexCullingRatio(QVector4D p, QVector3D r):m_point(p),m_ratio(r){}
    QVector4D m_point; //顶点
    QVector3D m_ratio; //比例系数
};

class GPrimitive
{
public:
    GPrimitive();
    GPrimitive(GVertexAttribute& a, GVertexAttribute& b, GVertexAttribute& c);
    bool isTriangleInFrustum() const;
    bool isPointInFrustum(QVector4D p) const;
    bool isPointInSinglePlane(QVector4D p, int plane) const;

    QList<QVector3D> homogeneousDiv();
    bool isDiscardCullingSuccess(int cullType) const;

    QList<GVertexCullingRatio> culling() const;
    QList<GVertexCullingRatio> cullingSinglePlane(QList<GVertexCullingRatio>&, int plane) const;
    float calculateCullingPercent(QVector4D a, QVector4D b, int plane) const;
    float calculateCullingPercent(float x1, float x2, float w1, float w2, int sign) const;

    GVertexAttribute interpolationAttribute(QVector3D weight);
public:
    GVertexAttribute  m_triangle[3];
};

#endif // GPRIMITIVE_H
