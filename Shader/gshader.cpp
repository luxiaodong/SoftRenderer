#include "gshader.h"
#include <QPoint>
#include <QVector2D>
#include <QImage>
#include <QDebug>

GShader::GShader()
{
    m_white = QImage(1,1,QImage::Format_RGBA8888);
    m_white.fill(Qt::white);
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

QColor GShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QImage image = map.value("base", m_white);
    return this->color(image, va.m_uv);
}

QColor GShader::color(QImage& image, QVector2D uv)
{
    QPoint pt = QPoint( uv.x()*(image.width()-1), uv.y()*(image.height()-1) );
    return image.pixelColor(pt.x(), pt.y());
}
