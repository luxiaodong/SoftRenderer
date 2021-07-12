#ifndef GSHADER_H
#define GSHADER_H

#include <QVector4D>
#include <QMatrix4x4>
#include "Render/Raster/gvertexattribute.h"

class GShader
{
public:
    GShader();

    void vertex(GVertexAttribute*);
    void tessellator(){}
    void geometry(){}
    void fragment();

    QVector4D worldToClipping(QVector4D pos);

public:
    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影
};

#endif // GSHADER_H
