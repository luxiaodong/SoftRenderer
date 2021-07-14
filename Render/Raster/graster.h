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

class GRaster
{
public:
    GRaster();

    void createBuffer();
    float* doRendering();

    void setRenderSize(const QSize& size){m_size = size;}
    void setClearColor(const QColor& color){m_color = color;}

    void setViewMatrix(const QMatrix4x4& mat){ m_shader.m_viewMat = mat;}
    void setProjMatrix(const QMatrix4x4& mat){ m_shader.m_projMat = mat;}
    void setViewPortMatrix(const QMatrix4x4 &mat){m_viewPortMat = mat;}

    QVector3D ndcToScreenPoint(QVector4D&);

    void clearColor();
    void setVertexAttribute(int attId, GVertexAttribute*);
    void vertexToPrimitive(GVertexAttribute*);
    bool isTriangleInFrustum(QVector4D a, QVector4D b, QVector4D c);
    bool isPointInFrustum(QVector4D p);
    void cullingInHomogeneousSpace(GPrimitive& primitive);

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
