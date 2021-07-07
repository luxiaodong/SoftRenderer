#ifndef GGAMEOBJECT_H
#define GGAMEOBJECT_H

#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include "Model/gmesh.h"

class GGameObject
{
public:
    GGameObject();
    void setMesh(const GMesh &);

private:
    QVector3D m_position;
    QQuaternion m_rotate;
    QVector3D m_scale;

    GMesh m_mesh;
};

#endif // GGAMEOBJECT_H
