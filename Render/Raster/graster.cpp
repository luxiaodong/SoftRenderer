#include "graster.h"
#include <QDebug>

GRaster::GRaster()
{
    m_frameBuffer = 0;
    m_depthBuffer = 0;
}

void GRaster::createBuffer()
{
    m_frameBuffer = new GFrameBuffer(m_size.width(), m_size.height(), 3);
    m_depthBuffer = new GDepthBuffer(m_size.width(), m_size.height());
}

void GRaster::setVertexAttribute(int attId, GVertexAttribute* vBuffer)
{
    if (m_vertexs.contains(attId))
    {
        GVertexAttribute* p = m_vertexs.value(attId);
        delete []p;
        m_vertexs.remove(attId);
    }

    m_vertexs.insert(attId, vBuffer);
}

void GRaster::vertexToPrimitive(GVertexAttribute* vBuffer)
{
    if (vBuffer->m_vertexCount%3 > 0)
    {
        qDebug()<<"vertex count is not 3n.";
        return ;
    }

    for(int i = 0; i < vBuffer->m_vertexCount; i+=3)
    {
        float x = vBuffer->m_vertexsInClips[4*i];
        float y = vBuffer->m_vertexsInClips[4*i + 1];
        float z = vBuffer->m_vertexsInClips[4*i + 2];
        float w = vBuffer->m_vertexsInClips[4*i + 3];
        QVector4D a(x, y, z, w);

        x = vBuffer->m_vertexsInClips[4*(i+1)];
        y = vBuffer->m_vertexsInClips[4*(i+1) + 1];
        z = vBuffer->m_vertexsInClips[4*(i+1) + 2];
        w = vBuffer->m_vertexsInClips[4*(i+1) + 3];
        QVector4D b(x, y, z, w);

        x = vBuffer->m_vertexsInClips[4*(i+2)];
        y = vBuffer->m_vertexsInClips[4*(i+2) + 1];
        z = vBuffer->m_vertexsInClips[4*(i+2) + 2];
        w = vBuffer->m_vertexsInClips[4*(i+2) + 3];
        QVector4D c(x, y, z, w);

        GPrimitive primitive;
        primitive.setTriangle(a, b, c);
        m_primitivesBeforeCulling.append(primitive);
    }
}

void GRaster::cullingInHomogeneousSpace(GPrimitive& primitive)
{
    QList<QVector4D> list = primitive.culling();

    for(int i = 0; i < list.size() - 2; ++i)
    {
        QVector4D a = list.at(0);
        QVector4D b = list.at(i+1);
        QVector4D c = list.at(i+2);

        GPrimitive primitive;
        primitive.setTriangle(a, b, c);
        m_primitivesBeforeCulling.append(primitive);
    }
}

QVector3D GRaster::ndcToScreenPoint(QVector4D& pos)
{
    QVector4D p = m_viewPortMat*pos;
    return QVector3D(p.x(), p.y(), p.z());
}

float* GRaster::doRendering()
{
    //ME(Micro Engine) 配置管线, 比如渲染结果回传给cpu
    m_frameBuffer->clearColor(m_color.redF(), m_color.greenF(), m_color.blueF());

    //IA(Input Assemble) 获取索引和顶点数据
    m_primitivesBeforeCulling.clear();

    //VS(Vertex Shader) 顶点处理
    QMap<int, GVertexAttribute*>::const_iterator i = m_vertexs.constBegin();
    while( i != m_vertexs.constEnd())
    {
        m_shader.vertex(i.value());

        //曲面细分着色器,分为三部分,TCS,TE,TES
        //TCS(Tessellation Control Shaders, DX叫 Hull Shader) 控制细分等级
        //TE(Tessellation Engine) 将复杂曲面转换为点线面
        //TES(Tessellation Evaluation Shaders, DX叫Domain Shader) 细分后的顶点运行该着色器,重新转化为顶点

        //GS(Geometry Shader) 输入图元,选择性使用顶点,输出图元
        //SO(Stream Out) 允许数据写回内存, 比如毛发,水流的物理计算

        //PA(Primitive Assembly) 图元装配,顶点组装成图元,三个顶点归为一组
        this->vertexToPrimitive(i.value());
        ++i;
    }

    //Clipping 剪裁(视锥体剪裁, 将屏幕外的三角形丢掉)
    m_primitivesAfterCulling.clear();
    foreach (GPrimitive primitive, m_primitivesBeforeCulling)
    {
        //三个点都在视锥体里面
        if(primitive.isTriangleInFrustum())
        {
            m_primitivesAfterCulling.append(primitive);
        }
        else
        {
            this->cullingInHomogeneousSpace(primitive);
        }
    }

//    qDebug()<<m_primitivesAfterCulling.size();
    foreach (GPrimitive primitive, m_primitivesAfterCulling)
    {
        qDebug()<<primitive.m_b;
        //PD(perspective division) 透视除法
        primitive.homogeneousDiv();

        //FC(face culling) 背面剔除
        if(primitive.isDiscardCullingSuccess())
        {
            continue;
        }

        qDebug()<<primitive.m_b;

        //Screen Mapping 屏幕映射
        QVector3D a = this->ndcToScreenPoint(primitive.m_a);
        QVector3D b = this->ndcToScreenPoint(primitive.m_b);
        QVector3D c = this->ndcToScreenPoint(primitive.m_c);

        qDebug()<<a<<b<<c;
        qDebug()<<"=======================================";
    }

    //建立三角形,然后遍历, 从三个顶点建立三条线,再遍历每条扫描线
    //PZ(Pre-Z)
    //FS(Fragment Shader)
    //ZT(Z-Test) 模版测试,深度测试
    //OM(Output Merger) 进行Alpha Blend，颜色混合

    return m_frameBuffer->m_data;
}
