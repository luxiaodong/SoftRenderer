#include "graster.h"
#include <QDebug>
#include <QString>

GRaster::GRaster()
{
    m_frameBuffer = 0;
    m_depthBuffer = 0;
    m_enableDepthTest = true;
    m_enableDepthWrite = true;
    m_enableBlend = false;
    m_enableShadow = false;
    m_isTileBased = true;
    m_tileSize = QSize(64,64);
}

void GRaster::setRenderSize(const QSize& size)
{
    m_size = size;
    m_frameBuffer = new GFrameBuffer(m_size.width(), m_size.height(), 3);
    m_depthBuffer = new GDepthBuffer(m_size.width(), m_size.height());
    m_shadowMap = new GDepthBuffer(m_size.width(), m_size.height());

    if(m_isTileBased)
    {
        m_tileFrameBuffer = new GFrameBuffer(m_tileSize.width(), m_tileSize.height(), 3);
        m_tileDepthBuffer = new GDepthBuffer(m_tileSize.width(), m_tileSize.height());
        m_tileShadowMap = new GDepthBuffer(m_tileSize.width(), m_tileSize.height());

        int tileWidthCount = m_size.width()/m_tileSize.width();
        int tileHeightCount = m_size.height()/m_tileSize.height();
        if(tileWidthCount*m_tileSize.width() < m_size.width()) tileWidthCount++;
        if(tileHeightCount*m_tileSize.height() < m_size.height()) tileHeightCount++;
        m_tileCount = QSize(tileWidthCount, tileHeightCount);
    }
}

void GRaster::clearColor(const QColor& color)
{
    m_frameBuffer->clearColor(color);
}

void GRaster::clearDepth()
{
    m_depthBuffer->clear(1.0f);
}

void GRaster::clearShadowMap()
{
    m_shadowMap->clear(0.0f);
}

void GRaster::renderGameObject(const GGameObject& obj, bool isReceiveShadow)
{
    m_mesh = obj.m_mesh;
    m_material = obj.m_material;
    m_pShader = obj.m_material.m_pShader;
    m_pShader->m_isReceiveShadow = isReceiveShadow;
    m_pShader->m_modelMat = obj.objectToWorldMatrix();
    m_pShader->m_viewMat = m_pCamera->m_viewMat;
    m_pShader->m_projMat = m_pCamera->m_projMat;
    if(m_isTileBased)
    {
        m_enableShadow = false;
        m_pShader->m_isReceiveShadow = false;
        this->tileBasedRendering();
    }
    else
    {
        this->immediateRendering();
    }
}

void GRaster::cullingInHomogeneousSpace(GPrimitive& primitive)
{
    QList<GVertexCullingRatio> list = primitive.culling();
    QList<GVertexAttribute> vaList;
    foreach(GVertexCullingRatio single, list)
    {
        vaList.append( primitive.interpolationAttribute(single.m_ratio) );
    }

    for(int i = 0; i < vaList.size() - 2; ++i)
    {
        GVertexAttribute a = vaList.at(0);
        GVertexAttribute b = vaList.at(i+1);
        GVertexAttribute c = vaList.at(i+2);

        m_primitivesAfterCulling.append( GPrimitive(a,b,c) );
    }
}

QRect GRaster::aabb(QPoint& a, QPoint& b, QPoint& c)
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

    return QRect(xMin, yMin, xMax-xMin, yMax-yMin);
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

QList<QPoint> GRaster::calculateBoundary(QPoint& a, QPoint& b,QPoint& c)
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

bool GRaster::isZeroArea(QPoint a, QPoint b, QPoint c)
{
    int temp2 = (a.y() - b.y())*c.x() + (b.x() - a.x())*c.y() + a.x()*b.y() - b.x()*a.y();
    int temp4 = (a.y() - c.y())*b.x() + (c.x() - a.x())*b.y() + a.x()*c.y() - c.x()*a.y();
    if (temp2 == 0 || temp4 == 0)
    {
        return true;
    }

    return false;
}

