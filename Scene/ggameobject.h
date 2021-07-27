#ifndef GGAMEOBJECT_H
#define GGAMEOBJECT_H

#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include "Model/gmesh.h"
#include "Model/gmaterial.h"

class GGameObject
{
public:
    GGameObject();
    void setMesh(const GMesh &);
    void setMaterial(const GMaterial &);
    void setScale(float sx, float sy, float sz);
    void setRotate(float rx, float ry, float rz);
    void setPosition(float x, float y, float z);

    QMatrix4x4 objectToWorldMatrix() const;

public:
    QVector3D m_position;
    QVector3D m_rotate;
    QVector3D m_scale;
    GGameObject* m_parent;

    GMesh m_mesh;
    GMaterial m_material;
};

#endif // GGAMEOBJECT_H
