#include "gscene.h"

#include <QDir>
#include <QDebug>
#include "Shader/gshader.h"
#include "Shader/phongshader.h"
#include "Shader/skyboxshader.h"

GScene::GScene()
{
    m_enableShadow = false;
    m_gameObjects.clear();
    m_light = new GLight();
    m_lightCamera = NULL;
}

void GScene::setLight()
{
//    m_light->setRotate(50, -120, 0);
    m_light->m_intensity = 1.0f;
//    qDebug()<<m_light->dir();
}

GCamera* GScene::getLightCamera()
{
    if(m_lightCamera == NULL)
    {
        m_lightCamera = new GCamera();
//        m_lightCamera->setViewMatrix(QVector3D(0, 0, 0), 50, -120, 0);
//        QMatrix4x4 matUnity(
//                    -0.50000,	0.00000,	0.86603,	-9.61336,
//                    -0.66341,	0.64279,	-0.38302,	11.35071,
//                    0.55667,	0.76604,	0.32139,	-30.61082,
//                    0.00000,	0.00000,	0.00000,	1.00000);
//        qDebug()<<m_lightCamera->m_viewMat.inverted()*matUnity;

        //位置不知道怎么计算,先抄unity.
        m_lightCamera->setViewMatrix(QVector3D(19.7637f, 16.1532f, 22.5111f), 50, -120, 0);
        //近平面,远平面,大小不知道怎么计算,先抄unity.
        m_lightCamera->setOrthMatrix(29.6471f, 1, -23.431, 68.186f);
        //阴影大小用屏幕大小,避免两次申请内存不一致.
        m_lightCamera->m_viewPortMat = m_camera->m_viewPortMat;

        //在渲染物体的时候还要用到这些矩阵
        m_light->m_lightDir = QVector3D(0.5566704f,0.7660444f,0.3213937f);
        m_light->m_viewMat = m_lightCamera->m_viewMat;
        m_light->m_projMat = m_lightCamera->m_projMat;
        m_light->m_viewPortMat = m_lightCamera->m_viewPortMat;

//        qDebug()<<m_lightCamera->m_viewMat;
//        qDebug()<<m_lightCamera->m_projMat;
//        qDebug()<<m_lightCamera->m_projMat*m_lightCamera->m_viewMat;
    }

    return m_lightCamera;
}

void GScene::loadPlane()
{
    GGameObject obj = GGameObject();
    obj.setMesh(GModel::loadPlane());
    obj.m_material.setShader( new GShader() );
//    obj.m_material.addImage("base", ":/texture/uv.tga");
    obj.setPosition(0,0,-1);
    obj.setRotate(90, 0, 0);
    obj.setScale(10, 10, 10);
    obj.m_castShadow = true;
    obj.m_receiveShadow = true;
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
    obj.m_castShadow = true;
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
