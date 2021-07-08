#include "ggraphicsapi.h"
#include "Render/Raster/graster.h"
#include <QtMath>
#include <QMatrix4x4>

GGraphicsAPI::GGraphicsAPI()
{
//    m_pRender = new GRender();
    m_pRender = new GRaster();
}

void GGraphicsAPI::setRenderSize(const QSize& size)
{
    m_pRender->setRenderSize(size);
    m_pRender->createBuffer();
}

void GGraphicsAPI::setClearColor(const QColor& color)
{
    m_pRender->setClearColor(color);
}

void GGraphicsAPI::setViewMatrix(QVector3D forward, QVector3D up)
{
    // right, up, forward, (x,y,z)
    QVector3D right = QVector3D::crossProduct(up, forward);
    QVector3D up2 = QVector3D::crossProduct(forward, right);

    QMatrix4x4 mat;
    m_pRender->setViewMatrix(mat);
}

void GGraphicsAPI::setOrthMatrix(float size, float aspect, float n, float f)
{
    // z in [-1,1]
    QMatrix4x4 mat(
        1.0f/(aspect*size),      0.0f,          0.0f,          0.0f,
                0.0f,       1.0f/size,          0.0f,          0.0f,
                0.0f,            0.0f,       2/(n-f),   (n+f)/(n-f),
                0.0f,            0.0f,         -1.0f,         0.0f);

    m_pRender->setProjMatrix(mat);
}

void GGraphicsAPI::setProjMatrix(float fov, float aspect, float n, float f)
{
    // z in [-1,1]
    float tan = qTan( qDegreesToRadians(fov/2) );
    QMatrix4x4 mat(
        1.0f/(aspect*tan),       0.0f,          0.0f,          0.0f,
                0.0f,        1.0f/tan,          0.0f,          0.0f,
                0.0f,            0.0f,   (f+n)/(n-f),   2*n*f/(n-f),
                0.0f,            0.0f,         -1.0f,         0.0f);

    m_pRender->setProjMatrix(mat);
}

float* GGraphicsAPI::doRendering()
{
    return m_pRender->doRendering();
}
