#ifndef SHSHADER_H
#define SHSHADER_H

#include <QVector3D>
#include "Shader/gshader.h"

class SHShader : public GShader
{
public:
    SHShader(QColor color=Qt::white);
    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

    QVector3D m_lightDir;
};

#endif // SHSHADER_H
