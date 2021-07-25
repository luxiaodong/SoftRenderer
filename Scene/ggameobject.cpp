#include "ggameobject.h"
#include <QVector3D>
#include "Shader/gshader.h"
#include <QMatrix4x4>

GGameObject::GGameObject()
{
    m_position = QVector3D(0,0,0);
    m_rotate = QVector3D(0,0,0);
    m_scale = QVector3D(1,1,1);

    m_material.setShader( new GShader() );
}

void GGameObject::setMesh(const GMesh &mesh)
{
    m_mesh = mesh;
}

void GGameObject::setMaterial(const GMaterial &material)
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
