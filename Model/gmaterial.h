#ifndef GMATERIAL_H
#define GMATERIAL_H

#include <QImage>
#include <QColor>
#include <QPoint>
#include <QVector2D>
#include "Shader/gshader.h"

class GMaterial
{
public:
    GMaterial();
    QColor baseColor(QVector2D uv);
    void setShader(GShader* pShader){m_pShader = pShader;}

private:
    QImage m_baseImage;


// 各种贴图.
    GShader* m_pShader;
};

#endif // GMATERIAL_H
