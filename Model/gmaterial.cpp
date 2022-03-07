#include "gmaterial.h"
#include <QFile>
#include <QImage>
#include <fstream>
#include <QDebug>
#include <QDir>

GMaterial::GMaterial()
{
    m_uvArgs.setRect(0,0,1,1);
}

GMaterial::GMaterial(GShader* shader):m_pShader(shader)
{
    m_uvArgs.setRect(0,0,1,1);
}

void GMaterial::addImage(QString key, QString filePath)
{
    QImage image;

    if (filePath.right(4) == ".tga")
    {
        image = QImage(filePath, "tga");
    }
    else if(filePath.right(4) == ".png")
    {
        image = QImage(filePath, "png");
    }
    else if(filePath.right(4) == ".jpg")
    {
        image = QImage(filePath, "jpg");
    }

    m_imageSet.insert(key, image.mirrored() );
}

void GMaterial::setUVArgs(float offsetX, float offsetY, float scaleX, float scaleY)
{
    m_uvArgs.setRect(static_cast<double>(offsetX),static_cast<double>(offsetY),static_cast<double>(scaleX),static_cast<double>(scaleY));
}
