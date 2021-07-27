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

//    GGameObject obj0 = GGameObject();
//    obj0.setMesh(GModel::loadTriangle());
//    obj0.m_material.setShader( new GShader() );
//    obj0.setPosition(0,0,1);
//    m_gameObjects.append(obj0);

    GGameObject obj1 = GGameObject();
    obj1.setMesh(GModel::loadPlane());
    obj1.m_material.setShader( new GShader() );
    obj1.m_material.addImage("base", ":/texture/rgb.png");
    obj1.setPosition(0,0,-1);
    m_gameObjects.append(obj1);

    GGameObject obj2 = GGameObject();
    obj2.setMesh(GModel::loadObject(":/mesh/marry.obj"));
    obj2.m_material.setShader( new GShader() );
    obj2.m_material.addImage("base", ":/texture/marry.png");
    obj2.setPosition(0,0,-2);
    obj2.setScale(2, 2, 2);
    obj2.setRotate(0, 150, 0);
    m_gameObjects.append(obj2);
}
