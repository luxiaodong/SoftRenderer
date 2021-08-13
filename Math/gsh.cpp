#include "gsh.h"
#include <QtMath>
#include <QQuaternion>
#include <QGenericMatrix>

GSH::GSH()
{
}

QImage GSH::testImage(QImage& image)
{
    QList<QVector3D> coffs = calculateCoff(image);
    return rebuildImage(coffs, image.width(), image.height());
}

QVector3D GSH::toNormal(float phi, float theta)
{
    return QVector3D( qSin(theta)*qCos(phi), qSin(theta)*qSin(phi), qCos(theta));
}

QImage GSH::rebuildImage(QList<QVector3D>& coffs, int width, int height)
{
    QImage image = QImage(width, height, QImage::Format_RGBA8888);
    image.fill(Qt::black);

    float imageW = image.width();
    float imageH = image.height();

    for(int h = 0; h < imageH; ++h)
    {
        float theta = G_PI*(h+0.5f)/imageH;
        for(int w = 0; w < imageW; ++w)
        {
            float phi = 2.0f*G_PI*(w+0.5f)/imageW;
            QVector3D cv = evalSH(coffs, toNormal(phi, theta));
            QColor c(cv.x()*255, cv.y()*255, cv.z()*255);
            image.setPixelColor(w, h, c);
        }
    }

    return image;
}

QList<QVector3D> GSH::calculateCoff(QImage& image)
{
    int order = SH_MAX_ORDER;
    QList<QVector3D> coffs;
    for(int i = 0; i<order*order; ++i)
    {
        coffs.append(QVector3D(0,0,0));
    }

    float imageW = image.width();
    float imageH = image.height();

    //参数取值范围是 phi(0,pi), theta(-pi,pi), 所以面积是2*pi*pi.
    float pixel_per_area = 2.0f*G_PI*G_PI/(imageW*imageH);

    for(int h = 0; h < imageH; ++h)
    {
        float theta = G_PI*(h+0.5f)/imageH;
        float weight = pixel_per_area*qSin(theta);

        for(int w = 0; w < imageW; ++w)
        {
            float phi = 2.0f*G_PI*(w+0.5f)/imageW;
            QColor c = image.pixel(w, h);
            QVector3D cv(c.redF(), c.greenF(), c.blueF() );

            for(int m = 0; m < order; ++m)
            {
                for(int l = 0; l < 2*m + 1; ++l)
                {
                    float shBase = this->baseSH(m, l, toNormal(phi, theta) );
                    int index = indexSH(m, l);
                    QVector3D coff = coffs.at(index);
                    coff += shBase*weight*cv;
                    coffs.replace(index, coff);
                }
            }
        }
    }

    return coffs;
}

int GSH::indexSH(int m, int l)
{
    return m*m + l;
}

QVector3D GSH::evalSH(QList<QVector3D>& coffs, QVector3D n)
{
    GSHRotate shRotate;
    QList<QVector3D> rotated_coffs = shRotate.getCoff(n);

    QVector3D sum(0.0f,0.0f,0.0f);
    for(int i = 0; i < coffs.size(); ++i)
    {
        sum += rotated_coffs.at(i)*coffs.at(i);
    }

    return sum;
}

float GSH::baseSH(int m, int l, QVector3D n)
{
    if(m == 0)
    {
        if(l == 0) return baseSH_00(n);
    }
    else if(m == 1)
    {
        if(l == -1) return baseSH_10(n);
        if(l ==  0) return baseSH_11(n);
        if(l ==  1) return baseSH_12(n);
    }
    else if(m == 2)
    {
        if(l == -2) return baseSH_20(n);
        if(l == -1) return baseSH_21(n);
        if(l ==  0) return baseSH_22(n);
        if(l ==  1) return baseSH_23(n);
        if(l ==  2) return baseSH_24(n);
    }
    else if(m == 3)
    {
        if(l == -3) return baseSH_30(n);
        if(l == -2) return baseSH_31(n);
        if(l == -1) return baseSH_32(n);
        if(l ==  0) return baseSH_33(n);
        if(l ==  1) return baseSH_34(n);
        if(l ==  2) return baseSH_35(n);
        if(l ==  3) return baseSH_36(n);
    }
    else if(m == 4)
    {
        if(l == -4) return baseSH_40(n);
        if(l == -3) return baseSH_41(n);
        if(l == -2) return baseSH_42(n);
        if(l == -1) return baseSH_43(n);
        if(l ==  0) return baseSH_44(n);
        if(l ==  1) return baseSH_45(n);
        if(l ==  2) return baseSH_46(n);
        if(l ==  3) return baseSH_47(n);
        if(l ==  4) return baseSH_48(n);
    }

    return 0.0f;
}

//0阶
float GSH::baseSH_00(QVector3D& n)
{
    // 0.5 * sqrt(1/pi)
    return 0.282095f;
}

//1阶
float GSH::baseSH_10(QVector3D& n)
{
    // -sqrt(3/(4pi)) * y
    return -0.488603 * n.y();
}

float GSH::baseSH_11(QVector3D& n)
{
    // sqrt(3/(4pi)) * z
    return 0.488603 * n.z();
}

