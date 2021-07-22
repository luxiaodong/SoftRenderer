#ifndef GMODEL_H
#define GMODEL_H

#include "Scene/ggameobject.h"
#include "Model/gmesh.h"
#include "Model/gmaterial.h"

class GModel
{
public:
    GModel();
    static GMesh loadTriangle();
    static GMesh loadPlane();
    static GMesh loadMarry();
    static GMesh loadObject(QString filePath);

//private:
//    static QString m_resourceDir;
};

#endif // GMODEL_H
