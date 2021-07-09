#ifndef GRASTER_H
#define GRASTER_H

#include "Render/Raster/gframebuffer.h"
#include "Render/Raster/gdepthbuffer.h"
#include "Render/Raster/gvertexattributebuffer.h"
#include "Shader/gshader.h"
#include <QMatrix4x4>
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

    void clearColor();
    void setVertexAttribute(int attId, GVertexAttributeBuffer*);

private:
    QSize m_size;
    QColor m_color;
    GShader m_shader;

    GFrameBuffer* m_frameBuffer;
    GDepthBuffer* m_depthBuffer;
    QMap<int, GVertexAttributeBuffer*> m_vertexs;
};

#endif // GRASTER_H
