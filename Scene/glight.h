#ifndef GLIGHT_H
#define GLIGHT_H

//#include "Scene/ggameobject.h"
#include <QVector3D>
#include <QMatrix4x4>

class GLight
{
public:
    GLight();

public:
    QVector3D m_lightDir;
    float m_intensity;

    //以灯光为相机时候的矩阵
    QMatrix4x4 m_viewMat;  //视口矩阵
    QMatrix4x4 m_projMat;  //投影矩阵,包括正交投影
    QMatrix4x4 m_viewPortMat; //转屏幕的矩阵
};

#endif // GLIGHT_H
