#ifndef GLIGHT_H
#define GLIGHT_H

#include "Scene/ggameobject.h"
#include <QVector3D>
#include <QMatrix4x4>

class GLight : public GGameObject
{
public:
    GLight();

    QVector3D dir();

public:
    float m_intensity;
};

#endif // GLIGHT_H
