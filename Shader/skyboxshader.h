#ifndef SKYBOXSHADER_H
#define SKYBOXSHADER_H

#include "Shader/gshader.h"

class SkyboxShader : public GShader
{
public:
    SkyboxShader(bool is6Side);

    virtual GVertexAttribute vertex(GVertexAttribute& va);
    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

private:
    QString mapKey(QVector3D normal);
    QVector2D uvIn6Side(QString key, QVector2D uv);
    QVector2D uvInPanoramic(QVector3D normal);

private:
    bool m_is6Side;

};

#endif // SKYBOXSHADER_H
