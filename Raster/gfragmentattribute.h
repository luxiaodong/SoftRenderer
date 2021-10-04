#ifndef GFRAGMENTATTRIBUTE_H
#define GFRAGMENTATTRIBUTE_H

#include <QPoint>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include "Raster/gvertexattribute.h"
#include "Shader/gshader.h"
#include "Model/gmaterial.h"

class GFragmentAttribute
{
public:
    GFragmentAttribute();

public:
    QPoint m_pt;
    float m_depth;
    GVertexAttribute m_va;
    GShader* m_pShader;
    GMaterial* m_material;
};

#endif // GFRAGMENTATTRIBUTE_H
