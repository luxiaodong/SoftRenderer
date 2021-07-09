#include "ggraphicsapi.h"
#include "Render/Raster/graster.h"
#include <QtMath>
#include <QMatrix4x4>
#include <QDebug>

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

void GGraphicsAPI::setViewMatrix(QVector3D position, float xDegree, float yDegree, float zDegree)
{
    float xRadians = qDegreesToRadians(xDegree);
    float yRadians = qDegreesToRadians(yDegree);
    float zRadians = qDegreesToRadians(zDegree);

    QMatrix4x4 xMat(
                1.0,            0.0,             0.0, 0.0,
                0.0, qCos(xRadians), -qSin(xRadians), 0.0,
                0.0, qSin(xRadians),  qCos(xRadians), 0.0,
                0.0,            0.0,             0.0, 1.0
                );

    QMatrix4x4 yMat(
                qCos(yRadians),  0.0, qSin(yRadians), 0.0,
                0.0,             1.0,            0.0, 0.0,
                -qSin(yRadians), 0.0, qCos(yRadians), 0.0,
                0.0,             0.0,            0.0, 1.0
                );

    QMatrix4x4 zMat(
                qCos(zRadians), -qSin(zRadians), 0.0, 0.0,
                qSin(zRadians),  qCos(zRadians), 0.0, 0.0,
                0.0,            0.0,             1.0, 0.0,
                0.0,            0.0,             0.0, 1.0
                );

    QMatrix4x4 zflip(
                1.0, 0.0,  0.0, 0.0,
                0.0, 1.0,  0.0, 0.0,
                0.0, 0.0, -1.0, 0.0,
                0.0, 0.0,  0.0, 1.0
                );

    QMatrix4x4 pos(
                1.0, 0.0, 0.0, -position.x(),
                0.0, 1.0, 0.0, -position.y(),
                0.0, 0.0, 1.0, -position.z(),
                0.0, 0.0, 0.0,          1.0
                );

    QMatrix4x4 xyzMat = yMat*xMat*zMat;
    QMatrix4x4 mat = zflip*xyzMat.inverted()*pos;
//    qDebug()<<mat;
    m_pRender->setViewMatrix(mat);
}

void GGraphicsAPI::setViewMatrix(QVector3D position, QVector3D forward, QVector3D up)
{
    // right, up, forward, (x,y,z)
    QVector3D right = QVector3D::crossProduct(up, forward).normalized();
    QVector3D up2 = QVector3D::crossProduct(forward, right).normalized();

    QMatrix4x4 view(
                right.x(), up2.x(), forward.x(), 0.0,
                right.y(), up2.y(), forward.y(), 0.0,
                right.z(), up2.z(), forward.z(), 0.0,
                      0.0,     0.0,         0.0, 1.0
                );

    QMatrix4x4 pos(
                1.0, 0.0, 0.0, -position.x(),
                0.0, 1.0, 0.0, -position.y(),
                0.0, 0.0, 1.0, -position.z(),
                0.0, 0.0, 0.0,           1.0
                );

    QMatrix4x4 zflip(
                1.0, 0.0,  0.0, 0.0,
                0.0, 1.0,  0.0, 0.0,
                0.0, 0.0, -1.0, 0.0,
                0.0, 0.0,  0.0, 1.0
                );

    QMatrix4x4 mat = zflip*view.inverted()*pos;
//    qDebug()<<mat;
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
//    qDebug()<<mat;
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

//    qDebug()<<mat;
    m_pRender->setProjMatrix(mat);
}

float* GGraphicsAPI::doRendering()
{
    return m_pRender->doRendering();
}
