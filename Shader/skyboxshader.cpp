#include "skyboxshader.h"
#include <QtMath>
#include <QDebug>

SkyboxShader::SkyboxShader()
{
    m_cameraPos = QVector3D(0, 10, -10);
}

GVertexAttribute SkyboxShader::vertex(GVertexAttribute& va)
{
    GVertexAttribute outVa;
    outVa.m_vertex = this->objectToClipping(va.m_vertex);
//    QVector3D posW =  this->objectToWorld(va.m_vertex).toVector3D();
//    QVector3D normal = (posW - m_cameraPos).normalized();

//    QVector3D posW = this->objectToView(va.m_vertex).toVector3D();
    QVector3D normal = va.m_vertex.toVector3D().normalized();

    outVa.m_normal =  normal;
    outVa.m_uv = va.m_uv;
    return outVa;
}

QColor SkyboxShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QString key = this->mapKey(va.m_normal);
    QVector2D uv = this->uv(key, va.m_normal);
    QImage image = map.value(key, m_white);
//    return this->color(image, uv);
    return this->color(image, va.m_uv);
}

QString SkyboxShader::mapKey(QVector3D normal)
{
    QString str = "";
    float x = qAbs(normal.x());
    float y = qAbs(normal.y());
    float z = qAbs(normal.z());

    if(x >= y && x >= z)
    {
        if(normal.x() >= 0) str = "right";
        else str = "left";
    }
    else if(y >= x && y >= z)
    {
        if(normal.y() >= 0) str = "up";
        else str = "down";
    }
    else if(z >= x && z >= y)
    {
        if(normal.z() >= 0) str = "forward";
        else str = "back";
    }

    return str;
}

QVector2D SkyboxShader::uv(QString key, QVector3D normal)
{
    float u = 0.0f;
    float v = 0.0f;
    if(key == "forward" || key == "back" )
    {
        u = normal.x()/normal.z();
        v = normal.y()/normal.z();
    }
    else if(key == "up" || key == "down" )
    {
        u = normal.x()/normal.y();
        v = normal.z()/normal.y();
    }
    else if(key == "left" || key == "right")
    {
        u = normal.z()/normal.x();
        v = normal.y()/normal.x();
    }

    return QVector2D(0.5*u+0.5, 0.5*v+0.5);
}

