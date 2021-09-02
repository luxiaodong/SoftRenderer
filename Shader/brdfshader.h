#ifndef BRDFSHADER_H
#define BRDFSHADER_H

#include "Shader/gshader.h"
#include <QVector3D>

class BRDFShader : public GShader
{
public:
    BRDFShader(QColor color=Qt::white);
    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

private:
    void initArgs();
    float factor_d(QVector3D h, QVector3D n, float r);
    float factor_g_sub(QVector3D h, QVector3D v, float r);
    float factor_g(QVector3D h, QVector3D l, QVector3D v, float r);
    QVector3D factor_f(QVector3D h, QVector3D v, QVector3D F0);

public:
    float m_roughness;
    float m_metalness;
    QVector3D m_fresnel;
    QVector3D m_f0;

private:
    QVector3D m_lightDir;
};

#endif // BRDFSHADER_H
