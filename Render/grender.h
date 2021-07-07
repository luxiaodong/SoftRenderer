#ifndef GRENDER_H
#define GRENDER_H

#include <QSize>
#include <QImage>
#include <QDebug>
#include <QRectF>
#include <QColor>
#include "Render/ggameobject.h"
#include "Render/gcamear.h"
#include "Scene/gscene.h"

class GRender
{
public:
    GRender();

    void setRenderSize(const QSize& size){m_size = size;}
    void setClearColor(const QColor& color){m_color = color;}

    void setScene(const GScene& scene){m_scene = scene;}
    void setCamear(const GCamear& camear){m_camera = camear;}

public:
    virtual QImage DoRendering();

protected:
    QSize m_size;
    QColor m_color;

    GScene m_scene;
    GCamear m_camera;
};

#endif // GRENDER_H
