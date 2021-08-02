#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H

#include "Shader/gshader.h"

class SkyboxShader : public GShader
{
public:
    SkyboxShader();

    virtual GVertexAttribute vertex(GVertexAttribute& va);
    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

private:
    QString mapKey(QVector3D normal);
    QVector2D uv(QString key, QVector3D normal);

    QVector3D m_cameraPos;
};

#endif // SKYBOXSHADER_H
