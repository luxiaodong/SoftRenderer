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

    // aspect = width/height
    void setOrthMatrix(float size, float aspect, float n, float f);
    void setProjMatrix(float fov,  float aspect, float n, float f);
    void setViewMatrix(QVector3D forward, QVector3D up = QVector3D(0.0f, 1.0f, 0.0f) );

    float* doRendering();


    void setScene(const GScene& scene){m_scene = scene;}
    void setCamear(const GCamear& camear){m_camera = camear;}

private:
    GRender* m_pRender;
    GScene m_scene;
    GCamear m_camera;
};

#endif // GGRAPHICSAPI_H
