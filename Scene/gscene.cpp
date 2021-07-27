#include "gscene.h"
#include <QDir>
#include <QDebug>

GScene::GScene()
{
//    qDebug()<<QDir::current();
//    qDebug()<<QDir::currentPath();
}

void GScene::loadSceneTest()
{
    m_gameObjects.clear();

    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadTriangle());
//    obj.setMesh(GModel::loadPlane());
//    obj.setMesh(GModel::loadMarry());
    obj.setPosition(0,0,1);
//    obj.setScale(2, 2, 2);
//    obj.setRotate(0, 180, 0);
    m_gameObjects.append(obj);
}
