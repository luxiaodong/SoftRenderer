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

bool GRaster::isTriangleInFrustum(QVector4D a, QVector4D b, QVector4D c)
{
    if (!this->isPointInFrustum(a)) return false;
    if (!this->isPointInFrustum(b)) return false;
    if (!this->isPointInFrustum(c)) return false;
    return true;
}

bool GRaster::isPointInFrustum(QVector4D p)
{
    if( p.x() < -p.w() || p.x() > p.w() ) return false;
    if( p.y() < -p.w() || p.y() > p.w() ) return false;
    if( p.z() < -p.w() || p.z() > p.w() ) return false;
    return true;
}

float* GRaster::doRendering()
{
    //ME(Micro Engine) 配置管线, 比如渲染结果回传给cpu
    m_frameBuffer->clearColor(m_color.redF(), m_color.greenF(), m_color.blueF());

    //IA(Input Assemble) 获取索引和顶点数据
    m_primitivesBeforeCulling.clear();
    m_primitivesAfterCulling.clear();

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
    foreach (GPrimitive primitive, m_primitivesBeforeCulling)
    {
        if(this->isTriangleInFrustum(primitive.m_a, primitive.m_b, primitive.m_c))
        {
            m_primitivesAfterCulling.append(primitive);
        }
        else
        {
            //Cohen-Sutherland算法, 找出新的三角形
        }
    }

    //
    foreach (GPrimitive primitive, m_primitivesAfterCulling)
    {
        //PD(perspective division) 透视除法
        primitive.homogeneousDiv();

        //FC(face culling) 背面剔除
        if(primitive.isDiscardCullingSuccess())
        {
            continue;
        }


    }



    //Screen Mapping 屏幕映射
    //建立三角形,然后遍历, 从三个顶点建立三条线,再遍历每条扫描线
    //PZ(Pre-Z)
    //FS(Fragment Shader)
    //ZT(Z-Test) 模版测试,深度测试
    //OM(Output Merger) 进行Alpha Blend，颜色混合

    return m_frameBuffer->m_data;
}
