#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "Shader/gshader.h"
#include <QVector3D>

class PhongShader : public GShader
{
public:
    PhongShader();

    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

private:
    QVector3D m_lightDir;
};

#endif // PHONGSHADER_H
