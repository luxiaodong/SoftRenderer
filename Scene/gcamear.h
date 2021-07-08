#ifndef GCAMEAR_H
#define GCAMEAR_H

#include <QVector3D>

class GCamear
{
public:
    GCamear();

public:
    QVector3D m_position;

    float m_near;
    float m_far;

    //Projection
    float m_fov;    // 弧度
    float m_aspect; // width/height

    //Ortho
    float m_size;

private:
    bool m_isProjection;

};

#endif // GCAMEAR_H
