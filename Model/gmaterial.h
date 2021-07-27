#ifndef GMATERIAL_H
#define GMATERIAL_H

#include <QString>
#include <QImage>
#include <QColor>
#include <QPoint>
#include <QVector2D>
#include <QMap>
#include "Shader/gshader.h"

class GMaterial
{
public:
    GMaterial();
    QColor color(QString texName, QVector2D uv);

public:
    QMap<QString, QImage> m_imageMap;
    GShader* m_pShader;
};

#endif // GMATERIAL_H
