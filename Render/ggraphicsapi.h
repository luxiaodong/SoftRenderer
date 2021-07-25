#ifndef GGRAPHICSAPI_H
#define GGRAPHICSAPI_H

#include <QImage>
#include <QVector3D>
#include <QQuaternion>
#include "Render/Raster/graster.h"
#include "Model/gmesh.h"
#include "Model/gmaterial.h"
#include "Scene/gscene.h"
#include "Scene/gcamera.h"

class GGraphicsAPI
{
public:
    GGraphicsAPI();

    void setRenderSize(const QSize& size);
    void setClearColor(const QColor& color);
    void setClearDepth();

    void setModelMatrix(QVector3D position, QVector3D rotate, QVector3D scale);
    void setViewMatrix(QVector3D position, float xDegree = 0, float yDegree = 0, float zDegree = 0);
    void setViewMatrix(QVector3D position, QVector3D forward, QVector3D up = QVector3D(0.0f, 1.0f, 0.0f) );
    void setOrthMatrix(float size, float aspect, float n, float f); // aspect = width/height
    void setProjMatrix(float fov,  float aspect, float n, float f);
    void setViewPortMatrix(float x, float y, float w, float h);

    void setVertexAttribute(const GMesh& mesh, const GMaterial& material);

    int* doRendering();

    void setScene(const GScene& scene){m_scene = scene;}
    void setCamera(const GCamera& camera){m_pRender->m_camera = camera;}

private:
    GRaster* m_pRender;
    GScene m_scene;
};

#endif // GGRAPHICSAPI_H
