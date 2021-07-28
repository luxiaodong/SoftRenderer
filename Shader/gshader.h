#ifndef GSHADER_H
#define GSHADER_H

#include <QColor>
#include <QVector4D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QImage>
#include <QMap>
#include <QString>
#include "Raster/gvertexattribute.h"

class GShader
{
public:
    GShader();

    GVertexAttribute vertex(GVertexAttribute& va);

    void tessellator(){}
    void geometry(){}
    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

    QVector3D objectToWorldDir(QVector3D pos);
    QVector4D objectToClipping(QVector4D pos);
    QColor color(QImage& image, QVector2D uv);

public:
    QMatrix4x4 m_modelMat; //模型矩阵
    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影

    QImage m_white;
};

#endif // GSHADER_H
