#ifndef GRASTER_H
#define GRASTER_H

#include "Render/Raster/gframebuffer.h"
#include "Render/Raster/gdepthbuffer.h"
#include "Render/Raster/gvertexattribute.h"
#include "Render/Raster/gprimitive.h"
#include "Shader/gshader.h"
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>
#include <QMap>
#include <QSize>
#include <QColor>
#include <QRect>
#include <QPoint>

class GRaster
{
public:
    GRaster();

    void createBuffer();
    int* doRendering();

    void setRenderSize(const QSize& size){m_size = size;}
    void setClearColor(const QColor& color){m_color = color;}

    void setViewMatrix(const QMatrix4x4& mat){ m_shader.m_viewMat = mat;}
    void setProjMatrix(const QMatrix4x4& mat){ m_shader.m_projMat = mat;}
    void setViewPortMatrix(const QMatrix4x4 &mat){m_viewPortMat = mat;}

    QVector3D ndcToScreenPoint(QVector4D&);
    QList<QPoint> bresenham(int x0, int y0, int x1, int y1);
    QList<QPoint> calculateBoundary(QVector3D&,QVector3D&,QVector3D&);
    QVector3D interpolationCoffInTriangle(QPoint a, QPoint b, QPoint c, QPoint p);
    QColor interpolationColor(QColor& ca, QColor& cb, QColor& cc, QVector3D coff);

    void clearColor();
    void setVertexAttribute(int attId, GVertexAttribute*);
    void vertexToPrimitive(GVertexAttribute*);
    void cullingInHomogeneousSpace(GPrimitive& primitive);
    QRect aabb(QVector3D& a, QVector3D& b, QVector3D& c);

private:
    QSize m_size;
    QColor m_color;
    GShader m_shader;

    QMatrix4x4 m_viewPortMat;

    GFrameBuffer* m_frameBuffer;
    GDepthBuffer* m_depthBuffer;
    QMap<int, GVertexAttribute*> m_vertexs;
    QList<GPrimitive> m_primitivesBeforeCulling;
    QList<GPrimitive> m_primitivesAfterCulling;
};

#endif // GRASTER_H
