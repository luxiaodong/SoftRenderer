#ifndef WORLD_H
#define WORLD_H

#include "Scene/glight.h"
#include "Model/gmodel.h"
#include "Scene/ggameobject.h"
#include "Scene/gcamera.h"
#include <QList>

class GScene
{
public:
    GScene();
    void setLight();

    void loadSHScene();
    void loadTriangle();
    void loadSphere();
    void loadPlane();
    void loadModel();

    GGameObject skybox();
    GGameObject skybox2();

    GCamera* getLightCamera();

public:
    QList<GGameObject> m_gameObjects;
    GLight* m_light;
    GCamera* m_camera;
    GCamera* m_lightCamera;
    bool m_enableShadow;
};

#endif // WORLD_H