QVector3D GRaster::interpolationCoffInTriangle(QPoint a, QPoint b, QPoint c, QPoint p)
{
    //p = alpha*a + beta*b + gamma*c
    //1 = alpha + beta + gamma
    //这里的系数可能 < 0

    int temp1 = (a.y() - b.y())*p.x() + (b.x() - a.x())*p.y() + a.x()*b.y() - b.x()*a.y();
    int temp2 = (a.y() - b.y())*c.x() + (b.x() - a.x())*c.y() + a.x()*b.y() - b.x()*a.y();
    int temp3 = (a.y() - c.y())*p.x() + (c.x() - a.x())*p.y() + a.x()*c.y() - c.x()*a.y();
    int temp4 = (a.y() - c.y())*b.x() + (c.x() - a.x())*b.y() + a.x()*c.y() - c.x()*a.y();

    float gamma = temp1*1.0f/temp2;
    float beta = temp3*1.0f/temp4;
    float alpha = 1.0f - gamma - beta;

    return QVector3D(alpha, beta, gamma);
}

bool GRaster::isInTriangle(QVector3D weight)
{
    if (weight.x() < -0.0001) return false;
    if (weight.y() < -0.0001) return false;
    if (weight.z() < -0.0001) return false;
    return true;
}

void GRaster::immediateRendering()
{
    //ME(Micro Engine) 配置管线, 比如渲染结果回传给cpu

    //IA(Input Assemble) 获取索引和顶点数据
    m_vertexAttributesBeforeVertexShader.clear();
    foreach (GVertexIndex index, m_mesh.m_indexs)
    {
        QVector3D vertex = m_mesh.m_vertexs.at( index.m_vertexIndex );

        QVector2D uv = QVector2D(0,0);
        if (m_mesh.m_uvs.size() > 0)
        {
            uv = m_mesh.m_uvs.at( index.m_uvIndex );
        }

        QVector3D normal = QVector3D(0,0,1);
        if (m_mesh.m_normals.size() > 0)
        {
            normal = m_mesh.m_normals.at( index.m_normalIndex );
        }

        m_vertexAttributesBeforeVertexShader.append( GVertexAttribute(vertex, uv, normal) );
    }

    if ( m_vertexAttributesBeforeVertexShader.size()%3 > 0 )
    {
        qDebug()<<"vertex count is not 3n.";
        return ;
    }

    //VS(Vertex Shader) 顶点处理
    m_vertexAttributesAfterVertexShader.clear();
    foreach(GVertexAttribute va, m_vertexAttributesBeforeVertexShader)
    {
        m_vertexAttributesAfterVertexShader.append( m_pShader->vertex(va) );
    }

    //曲面细分着色器,分为三部分,TCS,TE,TES
    //TCS(Tessellation Control Shaders, DX叫 Hull Shader) 控制细分等级
    //TE(Tessellation Engine) 将复杂曲面转换为点线面
    //TES(Tessellation Evaluation Shaders, DX叫Domain Shader) 细分后的顶点运行该着色器,重新转化为顶点

    //GS(Geometry Shader) 输入图元,选择性使用顶点,输出图元
    //SO(Stream Out) 允许数据写回内存, 比如毛发,水流的物理计算

    //PA(Primitive Assembly) 图元装配,顶点组装成图元,三个顶点归为一组
    m_primitivesBeforeCulling.clear();
    int count = m_vertexAttributesAfterVertexShader.size()/3;
    for(int i = 0; i < count; ++i)
    {
        GVertexAttribute v1 = m_vertexAttributesAfterVertexShader.at(3*i);
        GVertexAttribute v2 = m_vertexAttributesAfterVertexShader.at(3*i+1);
        GVertexAttribute v3 = m_vertexAttributesAfterVertexShader.at(3*i+2);
        m_primitivesBeforeCulling.append( GPrimitive(v1, v2, v3) );
    }

    //Clipping 剪裁(视锥体剪裁, 将屏幕外的三角形丢掉)
    //qDebug()<<"primitives count is "<<m_primitivesBeforeCulling.size();
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

    qDebug()<<"primitives count is "<<m_primitivesAfterCulling.size();
    //对每个三角形进行遍历
    foreach (GPrimitive primitive, m_primitivesAfterCulling)
    {
        //PD(perspective division) 透视除法
        QList<QVector3D> ndcList = primitive.homogeneousDiv();

        //FC(face culling) 背面剔除
        if( primitive.isDiscardCullingSuccess(m_pShader->m_cullType) )
        {
            //qDebug()<<"discard culling";
            continue;
        }

        //Screen Mapping 屏幕映射
        QPoint a = m_pCamera->ndcToScreenPoint(ndcList.at(0));
        QPoint b = m_pCamera->ndcToScreenPoint(ndcList.at(1));
        QPoint c = m_pCamera->ndcToScreenPoint(ndcList.at(2));

        //面积为0的三角形跳过
        if( this->isZeroArea(a, b, c) )
        {
            continue;
        }

//qDebug()<<primitive.m_triangle[0].m_vertex<<primitive.m_triangle[1].m_vertex<<primitive.m_triangle[2].m_vertex;
//qDebug()<<a<<b<<c;
        QList<QPoint> boundaryPair = this->calculateBoundary(a,b,c);

        //建立三角形,然后遍历, 从三个顶点建立三条线,再遍历每条扫描线
        int pairSize = boundaryPair.size()/2;
        for(int j=0; j<pairSize; ++j)
        {
            int xMin = boundaryPair.at(2*j).x();
            int xMax = boundaryPair.at(2*j+1).x();
            int y = boundaryPair.at(2*j).y();

            if(y >= m_size.height()) continue;

            //右边边界不能取整数
            xMax = qMin(xMax, m_size.width() - 1);

            //从左到右扫描
            for(int x=xMin; x<=xMax; ++x)
            {
                QVector3D weight = this->interpolationCoffInTriangle( a, b, c, QPoint(x,y) );
                if(this->isInTriangle(weight) == false)
                {
                    continue;
                }

//                QRect debugRect( 3*m_tileSize.width(), 3*m_tileSize.height(), m_tileSize.width(), m_tileSize.height() );
//                if(debugRect.contains(x, y) == false)
//                {
//                    continue;
//                }

                // 将2D的权重转化为3D的权重.
                if(m_pCamera->m_isOrth == false)
                {
                    float alpha = weight.x()/primitive.m_triangle[0].m_vertex.w();
                    float beta  = weight.y()/primitive.m_triangle[1].m_vertex.w();
                    float gamma = weight.z()/primitive.m_triangle[2].m_vertex.w();
                    float zView = 1.0f/(alpha + beta + gamma);
                    alpha = alpha*zView;
                    beta  = beta*zView;
                    gamma = gamma*zView;
                    weight = QVector3D(alpha, beta, gamma);
                }

                // 插值顶点属性, clip空间内
                GVertexAttribute va = primitive.interpolationAttribute(weight);

                // 计算插值后的深度值
//                QVector4D pInView(0,0,-zView,1);
//                QVector4D pInClip = m_pCamera->m_projMat*pInView;
//                float zNdc = pInClip.z()/pInClip.w();
//                float zDepth = zNdc*0.5 + 0.5; //再转到 (0-1)

                // 快速做法, 跳过投影矩阵
                float zNdc = va.m_vertex.z()/va.m_vertex.w();
                float zDepth = zNdc*0.5 + 0.5; //再转到 (0-1)

if(m_pShader->m_isReceiveShadow)
{
    //if(x == 307 && y == 195)
    if(x == 100 && y == 100)
    {
    }
}

                // 对于非透明物体,进行ealy-z
                if (m_enableDepthTest && zDepth >= m_depthBuffer->depth(x, y) )
                {
                    continue;
                }

                // FS(Fragment Shader)
                if(m_pShader->m_isReceiveShadow)
                {
                    m_pShader->m_light = this->m_light;
                    m_pShader->m_shadowMap = this->m_shadowMap;

//                    m_pShader->m_shadowMapVP = this->m_shadowMapVP;
//                    m_pShader->m_depthInShadowMap = m_shadowMap->depth( x*1.0f/m_size.width(), y*1.0f/m_size.height() );

if(m_pShader->m_isReceiveShadow)
{
//    if(x == 307 && y == 195)
    if(x == 100 && y == 100)
    {
    }
}
                }
                QColor srcColor = m_pShader->fragment(x*1.0f/m_size.width(), y*1.0f/m_size.height(), va, m_material.m_imageSet);

//                QColor srcColor = Qt::white;
//                if( va.m_uv.x() == nan  || va.m_uv.y() == nan )
//                {
//                    srcColor = Qt::black;
//                }

                // 模版测试, 暂不支持
                // ZT(Z-Test) 深度测试
                if(m_enableDepthTest && m_enableDepthWrite)
                {
                    m_depthBuffer->setDepth(x, y, zDepth);

                    if(m_enableShadow)
                    {
                        m_shadowMap->setDepth(x, y, 1.0f - zDepth);
                    }
                }

                // OM(Output Merger) 进行Alpha Blend，颜色混合
                if(m_enableBlend)
                {
                    QColor dstColor = m_frameBuffer->pixel(x, y);
                    float r = srcColor.redF()  *srcColor.alphaF() + dstColor.redF()  *(1 - srcColor.alphaF());
                    float g = srcColor.greenF()*srcColor.alphaF() + dstColor.greenF()*(1 - srcColor.alphaF());
                    float b = srcColor.blueF() *srcColor.alphaF() + dstColor.blueF() *(1 - srcColor.alphaF());
                    m_frameBuffer->setPixel(x, y, QColor(r*255, g*255, b*255) );
                }
                else
                {
                    m_frameBuffer->setPixel(x, y, srcColor);
                }
            }
        }
    }
}

