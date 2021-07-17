#ifndef GPRIMITIVE_H
#define GPRIMITIVE_H

#include <QVector2D>
#include <QVector4D>
#include <QColor>
#include <QList>
#include <QList>

class GPrimitive
{
public:
    GPrimitive();
    void setTriangle(QVector4D a, QVector4D b, QVector4D c);
    void setColor(QColor a, QColor b, QColor c);
    void homogeneousDiv();
    bool isDiscardCullingSuccess();
    bool isTriangleInFrustum();
    bool isPointInFrustum(QVector4D p);

    QList<QVector4D> culling();
    QList<QVector4D> cullingSinglePlane(QList<QVector4D>&, int plane);
    float calculateCullingPercent(QVector4D a, QVector4D b, int plane);
    float calculateCullingPercent(int x1, int x2, int w1, int w2, int sign);

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
