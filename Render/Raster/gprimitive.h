#ifndef GPRIMITIVE_H
#define GPRIMITIVE_H

#include <QVector4D>

class GPrimitive
{
public:
    GPrimitive();
    void setTriangle(QVector4D a, QVector4D b, QVector4D c);
    void homogeneousDiv();
    bool isDiscardCullingSuccess();

//三个顶点
    QVector4D m_a;
    QVector4D m_b;
    QVector4D m_c;
};

#endif // GPRIMITIVE_H
