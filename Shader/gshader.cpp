#include "gshader.h"
#include <QPoint>
#include <QVector2D>
#include <QImage>
#include <QDebug>

GShader::GShader()
{
    m_white = QImage(1,1,QImage::Format_RGBA8888);
    m_white.fill(Qt::white);
    m_cullType = 1;
    m_isReceiveShadow = false;
}

QVector4D GShader::objectToClipping(QVector4D pos)
{
//qDebug()<<"model:"<<m_modelMat;
//qDebug()<<"view:"<<m_viewMat;
//qDebug()<<"proj:"<<m_projMat;
    return m_projMat*m_viewMat*m_modelMat*pos;
}

QVector4D GShader::objectToView(QVector4D pos)
{
    return m_viewMat*m_modelMat*pos;
}

QVector4D GShader::objectToWorld(QVector4D pos)
{
    return m_modelMat*pos;
}

QVector3D GShader::objectToWorldDir(QVector3D pos)
{
    QMatrix4x4 mat = m_modelMat.transposed().inverted();
    QVector3D outPos = mat*QVector4D(pos,1).toVector3D().normalized();;
    return outPos;
}

GVertexAttribute GShader::vertex(GVertexAttribute& va)
{
    GVertexAttribute outVa;
    outVa.m_vertex = this->objectToClipping(va.m_vertex);
    outVa.m_normal = this->objectToWorldDir(va.m_normal);
    outVa.m_uv = va.m_uv;
//qDebug()<<va.m_vertex<<"--->"<<objectToWorld(va.m_vertex)<<"--->"<<(outVa.m_vertex/outVa.m_vertex.w());
    return outVa;
}

float GShader::depthInLightCamera(QVector4D posInClip)
{
    QMatrix4x4 currentVP = m_projMat*m_viewMat;
    QMatrix4x4 lightVP = m_light->m_projMat*m_light->m_viewMat;
    QVector4D worldPos =  currentVP.inverted()*posInClip;
    QVector4D clipPos = lightVP*worldPos;
    float zNdc = clipPos.z()/clipPos.w();
    float zDepth = zNdc*0.5 + 0.5; //再转到 (0-1)
    return zDepth;
}

float GShader::depthInShadowMap(QVector4D posInClip)
{
    QMatrix4x4 currentVP = m_projMat*m_viewMat;
    QMatrix4x4 lightVP = m_light->m_projMat*m_light->m_viewMat;
    QVector4D worldPos =  currentVP.inverted()*posInClip;
    QVector4D clipPos = lightVP*worldPos;
    QVector4D p = m_light->m_viewPortMat*QVector4D(clipPos.toVector3D(), 1.0f);
    int x = p.x();
    int y = p.y();
    return m_shadowMap->depth(x,y);
}

QColor GShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QImage image = map.value("base", m_white);
    QColor color = this->color(image, va.m_uv);
    if(m_isReceiveShadow == false) return color;

    bool isInShadow = false;
    float bias = 0.0005f;
    if( depthInLightCamera(va.m_vertex) - bias > 1.0f - depthInShadowMap(va.m_vertex) )
    {
        isInShadow = true;
    }

    if (isInShadow) return Qt::gray;
    return color;
}

QColor GShader::color(QImage& image, QVector2D uv)
{
    QPoint pt = QPoint( uv.x()*(image.width()-1), uv.y()*(image.height()-1) );
    return image.pixelColor(pt.x(), pt.y());
}
