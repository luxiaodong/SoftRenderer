#include "gscene.h"

#include <QDir>
#include <QDebug>
#include "Shader/gshader.h"
#include "Shader/phongshader.h"
#include "Shader/skyboxshader.h"

GScene::GScene()
{
    m_gameObjects.clear();
}

void GScene::loadSceneTest()
{
//    GGameObject obj1 = GGameObject();
//    obj1.setMesh(GModel::loadPlane());
//    obj1.m_material.setShader( new GShader() );
//    obj1.m_material.addImage("base", ":/texture/uv.tga");
//    obj1.setPosition(0,0,-1);
//    m_gameObjects.append(obj1);

//    GGameObject obj2 = GGameObject();
//    obj2.setMesh(GModel::loadObject(":/mesh/marry.obj"));
//    obj2.m_material.setShader( new PhongShader() );
//    obj2.m_material.addImage("base", ":/texture/marry.png");
//    obj2.setPosition(0,0,-2);
//    obj2.setScale(2, 2, 2);
//    obj2.setRotate(0, 180, 0);
//    m_gameObjects.append(obj2);
}

void GScene::loadTriangle()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadTriangle());
    obj.m_material.setShader( new GShader() );
    obj.setPosition(0,0,1);
    m_gameObjects.append(obj);
}

void GScene::loadSphere()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadObject(":/mesh/sphere.obj"));
    obj.m_material.setShader( new GShader() );
    obj.m_material.addImage("base", ":/texture/skybox.tga");
    obj.setPosition(0,9,-9);
    m_gameObjects.append(obj);
}

GGameObject GScene::skybox()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadCube(true));
    obj.setMesh(GModel::loadUnityCube());
    obj.m_material.setShader( new SkyboxShader() );
//    obj.m_material.addImage("left",    ":/texture/cubmap/negx.jpg");
//    obj.m_material.addImage("down",    ":/texture/cubmap/negy.jpg");
//    obj.m_material.addImage("back",    ":/texture/cubmap/negz.jpg");
//    obj.m_material.addImage("right",   ":/texture/cubmap/posx.jpg");
//    obj.m_material.addImage("up",      ":/texture/cubmap/posy.jpg");
//    obj.m_material.addImage("forward", ":/texture/cubmap/posz.jpg");

//    obj.m_material.addImage("left",    ":/texture/uv.tga");
//    obj.m_material.addImage("down",    ":/texture/uv.tga");
//    obj.m_material.addImage("back",    ":/texture/uv.tga");
//    obj.m_material.addImage("right",   ":/texture/uv.tga");
//    obj.m_material.addImage("up",      ":/texture/uv.tga");
//    obj.m_material.addImage("forward", ":/texture/uv.tga");

    obj.m_material.addImage("left",    ":/texture/cubmap/negx.jpg");
    obj.m_material.addImage("down",    ":/texture/cubmap/negy.jpg");
    obj.m_material.addImage("back",    ":/texture/cubmap/negz.jpg");
    obj.m_material.addImage("right",   ":/texture/cubmap/posx.jpg");
    obj.m_material.addImage("up",      ":/texture/cubmap/posy.jpg");
    obj.m_material.addImage("forward", ":/texture/uv.tga");

    float camearFar = 1000/2;
//    obj.setPosition(0, 10, -10);
    obj.setRotate(90,0,60);
    obj.setScale(camearFar,camearFar,camearFar);
    return obj;
}
