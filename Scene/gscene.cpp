#include "gscene.h"
#include "Shader/gshader.h"
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
//    obj.setMesh(GModel::loadObject(":/mesh/cube.obj"));
    obj.setMesh(GModel::loadPlane());

    obj.m_material.setShader( new GShader() );
    obj.m_material.addImage("base", ":/texture/rgb.png");

    obj.setPosition(0,0,1);
//    obj.setScale(2, 2, 2);
//    obj.setRotate(0, 180, 0);
    m_gameObjects.append(obj);
}
