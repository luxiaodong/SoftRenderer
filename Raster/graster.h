#ifndef GRASTER_H
#define GRASTER_H

#include "Raster/gframebuffer.h"
#include "Raster/gdepthbuffer.h"
#include "Raster/gvertexattribute.h"
#include "Raster/gfragmentattribute.h"
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
#include <QVector2D>
#include <QMap>
#include <QSize>
#include <QColor>
#include <QRect>
#include <QPoint>

class GRaster
{
public:
    GRaster();
    int* frameBuffer(){return m_frameBuffer->m_data;}
    float* shadowMap(){return m_shadowMap->m_data;}

    void setCamera(GCamera* camera){m_pCamera = camera;}
    void setRenderSize(const QSize& size);
    void clearColor(const QColor& color);
    void clearDepth();
    void clearShadowMap();

    void renderGameObject(const GGameObject&, bool isReceiveShadow = false);

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

public:
    void immediateRendering();
    void tileBasedRendering(int step);
    bool isTileBased(){return m_isTileBased;}

private:
    void tileBasedRendering_front();
    void tileBasedRendering_after();
    void vertexProcess();
    void tileBinning();
    QList<GFragmentAttribute> tileRaster(const QList<GPrimitive> &primitivesList, QRect rect);
    void renderingTile(const GFragmentAttribute &tileFragAttribute);

    bool isLineSegment(QPoint a, QPoint b, QPoint c, QPoint d);
    bool isIntersectInTile(QPoint a, QPoint b, QPoint c, QRect rect);
    void loadBuffer(int index);
    void saveBuffer(int index);
    QList<GFragmentAttribute> sortFragmentAttribute(const QList<GFragmentAttribute> &tileFragAttributeList);

private:
    QSize m_size;
    GFrameBuffer* m_frameBuffer;
    GDepthBuffer* m_depthBuffer;
    GDepthBuffer* m_shadowMap;

    GLight* m_light;
    GCamera* m_pCamera;
    GMesh m_mesh;
    GMaterial* m_material;
    GShader *m_pShader;

    QList<GVertexAttribute> m_vertexAttributesBeforeVertexShader;
    QList<GVertexAttribute> m_vertexAttributesAfterVertexShader;
    QList<GPrimitive> m_primitivesBeforeCulling;
    QList<GPrimitive> m_primitivesAfterCulling;

    bool m_enableDepthTest;
    bool m_enableDepthWrite;
    bool m_enableBlend;
    bool m_enableShadow;

    // tile based rendering, 仅单个物体的tilebase, 没有处理物体间的
    bool m_isTileBased;
    QSize m_tileSize;
    QSize m_tileCount;
    GFrameBuffer* m_tileFrameBuffer;
    GDepthBuffer* m_tileDepthBuffer;
    GDepthBuffer* m_tileShadowMap;
    QList< QList<GPrimitive> > m_tilePrimitivesAfterCulling;
    QList< QList<GFragmentAttribute> > m_tileFragmentAttribute;
    bool m_isUseHSR; //隐藏面消除.

};

#endif // GRASTER_H
