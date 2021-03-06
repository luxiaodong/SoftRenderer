#ifndef GSHADER_H
#define GSHADER_H

#include <QColor>
#include <QVector4D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QImage>
#include <QMap>
#include <QString>
#include <QPoint>
#include "Scene/glight.h"
#include "Raster/gvertexattribute.h"
#include "Raster/gdepthbuffer.h"

class GShader
{
public:
    GShader(QColor color=Qt::white);

    virtual GVertexAttribute vertex(GVertexAttribute& va);

    void tessellator(){}
    void geometry(){}
    virtual QColor fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map);

    QVector3D objectToWorldDir(QVector3D pos);
    QVector4D objectToWorld(QVector4D pos);
    QVector4D objectToView(QVector4D pos);
    QVector4D objectToClipping(QVector4D pos);
    QColor color(QImage& image, QVector2D uv);

public:
    float shadowValue(QVector4D posInClip, QVector3D normalInWorld);
    float depthInLightCamera(QVector4D posInClip, QVector3D normalInWorld);
    float depthInShadowMap(QVector4D posInClip, QPoint offset);

public:
    int m_cullType; //0,双面, 1,顺时针, 2.逆时针
    QMatrix4x4 m_modelMat; //模型矩阵
    QMatrix4x4 m_viewMat; //视口矩阵
    QMatrix4x4 m_projMat; //投影矩阵,包括正交投影

    //阴影
    GLight* m_light;
    GDepthBuffer* m_shadowMap;
    bool m_isReceiveShadow;

    QImage m_white;
};

#endif // GSHADER_H
