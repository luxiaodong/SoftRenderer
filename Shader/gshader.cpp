#include "gshader.h"
#include <QDebug>

GShader::GShader()
{

}

QVector4D GShader::objectToClipping(QVector4D pos)
{
//qDebug()<<"model:"<<m_modelMat;
//qDebug()<<"view:"<<m_viewMat;
//qDebug()<<"proj:"<<m_projMat;
    return m_projMat*m_viewMat*m_modelMat*pos;
}

GVertexAttribute GShader::vertex(GVertexAttribute& va)
{
    GVertexAttribute outVa;
    outVa.m_vertex = this->objectToClipping(va.m_vertex);
    outVa.m_normal = va.m_normal; // 法向量需要修正
    outVa.m_uv = va.m_uv;
    return outVa;
}

QColor GShader::fragment(float x, float y, GVertexAttribute& va)
{
    return Qt::white;
}
