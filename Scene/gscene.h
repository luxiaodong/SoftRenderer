#ifndef WORLD_H
#define WORLD_H

#include "Model/gmodel.h"
#include "Render/ggameobject.h"
#include <QList>

class GScene
{
public:
    GScene();

    void loadSceneTest();

public:
    QList<GGameObject> m_gameObjects;
};

#endif // WORLD_H
