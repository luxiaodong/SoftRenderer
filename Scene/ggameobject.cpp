#include "ggameobject.h"
#include <QVector3D>

GGameObject::GGameObject()
{
    m_scale = QVector3D(1,1,1);
}

void GGameObject::setMesh(const GMesh &mesh)
{
    m_mesh = mesh;
}
