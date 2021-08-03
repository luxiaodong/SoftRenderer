#ifndef WORLD_H
#define WORLD_H

#include "Model/gmodel.h"
#include "Scene/ggameobject.h"
#include <QList>

class GScene
{
public:
    GScene();

    void loadSceneTest();
    void loadTriangle();
    void loadSphere();
    void loadPlane();
    void loadModel();

    GGameObject skybox();

public:
    QList<GGameObject> m_gameObjects;
};

#endif // WORLD_H
