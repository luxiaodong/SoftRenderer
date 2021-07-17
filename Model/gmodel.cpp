#include "gmodel.h"
#include <QVector3D>
#include <QList>

GModel::GModel()
{
    m_resourceDir = "/Users/luxiaodong/Project/SoftRenderer/Resource";
}

GMesh GModel::loadTriangle()
{
    QList<QVector3D> list;
    //逆时针
    list.append(QVector3D(0.5, 0.0, 1.0));
    list.append(QVector3D(0.5, 0.5, 2.0));
    list.append(QVector3D(0.0, 0.0, 3.0));

    GMesh mesh;
    mesh.setPoints(list);
    return mesh;
}
