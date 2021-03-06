#include "gshader.h"
#include <QPoint>
#include <QVector2D>
#include <QImage>
#include <QDebug>

GShader::GShader(QColor color)
{
    m_white = QImage(1,1,QImage::Format_RGBA8888);
    m_white.fill(color);
    m_cullType = 1;
    m_isReceiveShadow = false;
}

QVector4D GShader::objectToClipping(QVector4D pos)
{
//qDebug()<<"model:"<<m_modelMat;
//qDebug()<<"view:"<<m_viewMat;
//qDebug()<<"proj:"<<m_projMat;
//qDebug()<<"view inv :"<<m_viewMat.inverted();
//qDebug()<<"proj inv :"<<m_projMat.inverted();
    QMatrix4x4 mat(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, -1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, -0.5f, 0.5f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

    QMatrix4x4 matGl = m_projMat*m_viewMat;
    QMatrix4x4 matDx = mat*m_projMat*m_viewMat;
//qDebug()<<"matGl : "<<matGl;
//qDebug()<<"matDx : "<<matDx;
//qDebug()<<matDx*m_modelMat*pos;

//    qDebug()<<"OS :"<<pos;
//    qDebug()<<"WS :"<<m_modelMat*pos;
//    qDebug()<<"VS :"<<m_viewMat*m_modelMat*pos;
//    qDebug()<<"CS :"<<m_projMat*m_viewMat*m_modelMat*pos;

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
    QMatrix4x4 mat = m_modelMat.inverted().transposed();
    QVector3D outPos = (mat*QVector4D(pos,0)).toVector3D().normalized();
    return outPos;
}

GVertexAttribute GShader::vertex(GVertexAttribute& va)
{
    GVertexAttribute outVa;
    outVa.m_vertex = this->objectToClipping(va.m_vertex);
    outVa.m_normal = this->objectToWorldDir(va.m_normal).normalized();
    outVa.m_uv = va.m_uv;
//qDebug()<<va.m_vertex<<"--->"<<objectToWorld(va.m_vertex)<<"--->"<<(outVa.m_vertex/outVa.m_vertex.w());
    return outVa;
}

float GShader::shadowValue(QVector4D posInClip, QVector3D normalInWorld)
{
    bool isPCF = true;
    float value = 0.0;
    float depthInCamera = depthInLightCamera(posInClip, normalInWorld);
    if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint(0,0)) ) value += 1.0;

    if(isPCF)
    {
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint(-1,-1)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint(-1, 0)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint(-1, 1)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint( 0,-1)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint( 0, 1)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint( 1,-1)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint( 1, 0)) ) value += 1.0;
        if( depthInCamera > 1.0f - depthInShadowMap(posInClip, QPoint( 1, 1)) ) value += 1.0;
        value = value/9.0f;
    }

    return value;
}

//float3 ApplyShadowBias(float3 positionWS, float3 normalWS, float3 lightDirection)
//{
//    float invNdotL = 1.0 - saturate(dot(lightDirection, normalWS));
//    float scale = invNdotL * _ShadowBias.y;
//    positionWS = lightDirection * _ShadowBias.xxx + positionWS;
//    positionWS = normalWS * scale.xxx + positionWS;
//    return positionWS;
//}

float GShader::depthInLightCamera(QVector4D posInClip, QVector3D normalInWorld)
{
    QMatrix4x4 currentVP = m_projMat*m_viewMat;
    QMatrix4x4 lightVP = m_light->m_projMat*m_light->m_viewMat;
    QVector4D worldPos =  currentVP.inverted()*posInClip;

    //??????????????????????????????????????????
    float invNdotL = 1.0f - qMax( QVector3D::dotProduct( m_light->m_lightDir, normalInWorld ), 0.0f);
    float shadowBias = 1.0;
    float scale = invNdotL*shadowBias;
    worldPos = worldPos + m_light->m_lightDir*scale;

    //????????????????????????????????????
    float shadowNormalBias = 1.0;
    worldPos = worldPos + normalInWorld * shadowNormalBias;

    QVector4D clipPos = lightVP*worldPos;
    float zNdc = clipPos.z()/clipPos.w();
    float zDepth = zNdc*0.5 + 0.5; //????????? (0-1)
    return zDepth;
}

float GShader::depthInShadowMap(QVector4D posInClip, QPoint offset)
{
    QMatrix4x4 currentVP = m_projMat*m_viewMat;
    QMatrix4x4 lightVP = m_light->m_projMat*m_light->m_viewMat;
    QVector4D worldPos =  currentVP.inverted()*posInClip;
    QVector4D clipPos = lightVP*worldPos;
    QVector4D p = m_light->m_viewPortMat*QVector4D(clipPos.toVector3D(), 1.0f);
    //?????????????????????
    int x = p.x() + offset.x();
    int y = p.y() + offset.y();
    return m_shadowMap->depth(x,y);
}

QColor GShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QImage image = map.value("base", m_white);
    QColor color = this->color(image, va.m_uv);
    if(m_isReceiveShadow == false) return color;

    float p = this->shadowValue(va.m_vertex, va.m_normal);
    QColor shadowColor = Qt::gray;

    float r = (1.0f - p)*color.redF() + p*shadowColor.redF();
    float g = (1.0f - p)*color.greenF() + p*shadowColor.greenF();
    float b = (1.0f - p)*color.blueF() + p*shadowColor.blueF();
    float a = (1.0f - p)*color.alphaF() + p*shadowColor.alphaF();
    return QColor(r*255, g*255, b*255, a*255);
}

QColor GShader::color(QImage& image, QVector2D uv)
{
    QPoint pt = QPoint( uv.x()*image.width(), uv.y()*image.height() );

    //????????????,1???????????????????????????image.width()-1

    int x = pt.x()%image.width();
    int y = pt.y()%image.height();

    return image.pixelColor(x, y);
}
