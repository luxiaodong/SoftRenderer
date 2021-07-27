#include "gmaterial.h"
#include <QFile>
#include <QImage>

void GMaterial::addImage(QString key, QString filePath)
{
    QImage image = QImage(filePath, "png");
    m_imageSet.insert(key, image);
}
