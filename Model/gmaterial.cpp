#include "gmaterial.h"
#include <QFile>
#include <QImage>
#include <fstream>
#include <QDebug>
#include <QDir>

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

    m_imageSet.insert(key, image);
}
