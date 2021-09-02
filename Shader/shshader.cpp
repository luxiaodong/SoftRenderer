#include "shshader.h"

SHShader::SHShader(QColor color):GShader(color)
{
    m_lightDir = QVector3D(0,0,-1);
    m_lightDir.normalize();
}

QColor SHShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QImage image = map.value("base", m_white);
    QColor color = this->color(image, va.m_uv);
    float ndotl =  QVector3D::dotProduct(m_lightDir, va.m_normal)*0.5f + 0.5f;
    float r = ndotl*color.redF();
    float g = ndotl*color.greenF();
    float b = ndotl*color.blueF();
    return QColor(r*255, g*255, b*255);
//    return m_white.pixelColor(0,0);
}

