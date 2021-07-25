#include "gmaterial.h"

GMaterial::GMaterial()
{
    m_baseImage = QImage(1, 1, QImage::Format_RGBA8888);
    m_baseImage.fill(Qt::white);
}

QColor GMaterial::baseColor(QVector2D uv)
{
    QPoint pt = QPoint( uv.x()*m_baseImage.width(), uv.y()*m_baseImage.height()  );
    return m_baseImage.pixelColor(pt.x(), pt.y());
}
