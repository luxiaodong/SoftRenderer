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

    void setCamera(GCamera* camera){m_pCamera = camera;}
    void setRenderSize(const QSize& size);
    void clearColor(const QColor& color);
    void clearDepth();

    void renderGameObject(const GGameObject& );

    QList<QPoint> bresenham(int x0, int y0, int x1, int y1);
    QList<QPoint> calculateBoundary(QVector4D&,QVector4D&,QVector4D&);
    QVector3D interpolationCoffInTriangle(QPoint a, QPoint b, QPoint c, QPoint p);
    QColor interpolationColor(QColor& ca, QColor& cb, QColor& cc, QVector3D weight, float zView);
    bool isZeroArea(QPoint a, QPoint b, QPoint c);
    bool isInTriangle(QVector3D weight);
    void cullingInHomogeneousSpace(GPrimitive& primitive);
    QRect aabb(QVector4D& a, QVector4D& b, QVector4D& c);

private:
    QSize m_size;
    GFrameBuffer* m_frameBuffer;
    GDepthBuffer* m_depthBuffer;

    GCamera* m_pCamera;
    GMesh m_mesh;
    GMaterial m_material;


    GShader *m_pShader;
    QMatrix4x4 m_modelMat;

    QList<GVertexAttribute> m_vertexAttributesBeforeVertexShader;
    QList<GVertexAttribute> m_vertexAttributesAfterVertexShader;
    QList<GPrimitive> m_primitivesBeforeCulling;
    QList<GPrimitive> m_primitivesAfterCulling;

    bool m_enableDepthTest;
    bool m_enableDepthWrite;
    bool m_enableBlend;
};

#endif // GRASTER_H
