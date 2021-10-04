#include "ggameobject.h"
#include <QVector3D>
#include "Shader/gshader.h"
#include <QMatrix4x4>
#include <QtMath>

GGameObject::GGameObject()
{
    m_castShadow = false;
    m_receiveShadow = false;
    m_position = QVector3D(0,0,0);
    m_rotate = QVector3D(0,0,0);
    m_scale = QVector3D(1,1,1);

    m_material = new GMaterial();
}

void GGameObject::setMesh(const GMesh &mesh)
{
    m_mesh = mesh;
}

void GGameObject::setMaterial(GMaterial* material)
{
    m_material = material;
}

void GGameObject::setPosition(float x, float y, float z)
{
    m_position = QVector3D(x, y, z);
}

void GGameObject::setRotate(float rx, float ry, float rz)
{
    m_rotate = QVector3D(rx, ry, rz);
}

void GGameObject::setScale(float sx, float sy, float sz)
{
    m_scale = QVector3D(sx, sy, sz);
}

QMatrix4x4 GGameObject::objectToWorldMatrix() const
{
    float xRadians = qDegreesToRadians(m_rotate.x());
    float yRadians = qDegreesToRadians(m_rotate.y());
    float zRadians = qDegreesToRadians(m_rotate.z());

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


    QMatrix4x4 s(
                m_scale.x(),    0.0f,           0.0f, 0.0f,
                0.0f,    m_scale.y(),           0.0f, 0.0f,
                0.0f,           0.0f,    m_scale.z(), 0.0f,
                0.0f,           0.0f,           0.0f, 1.0f
                );

    QMatrix4x4 r = yMat*xMat*zMat;

    QMatrix4x4 t(
                1.0f, 0.0f, 0.0f, m_position.x(),
                0.0f, 1.0f, 0.0f, m_position.y(),
                0.0f, 0.0f, 1.0f, m_position.z(),
                0.0f, 0.0f, 0.0f, 1.0f
                );

    return t*r*s;
}
