#ifndef GSHADER_H
#define GSHADER_H

#include <QMatrix4x4>
#include "Render/Raster/gvertexattributebuffer.h"

class GShader
{
public:
    GShader();

    void vertex(GVertexAttributeBuffer*);
    void fragment();

    QVector3D worldToClipping(QVector3D pos);

public:
    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影
};

#endif // GSHADER_H
