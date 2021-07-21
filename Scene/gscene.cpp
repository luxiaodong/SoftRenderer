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
//    obj.setMesh(GModel::loadTriangle());
    obj.setMesh(GModel::loadPlane());
//    obj.setPosition(0,0,1);
//    obj.setScale(0.5, 0.5, 1);
//    obj.setRotate(60, 0, 0);
    m_gameObjects.append(obj);
}
