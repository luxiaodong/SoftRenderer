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
    m_isTileBased = false;
    m_tileSize = QSize(128,128);
    m_isUseHSR = false;
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

        m_tilePrimitivesAfterCulling.clear();
        m_tileFragmentAttribute.clear();
        for(int i=0; i<m_tileCount.width()*m_tileCount.height(); ++i)
        {
            QList<GPrimitive> tilePrimitives;
            m_tilePrimitivesAfterCulling.append(tilePrimitives);

            QList<GFragmentAttribute> fragAttribute;
            m_tileFragmentAttribute.append(fragAttribute);
        }
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
    m_pShader = obj.m_material->m_pShader;
    m_pShader->m_isReceiveShadow = isReceiveShadow;
    m_pShader->m_modelMat = obj.objectToWorldMatrix();
    m_pShader->m_viewMat = m_pCamera->m_viewMat;
    m_pShader->m_projMat = m_pCamera->m_projMat;
    if(m_isTileBased)
    {
        m_enableShadow = false;
        m_pShader->m_isReceiveShadow = false;
        this->tileBasedRendering(1);
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
    //????????????????????? < 0

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
    //ME(Micro Engine) ????????????, ???????????????????????????cpu

    //IA(Input Assemble) ???????????????????????????
    m_vertexAttributesBeforeVertexShader.clear();
    foreach (GVertexIndex index, m_mesh.m_indexs)
    {
        QVector3D vertex = m_mesh.m_vertexs.at( index.m_vertexIndex );

        QVector2D uv = QVector2D(0,0);
        if (m_mesh.m_uvs.size() > 0)
        {
            uv = m_mesh.m_uvs.at( index.m_uvIndex );
            float u = m_material->m_uvArgs.x() + uv.x()*m_material->m_uvArgs.width();
            float v = m_material->m_uvArgs.y() + uv.y()*m_material->m_uvArgs.height();
            uv = QVector2D(u,v);
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

    //VS(Vertex Shader) ????????????
    m_vertexAttributesAfterVertexShader.clear();
    foreach(GVertexAttribute va, m_vertexAttributesBeforeVertexShader)
    {
        m_vertexAttributesAfterVertexShader.append( m_pShader->vertex(va) );
    }

    //?????????????????????,???????????????,TCS,TE,TES
    //TCS(Tessellation Control Shaders, DX??? Hull Shader) ??????????????????
    //TE(Tessellation Engine) ?????????????????????????????????
    //TES(Tessellation Evaluation Shaders, DX???Domain Shader) ????????????????????????????????????,?????????????????????

    //GS(Geometry Shader) ????????????,?????????????????????,????????????
    //SO(Stream Out) ????????????????????????, ????????????,?????????????????????

    //PA(Primitive Assembly) ????????????,?????????????????????,????????????????????????
    m_primitivesBeforeCulling.clear();
    int count = m_vertexAttributesAfterVertexShader.size()/3;
    for(int i = 0; i < count; ++i)
    {
        GVertexAttribute v1 = m_vertexAttributesAfterVertexShader.at(3*i);
        GVertexAttribute v2 = m_vertexAttributesAfterVertexShader.at(3*i+1);
        GVertexAttribute v3 = m_vertexAttributesAfterVertexShader.at(3*i+2);
        m_primitivesBeforeCulling.append( GPrimitive(v1, v2, v3) );
    }

    //Clipping ??????(???????????????, ??????????????????????????????)
    //qDebug()<<"primitives count is "<<m_primitivesBeforeCulling.size();
    m_primitivesAfterCulling.clear();
    foreach (GPrimitive primitive, m_primitivesBeforeCulling)
    {
        //??????????????????????????????
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
    //??????????????????????????????
    foreach (GPrimitive primitive, m_primitivesAfterCulling)
    {
        //PD(perspective division) ????????????
        QList<QVector3D> ndcList = primitive.homogeneousDiv();

        //FC(face culling) ????????????
        if( primitive.isDiscardCullingSuccess(m_pShader->m_cullType) )
        {
            //qDebug()<<"discard culling";
            continue;
        }

        //Screen Mapping ????????????
        QPoint a = m_pCamera->ndcToScreenPoint(ndcList.at(0));
        QPoint b = m_pCamera->ndcToScreenPoint(ndcList.at(1));
        QPoint c = m_pCamera->ndcToScreenPoint(ndcList.at(2));

        //?????????0??????????????????
        if( this->isZeroArea(a, b, c) )
        {
            continue;
        }

//qDebug()<<primitive.m_triangle[0].m_vertex<<primitive.m_triangle[1].m_vertex<<primitive.m_triangle[2].m_vertex;
//qDebug()<<a<<b<<c;
        QList<QPoint> boundaryPair = this->calculateBoundary(a,b,c);

        //???????????????,????????????, ??????????????????????????????,????????????????????????
        int pairSize = boundaryPair.size()/2;
        for(int j=0; j<pairSize; ++j)
        {
            int xMin = boundaryPair.at(2*j).x();
            int xMax = boundaryPair.at(2*j+1).x();
            int y = boundaryPair.at(2*j).y();

            if(y >= m_size.height()) continue;

            //???????????????????????????
            xMax = qMin(xMax, m_size.width() - 1);

            //??????????????????
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

                // ???2D??????????????????3D?????????.
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

                // ??????????????????, clip?????????
                GVertexAttribute va = primitive.interpolationAttribute(weight);

                // ???????????????????????????
//                QVector4D pInView(0,0,-zView,1);
//                QVector4D pInClip = m_pCamera->m_projMat*pInView;
//                float zNdc = pInClip.z()/pInClip.w();
//                float zDepth = zNdc*0.5 + 0.5; //????????? (0-1)

                // ????????????, ??????????????????
                float zNdc = va.m_vertex.z()/va.m_vertex.w();
                float zDepth = zNdc*0.5 + 0.5; //????????? (0-1)

if(m_pShader->m_isReceiveShadow)
{
    //if(x == 307 && y == 195)
    if(x == 100 && y == 100)
    {
    }
}

                // ?????????????????????,??????ealy-z
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
                QColor srcColor = m_pShader->fragment(x*1.0f/m_size.width(), y*1.0f/m_size.height(), va, m_material->m_imageSet);

//                QColor srcColor = Qt::white;
//                if( va.m_uv.x() == nan  || va.m_uv.y() == nan )
//                {
//                    srcColor = Qt::black;
//                }

                // ????????????, ????????????
                // ZT(Z-Test) ????????????
                if(m_enableDepthTest && m_enableDepthWrite)
                {
                    m_depthBuffer->setDepth(x, y, zDepth);

                    if(m_enableShadow)
                    {
                        m_shadowMap->setDepth(x, y, 1.0f - zDepth);
                    }
                }

                // OM(Output Merger) ??????Alpha Blend???????????????
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
void GRaster::tileBasedRendering(int step)
{
    if(step == 1)
    {
        this->tileBasedRendering_front();
    }
    if(step == 2)
    {
        this->tileBasedRendering_after();
    }
}

void GRaster::tileBasedRendering_front()
{
    this->vertexProcess();
    this->tileBinning();

    for(int j=0; j<m_tileCount.height(); ++j)
    {
        for(int i=0; i<m_tileCount.width(); ++i)
        {
            int x = m_tileSize.width()*i;
            int y = m_tileSize.height()*j;
            int index = j*m_tileCount.width() + i;
            QList<GPrimitive> tilePrimitives = m_tilePrimitivesAfterCulling.at(index);
            QList<GFragmentAttribute> tileFragAttributeList = m_tileFragmentAttribute.at(index);

            if(tilePrimitives.size() > 0)
            {
                QList<GFragmentAttribute> falist = this->tileRaster(tilePrimitives, QRect(x, y, m_tileSize.width(), m_tileSize.height()) );
                tileFragAttributeList.append(falist);
                m_tileFragmentAttribute.replace(index, tileFragAttributeList);
            }
        }
    }
}

void GRaster::tileBasedRendering_after()
{
    for(int j=0; j<m_tileCount.height(); ++j)
    {
        for(int i=0; i<m_tileCount.width(); ++i)
        {
            int index = j*m_tileCount.width() + i;
            QList<GFragmentAttribute> tileFragAttributeList = m_tileFragmentAttribute.at(index);

            if(m_isUseHSR)
            {
                tileFragAttributeList = this->sortFragmentAttribute(tileFragAttributeList);
            }

            if(tileFragAttributeList.size() > 0)
            {
                this->loadBuffer(index);

                foreach(GFragmentAttribute tileFragAttribute, tileFragAttributeList)
                {
                    this->renderingTile(tileFragAttribute);
                }

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

            if(ii >= m_size.width() || jj >= m_size.height()) continue;

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

            if(ii >= m_size.width() || jj >= m_size.height()) continue;

            float depth = m_tileDepthBuffer->depth(i,j);
            m_depthBuffer->setDepth(ii,jj,depth);

            QColor color = m_tileFrameBuffer->pixel(i,j);
            m_frameBuffer->setPixel(ii,jj,color);
        }
    }
}

bool GRaster::isLineSegment(QPoint a, QPoint b, QPoint c, QPoint d)
{
    QVector2D ca(c.x()-a.x(), c.y()-a.y());
    QVector2D cd(c.x()-a.x(), d.y()-d.y());
    QVector2D cb(c.x()-a.x(), b.y()-b.y());

    float cross1 = ca.x()*cd.y() - cd.x()*ca.y();
    float cross2 = cb.x()*cd.y() - cd.x()*cb.y();

    if(cross1*cross2) return true;
    return false;
}

// ??????????????????????????????
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

    QList<QPoint> list;
    list<<a<<b<<rect.bottomLeft()<<rect.bottomRight();
    list<<a<<c<<rect.bottomLeft()<<rect.bottomRight();
    list<<b<<c<<rect.bottomLeft()<<rect.bottomRight();
    list<<a<<b<<rect.topLeft()<<rect.topRight();
    list<<a<<c<<rect.topLeft()<<rect.topRight();
    list<<b<<c<<rect.topLeft()<<rect.topRight();
    list<<a<<b<<rect.bottomLeft()<<rect.topLeft();
    list<<a<<c<<rect.bottomLeft()<<rect.topLeft();
    list<<b<<c<<rect.bottomLeft()<<rect.topLeft();
    list<<a<<b<<rect.bottomRight()<<rect.topRight();
    list<<a<<c<<rect.bottomRight()<<rect.topRight();
    list<<b<<c<<rect.bottomRight()<<rect.topRight();

    for(int i=0; i<list.size(); i+=4)
    {
        QPoint a = list.at(i);
        QPoint b = list.at(i+1);
        QPoint c = list.at(i+2);
        QPoint d = list.at(i+3);

        if(this->isLineSegment(a,b,c,d)) return true;
    }

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
            float u = m_material->m_uvArgs.x() + uv.x()*m_material->m_uvArgs.width();
            float v = m_material->m_uvArgs.y() + uv.y()*m_material->m_uvArgs.height();
            uv = QVector2D(u,v);
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

    //VS(Vertex Shader) ????????????
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

    //Clipping ??????(???????????????, ??????????????????????????????)
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

// ???tile??????
void GRaster::tileBinning()
{
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

QList<GFragmentAttribute> GRaster::tileRaster(const QList<GPrimitive> &primitivesList, QRect rect)
{
    QList<GFragmentAttribute> fragAttributeList;
    //qDebug()<<"tile primitives count is "<<primitivesList.size();
    foreach (GPrimitive primitive, primitivesList)
    {
        //PD(perspective division) ????????????
        QList<QVector3D> ndcList = primitive.homogeneousDiv();
        //Screen Mapping ????????????
        QPoint a = m_pCamera->ndcToScreenPoint(ndcList.at(0));
        QPoint b = m_pCamera->ndcToScreenPoint(ndcList.at(1));
        QPoint c = m_pCamera->ndcToScreenPoint(ndcList.at(2));

        QList<QPoint> boundaryPair = this->calculateBoundary(a,b,c);

        //???????????????,????????????,??????????????????????????????,????????????????????????
        int pairSize = boundaryPair.size()/2;

        for(int j=0; j<pairSize; ++j)
        {
            int xMin = boundaryPair.at(2*j).x();
            int xMax = boundaryPair.at(2*j+1).x();
            int y = boundaryPair.at(2*j).y();

            if(y > rect.bottom()) continue;
            if(y < rect.top()) continue;

            //??????????????????
            for(int x=xMin; x<=xMax; ++x)
            {
                if(x > rect.right()) continue;
                if(x < rect.left()) continue;

                QVector3D weight = this->interpolationCoffInTriangle(a,b,c,QPoint(x,y));
                if(this->isInTriangle(weight) == false)
                {
                    continue;
                }

                // ???2D??????????????????3D?????????
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

                // ??????????????????, clip?????????
                GVertexAttribute va = primitive.interpolationAttribute(weight);

                // ????????????, ??????????????????
                float zNdc = va.m_vertex.z()/va.m_vertex.w();
                float zDepth = zNdc*0.5 + 0.5; //????????? (0-1)

                GFragmentAttribute fa;
                fa.m_pt = QPoint(x,y);
                fa.m_depth = zDepth;
                fa.m_va = va;
                fa.m_pShader = m_pShader;
                fa.m_material = m_material;
                fragAttributeList.append(fa);
            }
        }
    }

    return fragAttributeList;
}

void GRaster::renderingTile(const GFragmentAttribute &tileFragAttribute)
{
    int x = tileFragAttribute.m_pt.x();
    int y = tileFragAttribute.m_pt.y();
    float zDepth = tileFragAttribute.m_depth;
    GShader* shader = tileFragAttribute.m_pShader;
    GMaterial* material = tileFragAttribute.m_material;
    GVertexAttribute va = tileFragAttribute.m_va;

    // ?????????????????????,??????ealy-z
    if (m_enableDepthTest && zDepth >= m_tileDepthBuffer->depth(x%m_tileSize.width(), y%m_tileSize.height()) )
    {
        return;
    }

    QColor srcColor = shader->fragment(x*1.0f/m_tileSize.width(), y*1.0f/m_tileSize.height(), va, material->m_imageSet);

    // ????????????, ????????????
    // ZT(Z-Test) ????????????
    if(m_enableDepthTest && m_enableDepthWrite)
    {
        m_tileDepthBuffer->setDepth(x%m_tileSize.width(), y%m_tileSize.height(), zDepth);
    }

    // OM(Output Merger) ??????Alpha Blend???????????????
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

QList<GFragmentAttribute> GRaster::sortFragmentAttribute(const QList<GFragmentAttribute> &tileFragAttributeList)
{
    QList<GFragmentAttribute> faList;
    foreach(GFragmentAttribute one, tileFragAttributeList)
    {
        int findIndex = -1;
        for(int i = 0; i < faList.size(); ++i)
        {
            GFragmentAttribute other = faList.at(i);
            if(one.m_pt == other.m_pt)
            {
                findIndex = i;
            }
        }

        if(findIndex == -1)
        {
            faList.append(one);
        }
        else
        {
            GFragmentAttribute other = faList.at(findIndex);
            if( one.m_depth < other.m_depth )
            {
                faList.replace(findIndex, one);
            }
        }
    }
    return faList;
}
