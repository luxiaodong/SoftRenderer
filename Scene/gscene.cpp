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
    // load triangle
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadTriangle());
    m_gameObjects.append(obj);
}
