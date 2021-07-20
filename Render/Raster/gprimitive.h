#ifndef GPRIMITIVE_H
#define GPRIMITIVE_H

#include <QVector3D>
#include <QVector4D>
#include <QColor>
#include <QList>
#include <QMap>

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
    void setTriangle(QVector4D a, QVector4D b, QVector4D c);
    void setColor(QColor a, QColor b, QColor c);
    void homogeneousDiv();
    bool isDiscardCullingSuccess() const;
    bool isTriangleInFrustum() const;
    bool isPointInFrustum(QVector4D p) const;
    bool isPointInSinglePlane(QVector4D p, int plane) const;

    QList<GVertexCullingRatio> culling() const;
    QList<GVertexCullingRatio> cullingSinglePlane(QList<GVertexCullingRatio>&, int plane) const;
    float calculateCullingPercent(QVector4D a, QVector4D b, int plane) const;
    float calculateCullingPercent(float x1, float x2, float w1, float w2, int sign) const;

//三个顶点
    QVector4D m_a;
    QVector4D m_b;
    QVector4D m_c;

//三个uv值
    QColor m_colorA;
    QColor m_colorB;
    QColor m_colorC;
};

#endif // GPRIMITIVE_H
