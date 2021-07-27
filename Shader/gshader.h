#ifndef GSHADER_H
#define GSHADER_H

#include <QColor>
#include <QVector4D>
#include <QMatrix4x4>
#include <QImage>
#include <QMap>
#include "Raster/gvertexattribute.h"

class GShader
{
public:
    GShader();

    GVertexAttribute vertex(GVertexAttribute& va);

    void tessellator(){}
    void geometry(){}
    QColor fragment(float x, float y, GVertexAttribute& va);

    QVector4D objectToClipping(QVector4D pos);

public:
    QMatrix4x4 m_modelMat; //模型矩阵
    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影
};

#endif // GSHADER_H
