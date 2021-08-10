#ifndef GCAMEAR_H
#define GCAMEAR_H

#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QPoint>

class GCamera
{
public:
    GCamera();

public:
    void setViewMatrix(QVector3D position, float xDegree = 0, float yDegree = 0, float zDegree = 0);
    void setViewMatrix(QVector3D position, QVector3D forward, QVector3D up = QVector3D(0.0f, 1.0f, 0.0f) );
    void setOrthMatrix(float size, float aspect, float n, float f); // aspect = width/height
    void setProjMatrix(float fov,  float aspect, float n, float f);
    void setViewPortMatrix(float x, float y, float w, float h);

    QPoint ndcToScreenPoint(QVector3D pos);

public:
    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影
    QMatrix4x4 m_viewPortMat;
    bool m_isOrth;
};

#endif // GCAMEAR_H
