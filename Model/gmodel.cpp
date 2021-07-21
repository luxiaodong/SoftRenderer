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
//    list.append(QVector3D(0.0, 0.0, 0.0));
//    list.append(QVector3D(1.0, 0.0, 0.0));
//    list.append(QVector3D(0.0, 1.0, 0.0));

// 有问题的.
    list.append(QVector3D(1.0, -1.0, 0.0));
    list.append(QVector3D(1.0,  1.0, 0.0));
    list.append(QVector3D(-1.0, -1.0, 0.0));

    GMesh mesh;
    mesh.setPoints(list);
    return mesh;
}

GMesh GModel::loadPlane()
{
    QList<QVector3D> list;

    list.append(QVector3D(5.0, 0.0, 5.0));
    list.append(QVector3D(-5.0, 0.0, -5.0));
    list.append(QVector3D(5.0, 0.0, -5.0));

    list.append(QVector3D(-5.0, 0.0, 5.0));
    list.append(QVector3D(-5.0, 0.0, -5.0));
    list.append(QVector3D(5.0, 0.0, 5.0));

//    list.append(QVector3D(1.0,  0.0, 0.0));
//    list.append(QVector3D(-1.0, 0.0, 0.0));

    GMesh mesh;
    mesh.setPoints(list);
    return mesh;
}

GMesh GModel::loadObject(QString filePath)
{

}
