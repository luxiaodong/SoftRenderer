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
