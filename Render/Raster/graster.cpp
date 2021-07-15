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

QRect GRaster::aabb(QVector3D& a, QVector3D& b, QVector3D& c)
{
    int xMin = a.x();
    int xMax = a.x();
    int yMin = a.y();
    int yMax = a.y();

    if (b.x() < xMin) xMin = b.x();
    if (b.x() > xMax) xMax = b.x();
    if (b.y() < yMin) yMin = b.y();
    if (b.y() > yMax) yMax = b.y();

    if (c.x() < xMin) xMin = c.x();
    if (c.x() > xMax) xMax = c.x();
    if (c.y() < yMin) yMin = c.y();
    if (c.y() > yMax) yMax = c.y();

    return QRect(xMin, yMin, xMax-xMin, yMax-xMin);
}

QList<QPoint> GRaster::bresenham(int x0, int y0, int x1, int y1)
{
    QList<QPoint> list;

    int dx = qAbs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = qAbs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;)
    {
        list.append(QPoint(x0,y0));
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }

    return list;
}

QList<QPoint> GRaster::calculateBoundary(QVector3D& a,QVector3D& b,QVector3D& c)
{
    QList<QPoint> boundaryPoints;
    boundaryPoints.append( this->bresenham(a.x(), a.y(), b.x(), b.y()));
    boundaryPoints.append( this->bresenham(b.x(), b.y(), c.x(), c.y()));
    boundaryPoints.append( this->bresenham(c.x(), c.y(), a.x(), a.y()));
    QRect aabb = this->aabb(a,b,c);

    QList<QPoint> boundaryPair;
    for(int j = 0; j <= aabb.height(); j++)
    {
        int h = aabb.y() + j;
        boundaryPair.append(QPoint(aabb.x() + aabb.width(), h));
        boundaryPair.append(QPoint(aabb.x(), h));
    }

    foreach(QPoint pt, boundaryPoints)
    {
        int j = pt.y() - aabb.y();
        int x = pt.x();
        int xMin = boundaryPair.at(2*j).x();
        if(x < xMin) boundaryPair.replace(2*j, QPoint(x, pt.y()));

        int xMax = boundaryPair.at(2*j+1).x();
        if(x > xMax) boundaryPair.replace(2*j+1, QPoint(x, pt.y()));
    }

    return boundaryPair;
}

QVector3D GRaster::interpolationCoffInTriangle(QPoint a, QPoint b, QPoint c, QPoint p)
{
    //p = alpha*a + beta*b + gamma*c
    //1 = alpha + beta + gamma
    // 这里的系数可能 < 0

    int temp1 = (a.y() - b.y())*p.x() + (b.x() - a.x())*p.y() + a.x()*b.y() - b.x()*a.y();
    int temp2 = (a.y() - b.y())*c.x() + (b.x() - a.x())*c.y() + a.x()*b.y() - b.x()*a.y();
    int temp3 = (a.y() - c.y())*p.x() + (c.x() - a.x())*p.y() + a.x()*c.y() - c.x()*a.y();
    int temp4 = (a.y() - c.y())*b.x() + (c.x() - a.x())*b.y() + a.x()*c.y() - c.x()*a.y();

    float gamma = temp1*1.0f/temp2;
    float beta = temp3*1.0f/temp4;
    float alpha = 1.0f - beta - gamma;
    return QVector3D(alpha, beta, gamma);
}

QColor GRaster::interpolationColor(QColor& ca, QColor& cb, QColor& cc, QVector3D coff)
{
    int r = ca.red()*1.0f*coff.x() + cb.red()*1.0*coff.y() + cc.red()*1.0*coff.z();
    int g = ca.green()*1.0f*coff.x() + cb.green()*1.0*coff.y() + cc.green()*1.0*coff.z();
    int b = ca.blue()*1.0f*coff.x() + cb.blue()*1.0*coff.y() + cc.blue()*1.0*coff.z();
    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    if(r <  0 ) r = 0;
    if(g <  0 ) g = 0;
    if(b <  0 ) b = 0;
    return QColor(r, g, b);
}

int* GRaster::doRendering()
{
    //ME(Micro Engine) 配置管线, 比如渲染结果回传给cpu
    m_frameBuffer->clearColor(m_color);

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
//        qDebug()<<primitive.m_b;
        //PD(perspective division) 透视除法
        primitive.homogeneousDiv();

        //FC(face culling) 背面剔除
        if(primitive.isDiscardCullingSuccess())
        {
            continue;
        }

//        qDebug()<<primitive.m_b;

        //Screen Mapping 屏幕映射
        QVector3D a = this->ndcToScreenPoint(primitive.m_a);
        QVector3D b = this->ndcToScreenPoint(primitive.m_b);
        QVector3D c = this->ndcToScreenPoint(primitive.m_c);
        QList<QPoint> boundaryPair = this->calculateBoundary(a,b,c);
        QColor ca = primitive.m_colorA;
        QColor cb = primitive.m_colorB;
        QColor cc = primitive.m_colorC;

        int pairSize = boundaryPair.size()/2;
        for(int j=0; j<pairSize; ++j)
        {
            int xMin = boundaryPair.at(2*j).x();
            int xMax = boundaryPair.at(2*j+1).x();
            int y = boundaryPair.at(2*j).y();

            //从左到右扫描
            for(int x=xMin; x<=xMax; ++x)
            {
                QVector3D coff = this->interpolationCoffInTriangle( QPoint(a.x(), a.y()),
                                                                    QPoint(b.x(), b.y()),
                                                                    QPoint(c.x(), c.y()),
                                                                    QPoint(x,y) );
                //注,z的插值
                QColor color = this->interpolationColor(ca, cb, cc, coff);
                m_frameBuffer->setPixel(x, y, color);
            }
        }

//        QMap<int, int> range;
//        qDebug()<<line1.size()<<line2.size()<<line3.size();
//        qDebug()<<line1;
//        qDebug()<<line2;
//        qDebug()<<line3;
//        qDebug()<<c<<a;

//        qDebug()<<a<<b<<c;
//        qDebug()<<"=======================================";
    }

    //建立三角形,然后遍历, 从三个顶点建立三条线,再遍历每条扫描线
    //PZ(Pre-Z)
    //FS(Fragment Shader)
    //ZT(Z-Test) 模版测试,深度测试
    //OM(Output Merger) 进行Alpha Blend，颜色混合

    return m_frameBuffer->m_data;
}
