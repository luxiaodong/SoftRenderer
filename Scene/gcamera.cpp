#include "gcamera.h"
#include <QtMath>
#include <QMatrix4x4>
#include <QDebug>

GCamera::GCamera()
{
    m_isOrth = false;
}

void GCamera::setViewMatrix(QVector3D position, float xDegree, float yDegree, float zDegree)
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
    m_viewMat = mat;
}

void GCamera::setViewMatrix(QVector3D position, QVector3D forward, QVector3D up)
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
    m_viewMat = mat;
}

void GCamera::setOrthMatrix(float size, float aspect, float n, float f)
{
    // z in [-1,1]
    QMatrix4x4 mat(
        1.0f/(aspect*size),      0.0f,          0.0f,          0.0f,
                0.0f,       1.0f/size,          0.0f,          0.0f,
                0.0f,            0.0f,       2/(n-f),   (n+f)/(n-f),
                0.0f,            0.0f,          0.0f,         1.0f);

    m_projMat = mat;
    m_isOrth = true;
}

void GCamera::setProjMatrix(float fov,  float aspect, float n, float f)
{
    // z in [-n,f]
    float tan = qTan( qDegreesToRadians(fov/2) );
    QMatrix4x4 mat(
        1.0f/(aspect*tan),       0.0f,          0.0f,          0.0f,
                0.0f,        1.0f/tan,          0.0f,          0.0f,
                0.0f,            0.0f,   (f+n)/(n-f),   2*n*f/(n-f),
                0.0f,            0.0f,         -1.0f,         0.0f);
    m_projMat = mat;
    m_isOrth = false;
}

void GCamera::setViewPortMatrix(float x, float y, float w, float h)
{
    //float zMax = 1;
    //float zMin = -1;
    QMatrix4x4 s(
                 w/2, 0.0f,  0.0f, 0.0f,
                0.0f,  h/2,  0.0f, 0.0f,
                0.0f, 0.0f,  0.5f, 0.0f,
                0.0f, 0.0f,  0.0f, 1.0f);

    QMatrix4x4 t(
                1.0f, 0.0f, 0.0f, x+w/2,
                0.0f, 1.0f, 0.0f, y+h/2,
                0.0f, 0.0f, 1.0f, 0.5f,
                0.0f, 0.0f, 0.0f, 1.0f);

    // 这里将z从[-1,1]映射到[0,1]
    QMatrix4x4 mat = t*s;
    m_viewPortMat = mat;
}

QPoint GCamera::ndcToScreenPoint(QVector3D pos)
{
    QVector4D p = m_viewPortMat*QVector4D(pos, 1.0f);
    return QPoint(p.x(), p.y());
}