// ============================= tile based rendering ============================
void GRaster::tileBasedRendering()
{
    this->vertexProcess();
    this->tileBinning();

    for(int j=0; j<m_tileCount.height(); ++j)
    //int j = 3;
    {
        for(int i=0; i<m_tileCount.width(); ++i)
        //int i = 3;
        {
            int x = m_tileSize.width()*i;
            int y = m_tileSize.height()*j;
            int index = j*m_tileCount.width() + i;
            QList<GPrimitive> tilePrimitives = m_tilePrimitivesAfterCulling.at(index);

            if(tilePrimitives.size() > 0)
            {
                this->loadBuffer(index);
                this->renderingTile(tilePrimitives, QRect(x, y, m_tileSize.width(), m_tileSize.height()) );
                this->saveBuffer(index);
            }
        }
    }
}

void GRaster::loadBuffer(int index)
{
    int x = index%m_tileCount.width();
    int y = index/m_tileCount.width();

    for(int j=0; j<m_tileSize.height(); ++j)
    {
        for(int i=0; i<m_tileSize.width(); ++i)
        {
            int ii = x*m_tileSize.width() + i;
            int jj = y*m_tileSize.height() + j;

            float depth = m_depthBuffer->depth(ii,jj);
            m_tileDepthBuffer->setDepth(i,j, depth);

            QColor color = m_frameBuffer->pixel(ii,jj);
            m_tileFrameBuffer->setPixel(i,j, color);
        }
    }
}

