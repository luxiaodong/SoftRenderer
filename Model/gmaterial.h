#ifndef GMATERIAL_H
#define GMATERIAL_H

#include <QString>
#include <QImage>
#include <QColor>
#include <QPoint>
#include <QVector2D>
#include <QMap>
#include <QRectF>
#include "Shader/gshader.h"

class GMaterial
{
public:
    GMaterial();
    GMaterial(GShader* shader);
    void setShader(GShader* shader){m_pShader = shader;}

    void addImage(QString key, QString filePath);
    void setUVArgs(float offsetX, float offsetY, float scaleX, float scaleY);
public:
    QRectF m_uvArgs;
    GShader* m_pShader;
    QMap<QString, QImage> m_imageSet;
};

#endif // GMATERIAL_H
