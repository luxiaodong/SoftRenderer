#include "phongshader.h"
#include <QtMath>
#include <QtGlobal>

PhongShader::PhongShader()
{
    float x = 0.3213938;
    float y = 0.7660444;
    float z = -0.5566705;
    m_lightDir = QVector3D(x, y, z);
}

QColor PhongShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QImage image = map.value("base", m_white);
    QColor color = this->color(image, va.m_uv);
    return color;
//    float ndotl =  qMax(0.0f, QVector3D::dotProduct(m_lightDir, va.m_normal));
//    float r = ndotl*color.redF();
//    float g = ndotl*color.greenF();
//    float b = ndotl*color.blueF();
//    float a = ndotl*color.alphaF();
//    return QColor(r*255, g*255, b*255, a*255);

//    return m_white.pixel(0,0);
}
