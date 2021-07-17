#include "gshader.h"
#include <QDebug>

GShader::GShader()
{

}

QVector4D GShader::worldToClipping(QVector4D pos)
{
//    qDebug()<<m_viewMat;
//    qDebug()<<m_projMat;
    return m_projMat*m_viewMat*pos;
}

void GShader::vertex(GVertexAttribute* vBuffer)
{
    if (vBuffer->m_vertexsInClips == 0)
    {
        float *vertex = new float[vBuffer->m_vertexCount*4];
        vBuffer->m_vertexsInClips = vertex;
    }

    for(int i = 0; i < vBuffer->m_vertexCount; ++i)
    {
        float x = vBuffer->m_vertexsInWorld[3*i];
        float y = vBuffer->m_vertexsInWorld[3*i + 1];
        float z = vBuffer->m_vertexsInWorld[3*i + 2];

        QVector4D pos = this->worldToClipping(QVector4D(x, y ,z, 1.0f));

//        qDebug()<<QVector3D(x,y,z);
//        qDebug()<<pos;

        vBuffer->m_vertexsInClips[4*i] = pos.x();
        vBuffer->m_vertexsInClips[4*i + 1] = pos.y();
        vBuffer->m_vertexsInClips[4*i + 2] = pos.z();
        vBuffer->m_vertexsInClips[4*i + 3] = pos.w();
    }
}

QColor GShader::fragment(float x, float y, QColor r)
{
    (void)x;
    (void)y;
    return r;
}