void GRaster::saveBuffer(int index)
{
    int x = index%m_tileCount.width();
    int y = index/m_tileCount.width();

    for(int j=0; j<m_tileSize.height(); ++j)
    {
        for(int i=0; i<m_tileSize.width(); ++i)
        {
            int ii = x*m_tileSize.width() + i;
            int jj = y*m_tileSize.height() + j;

            float depth = m_tileDepthBuffer->depth(i,j);
            m_depthBuffer->setDepth(ii,jj,depth);

            QColor color = m_tileFrameBuffer->pixel(i,j);
            m_frameBuffer->setPixel(ii,jj,color);
        }
    }
}

// 三角形和矩形是否相交,不知道是否正确
bool GRaster::isIntersectInTile(QPoint a, QPoint b, QPoint c, QRect rect)
{
    if(rect.contains(a)) return true;
    if(rect.contains(b)) return true;
    if(rect.contains(c)) return true;

    QVector3D weight;
    weight = this->interpolationCoffInTriangle(a,b,c, rect.bottomLeft());
    if(this->isInTriangle(weight)) return true;
    weight = this->interpolationCoffInTriangle(a,b,c, rect.bottomRight());
    if(this->isInTriangle(weight)) return true;
    weight = this->interpolationCoffInTriangle(a,b,c, rect.topLeft());
    if(this->isInTriangle(weight)) return true;
    weight = this->interpolationCoffInTriangle(a,b,c, rect.topRight());
    if(this->isInTriangle(weight)) return true;

    return false;
}

