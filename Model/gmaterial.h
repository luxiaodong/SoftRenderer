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
    GMaterial(){}
    GMaterial(GShader* shader):m_pShader(shader){}
    void setShader(GShader* shader){m_pShader = shader;}

    void addImage(QString key, QString filePath);
public:
    GShader* m_pShader;
    QMap<QString, QImage> m_imageSet;
};

#endif // GMATERIAL_H
