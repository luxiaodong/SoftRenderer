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
                0.0, 0.0, 0.0,           1.0
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
                0.0f,            0.0f,       2/(f-n),   (n+f)/(f-n),
                0.0f,            0.0f,          0.0f,         1.0f);

    QMatrix4x4 zflip(
                1.0, 0.0,  0.0, 0.0,
                0.0, 1.0,  0.0, 0.0,
                0.0, 0.0, -1.0, 0.0,
                0.0, 0.0,  0.0, 1.0
                );

    m_projMat = zflip*mat;
    m_isOrth = true;
}

void GCamera::setProjMatrix(float fov,  float aspect, float n, float f)
{
    // z in [-n,f]
    float tan = qTan( qDegreesToRadians(fov/2) );
    QMatrix4x4 mat(
        1.0f/(aspect*tan),       0.0f,          0.0f,          0.0f,
                0.0f,        1.0f/tan,          0.0f,          0.0f,
                0.0f,            0.0f,   (f+n)/(f-n),   2*n*f/(f-n),
                0.0f,            0.0f,         -1.0f,         0.0f);

    // 逆矩阵.
    // 1/A
    //     1/B
    //          0    -1
    //          1/D  C/D

    QMatrix4x4 zflip(
                1.0, 0.0,  0.0, 0.0,
                0.0, 1.0,  0.0, 0.0,
                0.0, 0.0, -1.0, 0.0,
                0.0, 0.0,  0.0, 1.0
                );

    m_projMat = zflip*mat;
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

void GCamera::testMatrix()
{
    QVector4D testPt(-8.66025, 4.89898, 16.3806, 16.9706);
    QVector4D posNDC = testPt/testPt.w();
qDebug()<<"view:"<<m_viewMat;
qDebug()<<"proj:"<<m_projMat;
qDebug()<<"vp  :"<<m_projMat*m_viewMat;
qDebug()<<"proj inv :"<<m_projMat.inverted();
qDebug()<<"view inv :"<<m_viewMat.inverted();
qDebug()<<testPt<<posNDC;

    //方法1.
    float a = m_projMat(2,2);
    float b = m_projMat(2,3);
    float rview = 1.0/b*posNDC.z() + a/b;
    QVector4D posCS(posNDC.x()/(rview), posNDC.y()/(rview), posNDC.z()/(rview), 1.0f/(rview));
    QVector4D posVS = m_projMat.inverted()*posCS;
qDebug()<<posCS<<posVS;

    //方法2.
    posCS = m_projMat.inverted()*posNDC;
    posVS = posCS/posCS.w();
qDebug()<<posCS<<posVS;

    //test.
    QVector4D posWS = m_viewMat.inverted()*posVS;
qDebug()<<posWS;
    QMatrix4x4 invViewMat = m_viewMat.inverted();
    a = invViewMat(1, 1);
    invViewMat(2,2) = a;
    invViewMat(1,2) = -a;
    posVS.setZ(-posVS.z());
    posWS = invViewMat*posVS;
qDebug()<<posWS;
}