void GRaster::vertexProcess()
{
    m_vertexAttributesBeforeVertexShader.clear();
    foreach (GVertexIndex index, m_mesh.m_indexs)
    {
        QVector3D vertex = m_mesh.m_vertexs.at( index.m_vertexIndex );

        QVector2D uv = QVector2D(0,0);
        if (m_mesh.m_uvs.size() > 0)
        {
            uv = m_mesh.m_uvs.at( index.m_uvIndex );
        }

        QVector3D normal = QVector3D(0,0,1);
        if (m_mesh.m_normals.size() > 0)
        {
            normal = m_mesh.m_normals.at( index.m_normalIndex );
        }

        m_vertexAttributesBeforeVertexShader.append( GVertexAttribute(vertex, uv, normal) );
    }

    if ( m_vertexAttributesBeforeVertexShader.size()%3 > 0 )
    {
        qDebug()<<"vertex count is not 3n.";
        return ;
    }

    //VS(Vertex Shader) 顶点处理
    m_vertexAttributesAfterVertexShader.clear();
    foreach(GVertexAttribute va, m_vertexAttributesBeforeVertexShader)
    {
        m_vertexAttributesAfterVertexShader.append( m_pShader->vertex(va) );
    }

    m_primitivesBeforeCulling.clear();
    int count = m_vertexAttributesAfterVertexShader.size()/3;
    for(int i = 0; i < count; ++i)
    {
        GVertexAttribute v1 = m_vertexAttributesAfterVertexShader.at(3*i);
        GVertexAttribute v2 = m_vertexAttributesAfterVertexShader.at(3*i+1);
        GVertexAttribute v3 = m_vertexAttributesAfterVertexShader.at(3*i+2);
        m_primitivesBeforeCulling.append( GPrimitive(v1, v2, v3) );
    }

    //Clipping 剪裁(视锥体剪裁, 将屏幕外的三角形丢掉)
    //qDebug()<<"primitives count is "<<m_primitivesBeforeCulling.size();
    m_primitivesAfterCulling.clear();
    foreach (GPrimitive primitive, m_primitivesBeforeCulling)
    {
        if(primitive.isTriangleInFrustum())
        {
            m_primitivesAfterCulling.append(primitive);
        }
        else
        {
            this->cullingInHomogeneousSpace(primitive);
        }
    }
}

// 将tile打包
void GRaster::tileBinning()
{
    m_tilePrimitivesAfterCulling.clear();
    for(int i=0; i<m_tileCount.width()*m_tileCount.height(); ++i)
    {
        QList<GPrimitive> tilePrimitives;
        m_tilePrimitivesAfterCulling.append(tilePrimitives);
    }

    qDebug()<<"primitives count is "<<m_primitivesAfterCulling.size();
    foreach (GPrimitive primitive, m_primitivesAfterCulling)
    {
        QList<QVector3D> ndcList = primitive.homogeneousDiv();
        if( primitive.isDiscardCullingSuccess(m_pShader->m_cullType) )
        {
            continue;
        }

        QPoint a = m_pCamera->ndcToScreenPoint(ndcList.at(0));
        QPoint b = m_pCamera->ndcToScreenPoint(ndcList.at(1));
        QPoint c = m_pCamera->ndcToScreenPoint(ndcList.at(2));

        if( this->isZeroArea(a, b, c) )
        {
            continue;
        }

        for(int j=0; j<m_tileCount.height(); ++j)
        {
            for(int i=0; i<m_tileCount.width(); ++i)
            {
                int x = m_tileSize.width()*i;
                int y = m_tileSize.height()*j;
                int index = j*m_tileCount.width() + i;
                QList<GPrimitive> tilePrimitives = m_tilePrimitivesAfterCulling.at(index);
                if(this->isIntersectInTile(a,b,c,QRect(x, y, m_tileSize.width(), m_tileSize.height())))
                {
                    tilePrimitives.append(primitive);
                    m_tilePrimitivesAfterCulling.replace(index, tilePrimitives);
                }
            }
        }
    }
}

