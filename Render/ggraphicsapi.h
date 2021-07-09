#ifndef GGRAPHICSAPI_H
#define GGRAPHICSAPI_H

#include <QImage>
#include <QVector3D>
#include "Render/grender.h"

class GGraphicsAPI
{
public:
    GGraphicsAPI();

    void setRenderSize(const QSize& size);
    void setClearColor(const QColor& color);

    void setViewMatrix(QVector3D position, float xDegree = 0, float yDegree = 0, float zDegree = 0);
    void setViewMatrix(QVector3D position, QVector3D forward, QVector3D up = QVector3D(0.0f, 1.0f, 0.0f) );
    void setOrthMatrix(float size, float aspect, float n, float f); // aspect = width/height
    void setProjMatrix(float fov,  float aspect, float n, float f);

    float* doRendering();

    void setScene(const GScene& scene){m_scene = scene;}
    void setCamear(const GCamear& camear){m_camera = camear;}

private:
    GRender* m_pRender;
    GScene m_scene;
    GCamear m_camera;
};

#endif // GGRAPHICSAPI_H
