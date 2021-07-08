#ifndef GRENDER_H
#define GRENDER_H

#include <QSize>
#include <QImage>
#include <QDebug>
#include <QRectF>
#include <QColor>
#include <QMatrix4x4>

#include "Scene/ggameobject.h"
#include "Scene/gcamear.h"
#include "Scene/gscene.h"

class GRender
{
public:
    GRender();

    void setRenderSize(const QSize& size){m_size = size;}
    void setClearColor(const QColor& color){m_color = color;}

    void setViewMatrix(const QMatrix4x4& mat){m_viewMat = mat;}
    void setProjMatrix(const QMatrix4x4& mat){m_projMat = mat;}

public:
    virtual float* doRendering(){return 0;}
    virtual void createBuffer(){}

protected:
    QSize m_size;
    QColor m_color;

    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影
};

#endif // GRENDER_H
