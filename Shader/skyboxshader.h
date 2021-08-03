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
    QVector2D uvIn6Side(QString key, QVector2D uv);

    QVector2D uv(QString key, QVector3D normal);
};

#endif // SKYBOXSHADER_H
