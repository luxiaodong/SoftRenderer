#include "skyboxshader.h"
#include <QtMath>
#include <QDebug>

SkyboxShader::SkyboxShader()
{
}

GVertexAttribute SkyboxShader::vertex(GVertexAttribute& va)
{
    GVertexAttribute outVa;
    outVa.m_vertex = this->objectToClipping(va.m_vertex);
    outVa.m_normal = va.m_normal; // va.m_vertex.toVector3D().normalized();
    outVa.m_uv = va.m_uv;
    return outVa;
}

QColor SkyboxShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QVector3D normal = va.m_normal.normalized();
    QString key = this->mapKey(normal);
    QVector2D uv = this->uvIn6Side(key, va.m_uv);
    QImage image = map.value(key, m_white);
    return this->color(image, uv);
//    return this->color(image, va.m_uv);
}

QString SkyboxShader::mapKey(QVector3D normal)
{
    QString str = "";
    float x = qAbs(normal.x());
    float y = qAbs(normal.y());
    float z = qAbs(normal.z());

    if(x >= y && x >= z)
    {
        if(normal.x() >= 0) str = "left";
        else str = "right";
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

QVector2D SkyboxShader::uvIn6Side(QString key, QVector2D uv)
{
    float u = uv.x();
    float v = uv.y();

    //这里的计算要以uv分布为准,没有标准.
    if(key == "back")
    {
        v = 1.0f - v;
    }

    return QVector2D(u, v);
}

//https://zhuanlan.zhihu.com/p/268645600?utm_source=qq&utm_medium=social&utm_oi=910587312751153152
QVector2D SkyboxShader::uv(QString key, QVector3D normal)
{
    float u = 0.0f;
    float v = 0.0f;

    float x = normal.x();
    float y = normal.y();
    float z = normal.z();

    if(key == "left" || key == "right" )
    {
        u = z/qAbs(x);
        v = y/qAbs(x);
    }
    else if(key == "down" || key == "up")
    {
        u = x/qAbs(y);
        v = z/qAbs(y);
    }
    else if(key == "back" || key == "farward")
    {
        u = x/qAbs(z);
        v = y/qAbs(z);
    }

    u = 0.5*u+0.5;
    v = 0.5*v+0.5;

    if(key == "down" || key == "up" || key == "forward")
    {
        u = 1.0 - u;
        v = 1.0 - v;
    }

    return QVector2D(u, v);
}