void GRaster::renderingTile(const QList<GPrimitive> primitivesList, QRect rect)
{
//    qDebug()<<"tile primitives count is "<<primitivesList.size();
    foreach (GPrimitive primitive, primitivesList)
    {
        //PD(perspective division) 透视除法
        QList<QVector3D> ndcList = primitive.homogeneousDiv();
        //Screen Mapping 屏幕映射
        QPoint a = m_pCamera->ndcToScreenPoint(ndcList.at(0));
        QPoint b = m_pCamera->ndcToScreenPoint(ndcList.at(1));
        QPoint c = m_pCamera->ndcToScreenPoint(ndcList.at(2));

        QList<QPoint> boundaryPair = this->calculateBoundary(a,b,c);

        //建立三角形,然后遍历,从三个顶点建立三条线,再遍历每条扫描线
        int pairSize = boundaryPair.size()/2;

        for(int j=0; j<pairSize; ++j)
        {
            int xMin = boundaryPair.at(2*j).x();
            int xMax = boundaryPair.at(2*j+1).x();
            int y = boundaryPair.at(2*j).y();

            if(y > rect.bottom()) continue;
            if(y < rect.top()) continue;

            //从左到右扫描
            for(int x=xMin; x<=xMax; ++x)
            {
                if(x > rect.right()) continue;
                if(x < rect.left()) continue;

                QVector3D weight = this->interpolationCoffInTriangle(a,b,c,QPoint(x,y));
                if(this->isInTriangle(weight) == false)
                {
                    continue;
                }

                // 将2D的权重转化为3D的权重
                if(m_pCamera->m_isOrth == false)
                {
                    float alpha = weight.x()/primitive.m_triangle[0].m_vertex.w();
                    float beta  = weight.y()/primitive.m_triangle[1].m_vertex.w();
                    float gamma = weight.z()/primitive.m_triangle[2].m_vertex.w();
                    float zView = 1.0f/(alpha + beta + gamma);
                    alpha = alpha*zView;
                    beta  = beta*zView;
                    gamma = gamma*zView;
                    weight = QVector3D(alpha, beta, gamma);
                }

                // 插值顶点属性, clip空间内
                GVertexAttribute va = primitive.interpolationAttribute(weight);

                // 快速做法, 跳过投影矩阵
                float zNdc = va.m_vertex.z()/va.m_vertex.w();
                float zDepth = zNdc*0.5 + 0.5; //再转到 (0-1)

                // 对于非透明物体,进行ealy-z
                if (m_enableDepthTest && zDepth >= m_tileDepthBuffer->depth(x%m_tileSize.width(), y%m_tileSize.height()) )
                {
                    continue;
                }

                QColor srcColor = m_pShader->fragment(x*1.0f/m_tileSize.width(), y*1.0f/m_tileSize.height(), va, m_material.m_imageSet);

                // 模版测试, 暂不支持
                // ZT(Z-Test) 深度测试
                if(m_enableDepthTest && m_enableDepthWrite)
                {
                    m_tileDepthBuffer->setDepth(x%m_tileSize.width(), y%m_tileSize.height(), zDepth);
                }

                // OM(Output Merger) 进行Alpha Blend，颜色混合
                if(m_enableBlend)
                {
                    QColor dstColor = m_tileFrameBuffer->pixel(x, y);
                    float r = srcColor.redF()  *srcColor.alphaF() + dstColor.redF()  *(1 - srcColor.alphaF());
                    float g = srcColor.greenF()*srcColor.alphaF() + dstColor.greenF()*(1 - srcColor.alphaF());
                    float b = srcColor.blueF() *srcColor.alphaF() + dstColor.blueF() *(1 - srcColor.alphaF());
                    m_tileFrameBuffer->setPixel(x%m_tileSize.width(), y%m_tileSize.height(), QColor(r*255, g*255, b*255) );
                }
                else
                {
                    m_tileFrameBuffer->setPixel(x%m_tileSize.width(), y%m_tileSize.height(), srcColor);
                }
            }
        }
    }
}