float GSH::baseSH_12(QVector3D& n)
{
    // -sqrt(3/(4pi)) * x
    return -0.488603 * n.x();
}

//2阶
float GSH::baseSH_20(QVector3D& n)
{
    // 0.5 * sqrt(15/pi) * x * y
    return 1.092548 * n.x() * n.y();
}

float GSH::baseSH_21(QVector3D& n)
{
    // -0.5 * sqrt(15/pi) * y * z
    return -1.092548 * n.y() * n.z();
}

float GSH::baseSH_22(QVector3D& n)
{
    // 0.25 * sqrt(5/pi) * (3z^2 - 1)
    return 0.315392 * ( 3.0 * n.z() * n.z() - 1.0f );
}

float GSH::baseSH_23(QVector3D& n)
{
    // -0.5 * sqrt(15/pi) * x * z
    return -1.092548 * n.x() * n.z();
}

float GSH::baseSH_24(QVector3D& n)
{
    // 0.25 * sqrt(15/pi) * (x^2 - y^2)
    return 0.546274 * (n.x() * n.x() - n.y() * n.y());
}


//3阶
float GSH::baseSH_30(QVector3D& n)
{
    // -0.25 * sqrt(35/(2pi)) * y * (3x^2 - y^2)
    return -0.590044 * n.y() * (3.0 * n.x() * n.x() - n.y() * n.y());
}

float GSH::baseSH_31(QVector3D& n)
{
    // 0.5 * sqrt(105/pi) * x * y * z
    return 2.890611 * n.x() * n.y() * n.z();
}

float GSH::baseSH_32(QVector3D& n)
{
    // -0.25 * sqrt(21/(2pi)) * y * (5z^2-1)
    return -0.457046 * n.y() * (5.0 * n.z() * n.z() - 1.0f);
}

float GSH::baseSH_33(QVector3D& n)
{
    // 0.25 * sqrt(7/pi) * z * (5z^2 - 3)
    return 0.373176 * n.z() * (5.0 * n.z() * n.z() - 3.0f);
}

float GSH::baseSH_34(QVector3D& n)
{
    // -0.25 * sqrt(21/(2pi)) * x * (5z^2-1)
    return -0.457046 * n.x() * (5.0 * n.z() * n.z() - 1.0f);
}

float GSH::baseSH_35(QVector3D& n)
{
    // 0.25 * sqrt(105/pi) * z * (x^2 - y^2)
    return 1.445306 * n.z() * (n.x() * n.x() - n.y() * n.y());
}

float GSH::baseSH_36(QVector3D& n)
{
    // -0.25 * sqrt(35/(2pi)) * x * (x^2-3y^2)
    return -0.590044 * n.x() * (n.x() * n.x() - 3.0 * n.y() * n.y());
}

//4阶
float GSH::baseSH_40(QVector3D& n)
{
    // 0.75 * sqrt(35/pi) * x * y * (x^2-y^2)
    return 2.503343 * n.x() * n.y() * (n.x() * n.x() - n.y() * n.y());
}

float GSH::baseSH_41(QVector3D& n)
{
    // -0.75 * sqrt(35/(2pi)) * y * z * (3x^2-y^2)
    return -1.770131 * n.y() * n.z() * (3.0 * n.x() * n.x() - n.y() * n.y());
}

float GSH::baseSH_42(QVector3D& n)
{
    // 0.75 * sqrt(5/pi) * x * y * (7z^2-1)
    return 0.946175 * n.x() * n.y() * (7.0 * n.z() * n.z() - 1.0);
}

float GSH::baseSH_43(QVector3D& n)
{
    // -0.75 * sqrt(5/(2pi)) * y * z * (7z^2-3)
    return -0.669047 * n.y() * n.z() * (7.0 * n.z() * n.z() - 3.0);
}

float GSH::baseSH_44(QVector3D& n)
{
    // 3/16 * sqrt(1/pi) * (35z^4-30z^2+3)
    double z2 = n.z() * n.z();
    return 0.105786 * (35.0 * z2 * z2 - 30.0 * z2 + 3.0);
}

float GSH::baseSH_45(QVector3D& n)
{
    // -0.75 * sqrt(5/(2pi)) * x * z * (7z^2-3)
    return -0.669047 * n.x() * n.z() * (7.0 * n.z() * n.z() - 3.0);
}

float GSH::baseSH_46(QVector3D& n)
{
    // 3/8 * sqrt(5/pi) * (x^2 - y^2) * (7z^2 - 1)
    return 0.473087 * (n.x() * n.x() - n.y() * n.y()) * (7.0 * n.z() * n.z() - 1.0);
}

float GSH::baseSH_47(QVector3D& n)
{
    // -0.75 * sqrt(35/(2pi)) * x * z * (x^2 - 3y^2)
    return -1.770131 * n.x() * n.z() * (n.x() * n.x() - 3.0 * n.y() * n.y());
}

float GSH::baseSH_48(QVector3D& n)
{
    // 3/16*sqrt(35/pi) * (x^2 * (x^2 - 3y^2) - y^2 * (3x^2 - y^2))
    double x2 = n.x() * n.x();
    double y2 = n.y() * n.y();
    return 0.625836 * (x2 * (x2 - 3.0 * y2) - y2 * (3.0 * x2 - y2));
}

