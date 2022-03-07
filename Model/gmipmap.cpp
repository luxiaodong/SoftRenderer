#include "gmipmap.h"
#include <QPainter>
#include <QtDebug>
#include "Math/gmath.h"
#include <QColor>

GMipmap::GMipmap()
{

}

GMipmap::GMipmap(QString filePath)
{
    m_source = QImage(filePath, "png");
    this->generate();
}

void GMipmap::test()
{
    QRect r = areaByLod(1,2);
    QImage black(QSize(2,2), QImage::Format_RGBA8888);
    black.fill(Qt::black);
    QPainter painter(&m_target);
    painter.drawImage(r, black);
}

// 纹理像素和屏幕像素的比值, 即多少个纹理像素占用一个屏幕像素
QColor GMipmap::color(float u, float v, float ratioX, float ratioY)
{
    float ratio = qMax(ratioX, ratioY);
    if(ratio > 1) //走mipmap
    {
        float level = this->switchLod(ratio);
        return this->color(u,v, level);
    }

    //走插值
    return this->color(u, v);
}

//根据距离选择哪一层lod
float GMipmap::switchLod(float ratio)
{
    ratio = qMax(ratio, 1.0f);
    return qLn(ratio)/qLn(2);
}

void GMipmap::generate()
{
    QSize s = m_source.size();
    m_target = QImage(s*2, QImage::Format_RGBA8888);
    m_target.fill(Qt::black);

    QPainter painter(&m_target);
    m_level = 0;
    QPoint pt = QPoint(0,0);

    while(true)
    {
        painter.drawImage(pt.x(), pt.y(), m_source.scaled(s.width(), s.height()));

        if(s.width() == 1 || s.height() == 1)
        {
            break;
        }

        pt += QPoint(s.width(), s.height());
        s = s/2;
        m_level += 1;
    }

    s = m_source.size();
    pt = QPoint(0,0);
    for(int i = 0; i < m_level; ++i)
    {
        int x = 0;
        int y = 0;
        int w = s.width();
        int h = s.height();

        for(int j = i; j < m_level; ++j)
        {
            x += w;
            w = w/2;
            painter.drawImage(pt.x() + x, pt.y(), m_source.scaled(w, s.height()));

            y += h;
            h = h/2;
            painter.drawImage(pt.x(), pt.y() + y, m_source.scaled(s.width(), h));
        }

        pt += QPoint(s.width(), s.height());
        s = s/2;
    }
}

// 根据xy的lod值获取在target矩形区域内的值
QRect GMipmap::areaByLod(int lodX, int lodY)
{
    int x = 0;
    int y = 0;
    int w = m_source.size().width();
    int h = m_source.size().height();

    for(int i = 0; i < m_level; ++i)
    {
        if(i < lodX)
        {
            x += w;
            w = w/2;
        }

        if(i < lodY)
        {
            y += h;
            h = h/2;
        }
    }

    return QRect(x,y,w,h);
}

QColor GMipmap::color(float u, float v)
{
    float x = u * m_source.width();
    float y = v * m_source.height();
    int x0 = static_cast<int>(x-0.5f);
    int x1 = static_cast<int>(x+0.5f);
    int y0 = static_cast<int>(y-0.5f);
    int y1 = static_cast<int>(y+0.5f);

    QVector3D x0y0 = GMath::toVector(m_source.pixelColor(x0, y0));
    QVector3D x1y0 = GMath::toVector(m_source.pixelColor(x1, y0));
    QVector3D x0y1 = GMath::toVector(m_source.pixelColor(x0, y1));
    QVector3D x1y1 = GMath::toVector(m_source.pixelColor(x1, y1));

    float wx0 = x - (x0 + 0.5f);
    float wx1 = (x1 + 0.5f) - x;
    float wy0 = y - (y0 + 0.5f);
    float wy1 = (y1 + 0.5f) - y;

    QVector3D x01y0 = x0y0*wx1 + x1y0*wx0;
    QVector3D x01y1 = x0y1*wx1 + x1y1*wx0;

    return GMath::toColor( x01y0*wy1 + x01y1*wy0 );
}

QColor GMipmap::color(float u, float v, float level)
{
    int l0 = static_cast<int>(level);
    int l1 = l0+1;

    QRect r0 = this->areaByLod(l0, l0);
    QRect r1 = this->areaByLod(l1, l1);

    QPoint pt0(r0.x() + static_cast<int>(r0.width()*u + 0.5f), r0.y() + static_cast<int>(r0.height()*v + 0.5f));
    QPoint pt1(r1.x() + static_cast<int>(r1.width()*u + 0.5f), r1.y() + static_cast<int>(r1.height()*v + 0.5f));

    QVector3D v0 = GMath::toVector(m_source.pixelColor(pt0));
    QVector3D v1 = GMath::toVector(m_source.pixelColor(pt1));

    float p = level - l0;
    return GMath::toColor( (1-p)*v0 + p*v1 );
}
