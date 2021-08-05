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

void GScene::loadPlane()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadPlane());
    obj.m_material.setShader( new GShader() );
    obj.m_material.addImage("base", ":/texture/uv.tga");
    obj.setPosition(0,0,-1);
    obj.setRotate(90, 0, 0);
    obj.setScale(10, 10, 10);
    m_gameObjects.append(obj);
}

void GScene::loadModel()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadObject(":/mesh/marry.obj"));
//    obj.m_material.setShader( new PhongShader() );
    obj.m_material.setShader( new GShader() );
    obj.m_material.addImage("base", ":/texture/marry.png");
    obj.setPosition(0,0,-3);
    obj.setRotate(0, 180, 0);
    obj.setScale(2, 2, 2);
    m_gameObjects.append(obj);
}

void GScene::loadTriangle()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadTriangle());
    obj.m_material.setShader( new GShader() );
    obj.setPosition(0, 10, -10);
    obj.setRotate(0,-90,0);
    obj.setScale(100,100,100);
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
    obj.setMesh(GModel::loadUnityCube());
    obj.m_material.setShader( new SkyboxShader(true) );
    obj.m_material.addImage("skybox", ":/texture/skybox.tga");

    if(true)
    {
        obj.m_material.addImage("left",    ":/texture/cubmap/negx.jpg");
        obj.m_material.addImage("down",    ":/texture/cubmap/negy.jpg");
        obj.m_material.addImage("back",    ":/texture/cubmap/negz.jpg");
        obj.m_material.addImage("right",   ":/texture/cubmap/posx.jpg");
        obj.m_material.addImage("up",      ":/texture/cubmap/posy.jpg");
        obj.m_material.addImage("forward", ":/texture/cubmap/posz.jpg");
    }
    else
    {
        obj.m_material.addImage("left",    ":/texture/uv.tga");
        obj.m_material.addImage("down",    ":/texture/uv.tga");
        obj.m_material.addImage("back",    ":/texture/uv.tga");
        obj.m_material.addImage("right",   ":/texture/uv.tga");
        obj.m_material.addImage("up",      ":/texture/uv.tga");
        obj.m_material.addImage("forward", ":/texture/uv.tga");
    }

    float camearFar = 1000;
    obj.setPosition(0, 10, -10);
    obj.setRotate(0,-90,0);
    obj.setScale(camearFar,camearFar,camearFar);
    return obj;
}

GGameObject GScene::skybox2()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadObject(":/mesh/sphere.obj"));

    GShader* pShader = new SkyboxShader(false);
    pShader->m_cullType = 2;
    obj.m_material.setShader(pShader);
    obj.m_material.addImage("skybox", ":/texture/skybox.tga");

    float camearFar = 1000;
//    obj.setPosition(0, 10, -10);
//    obj.setRotate(0,0,0);
    obj.setScale(camearFar,camearFar,camearFar);
    return obj;
}
