#include "gshader.h"
#include <QDebug>

GShader::GShader()
{

}

QVector3D GShader::worldToClipping(QVector3D pos)
{
    QVector4D worldPos = QVector4D(pos.x(), pos.y(), pos.z(), 1);
    QVector4D clipPos = m_projMat*m_viewMat*worldPos;
    return QVector3D(clipPos.x(), clipPos.y(), clipPos.z());
}

void GShader::vertex(GVertexAttributeBuffer* vBuffer)
{
    for(int i = 0; i < vBuffer->m_vertexCount; ++i)
    {
        float x = vBuffer->m_vertexs[3*i];
        float y = vBuffer->m_vertexs[3*i + 1];
        float z = vBuffer->m_vertexs[3*i + 2];

        QVector3D pos = this->worldToClipping(QVector3D(x,y,z));

//        qDebug()<<QVector3D(x,y,z);
//        qDebug()<<pos;

        vBuffer->m_vertexs[3*i] = pos.x();
        vBuffer->m_vertexs[3*i + 1] = pos.y();
        vBuffer->m_vertexs[3*i + 2] = pos.z();
    }
}

void GShader::fragment()
{

}
