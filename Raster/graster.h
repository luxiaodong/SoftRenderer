#ifndef GRASTER_H
#define GRASTER_H

#include "Raster/gframebuffer.h"
#include "Raster/gdepthbuffer.h"
#include "Raster/gvertexattribute.h"
#include "Raster/gprimitive.h"
#include "Shader/gshader.h"
#include "Model/gmaterial.h"
#include "Model/gmesh.h"
#include "Scene/gcamera.h"
#include "Scene/ggameobject.h"
#include "Scene/glight.h"
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
    void doRendering();
    int* frameBuffer(){return m_frameBuffer->m_data;}
    float* shadowMap(){return m_shadowMap->m_data;}

    void setCamera(GCamera* camera){m_pCamera = camera;}
    void setRenderSize(const QSize& size);
    void clearColor(const QColor& color);
    void clearDepth();
    void clearShadowMap();

    void renderGameObject(const GGameObject&, bool isCastShadow = false, bool isReceiveShadow = false);

    QList<QPoint> bresenham(int x0, int y0, int x1, int y1);
    QList<QPoint> calculateBoundary(QPoint&,QPoint&,QPoint&);
    QVector3D interpolationCoffInTriangle(QPoint a, QPoint b, QPoint c, QPoint p);
    bool isZeroArea(QPoint a, QPoint b, QPoint c);
    bool isInTriangle(QVector3D weight);
    void cullingInHomogeneousSpace(GPrimitive& primitive);
    QRect aabb(QPoint& a, QPoint& b, QPoint& c);

public:
    void enableShadow(bool isEnable){m_enableShadow = isEnable;}
    void setLight(GLight* light){m_light = light;}

private:
    QSize m_size;
    GFrameBuffer* m_frameBuffer;
    GDepthBuffer* m_depthBuffer;
    GDepthBuffer* m_shadowMap;

    GLight* m_light;
    GCamera* m_pCamera;
    GMesh m_mesh;
    GMaterial m_material;

    GShader *m_pShader;

    QList<GVertexAttribute> m_vertexAttributesBeforeVertexShader;
    QList<GVertexAttribute> m_vertexAttributesAfterVertexShader;
    QList<GPrimitive> m_primitivesBeforeCulling;
    QList<GPrimitive> m_primitivesAfterCulling;

    bool m_enableDepthTest;
    bool m_enableDepthWrite;
    bool m_enableBlend;
    bool m_enableShadow;
};

#endif // GRASTER_H
