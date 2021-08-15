#include "gsh.h"
#include "Math/gmath.h"
#include <QtMath>
#include <QQuaternion>
#include <QGenericMatrix>

GSH::GSH()
{
}

void GSH::test()
{
    //    const std::vector<double> c_red = {-1.028, 0.779, -0.275, 0.601, -0.256,1.891, -1.658, -0.370, -0.772};
    //    const std::vector<double> c_green = {-0.591, -0.713, 0.191, 1.206, -0.587,-0.051, 1.543, -0.818, 1.482};
    //    const std::vector<double> c_blue = {-1.119, 0.559, 0.433, -0.680, -1.815, -0.915, 1.345, 1.572, -0.622};

//    this->testEvalColor();
//    this->testCalculateShCoffs();
}

void GSH::testCalculateShCoffs()
{
    int imageW = 32;
    int imageH = 16;
    QImage originImage = QImage(imageW, imageH, QImage::Format_RGBA8888);
    originImage.fill(Qt::black);

    for(int j=0; j < imageH; ++j)
    {
        for(int i=0; i < imageW; ++i)
        {
            float r = i < imageW/2 ? 1.0 : 0.0;
            float g = i >= imageW/2 ? 1.0 : 0.0;
            float b = j > imageH/2 ? 1.0 : 0.0;
            originImage.setPixelColor(i,j, QColor(r*255,g*255,b*255));
        }
    }

    QImage image1 = originImage.copy();
    float pixel_area = 2.0*G_PI/imageW*G_PI/imageH;
    for (int y = 0; y < imageH; y++) {
        for (int x = 0; x < imageW; x++) {
            QVector3D n = this->toNormal((x + 0.5)* 2*M_PI/imageW, (y + 0.5) * M_PI/imageH);
            QVector3D sum(0,0,0);
            for (int ey = 0; ey < imageH; ey++) {
                float theta = (ey + 0.5)*M_PI/imageH;
                float sa = pixel_area*qSin(theta);

                for (int ex = 0; ex < imageW; ex++) {
                    QVector3D light = this->toNormal( (ex + 0.5)*2*M_PI/imageW, (ey+0.5)*M_PI/imageH );
                    float ldotn = GMath::clamp(QVector3D::dotProduct(light, n));
                    sum += sa*ldotn* GMath::toVector( originImage.pixelColor(ex,ey) );
                }
            }
            image1.setPixelColor(x, y, GMath::toColor(sum));
        }
    }

    QList<QVector3D> coffs = calculateShCoffs(originImage);
    QImage image2 = rebuildImage(coffs, imageW, imageH);

    for (int y = 0; y < imageH; y++) {
        for (int x = 0; x < imageW; x++) {
            QColor c1 = image1.pixelColor(x,y);
            QColor c2 = image2.pixelColor(x,y);
            QVector3D diff = GMath::toVector(c1) - GMath::toVector(c2);
            qDebug()<<diff;
        }
    }

//    QImage image2 = rebuildImage(coffs, image.width(), image.height());
//    QList<QVector3D> coffs2 = calculateShCoffs(image2);

//    for(int i = 0; i< coffs.size(); ++i)
//    {
//        qDebug()<<coffs2.at(i) - coffs.at(i);
//    }
}

void GSH::testEvalColor()
{
    QList<QVector3D> shCoffs;
    shCoffs.append(QVector3D(-1.028, -0.591, -1.119));

    shCoffs.append(QVector3D(0.779, -0.713, 0.559));
    shCoffs.append(QVector3D(-0.275, 0.191, 0.433));
    shCoffs.append(QVector3D(0.601, 1.206, -0.680));

    shCoffs.append(QVector3D(-0.256, -0.587, -1.815));
    shCoffs.append(QVector3D(1.891, -0.051, -0.915));
    shCoffs.append(QVector3D(-1.658, 1.543, 1.345));
    shCoffs.append(QVector3D(-0.370, -0.818, 1.572));
    shCoffs.append(QVector3D(-0.772, 1.482, -0.622));

    QVector3D n = toNormal(G_PI/4, G_PI/4);
    QVector3D expected(0.0f, 0.0f, 0.0f);
    for (int m = 0; m < 3; m++) {
        for (int l = 0; l < 2*m+1; l++) {
            expected += shCoffs.at(indexSH(m,l)) * baseSH(m,l,n);
        }
    }
    expected -= this->evalColor(shCoffs, n);
    qDebug()<<expected;
}

QImage GSH::testImage(QImage& image)
{
    QList<QVector3D> coffs = calculateShCoffs(image);
    return rebuildImage(coffs, image.width(), image.height());
}

// --------以上都是测试函数--------------------

QVector3D GSH::toNormal(float phi, float theta)
{
    return QVector3D( qSin(theta)*qCos(phi), qSin(theta)*qSin(phi), qCos(theta));
}

QImage GSH::rebuildImage(QList<QVector3D>& shCoffs, int width, int height)
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
            QVector3D cv = evalColor(shCoffs, toNormal(phi, theta));
            image.setPixelColor(w, h, GMath::toColor(cv) );
        }
    }

    return image;
}

QList<QVector3D> GSH::calculateShCoffs(QImage& image)
{
    QList<QVector3D> shCoffs;
    for(int i = 0; i<(SH_MAX_ORDER*SH_MAX_ORDER); ++i)
    {
        shCoffs.append(QVector3D(0,0,0));
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

            //计算每个颜色点对球谐基函数的贡献. 2种计算,结果一样.
            QVector<float> baseCoffs = this->calculateBaseCoffs( toNormal(phi, theta) );
            for(int i = 0; i<(SH_MAX_ORDER*SH_MAX_ORDER); ++i)
            {
                QVector3D coff = shCoffs.at(i);
                coff += baseCoffs.at(i)*weight*cv;
                shCoffs.replace(i, coff);
            }

//            for(int m = 0; m < SH_MAX_ORDER; ++m)
//            {
//                for(int l = 0; l < 2*m + 1; ++l)
//                {
//                    float shBase = this->baseSH(m, l, toNormal(phi, theta) );
//                    int index = indexSH(m, l);
//                    QVector3D coff = shCoffs.at(index);
//                    coff += shBase*weight*cv;
//                    shCoffs.replace(index, coff);
//                }
//            }
        }
    }

    return shCoffs;
}

int GSH::indexSH(int m, int l)
{
    return m*m + l;
}

QVector3D GSH::evalColor(QList<QVector3D>& shCoffs, QVector3D n)
{
    QVector3D sum(0,0,0);
    QVector<float> baseCoffs = this->calculateBaseCoffs(n);
    for(int i = 0; i < baseCoffs.size(); ++i)
    {
        sum += baseCoffs.at(i)*shCoffs.at(i);
    }
    return sum;
}

QVector<float> GSH::calculateBaseCoffs(QVector3D n)
{
    QVector<float> vec;
    for(int m = 0; m < SH_MAX_ORDER; ++m)
    {
        for(int l = 0; l < 2*m + 1; ++l)
        {
            vec.append(baseSH(m, l, n));
        }
    }

    return vec;
}

float GSH::baseSH(int m, int l, QVector3D n)
{
    if(m == 0)
    {
        if(l == 0) return baseSH_00(n);
    }
    else if(m == 1)
    {
        if(l == 0) return baseSH_10(n);
        if(l == 1) return baseSH_11(n);
        if(l == 2) return baseSH_12(n);
    }
    else if(m == 2)
    {
        if(l == 0) return baseSH_20(n);
        if(l == 1) return baseSH_21(n);
        if(l == 2) return baseSH_22(n);
        if(l == 3) return baseSH_23(n);
        if(l == 4) return baseSH_24(n);
    }
    else if(m == 3)
    {
        if(l == 0) return baseSH_30(n);
        if(l == 1) return baseSH_31(n);
        if(l == 2) return baseSH_32(n);
        if(l == 3) return baseSH_33(n);
        if(l == 4) return baseSH_34(n);
        if(l == 5) return baseSH_35(n);
        if(l == 6) return baseSH_36(n);
    }
    else if(m == 4)
    {
        if(l == 0) return baseSH_40(n);
        if(l == 1) return baseSH_41(n);
        if(l == 2) return baseSH_42(n);
        if(l == 3) return baseSH_43(n);
        if(l == 4) return baseSH_44(n);
        if(l == 5) return baseSH_45(n);
        if(l == 6) return baseSH_46(n);
        if(l == 7) return baseSH_47(n);
        if(l == 8) return baseSH_48(n);
    }

    return baseSH_other(m, l-m, n);
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


QVector2D GSH::toPhiTheta(QVector3D n)
{
    float theta = qCos(n.z());
    float phi = qAtan2(n.y(), n.x());
    return QVector2D(phi, theta);
}

double GSH::factorial(int x)
{
    if(x < 0)
    {
        qDebug()<<"error x "<<x;
    }

    int kCacheSize = 16;
    static const double factorial_cache[16] = {1, 1, 2, 6, 24, 120, 720, 5040,
                                                  40320, 362880, 3628800, 39916800,
                                                  479001600, 6227020800,
                                                  87178291200, 1307674368000};

      if (x < kCacheSize) {
        return factorial_cache[x];
      } else {
        double s = factorial_cache[kCacheSize - 1];
        for (int n = kCacheSize; n <= x; n++) {
          s *= n;
        }
        return s;
      }
}

double GSH::doubleFactorial(int x)
{
//    if(x < 0)
//    {
//        qDebug()<<"error x "<<x;
//    }

    int kCacheSize = 16;
    static const double dbl_factorial_cache[16] = {1, 1, 2, 3, 8, 15, 48, 105,
                                                      384, 945, 3840, 10395, 46080,
                                                      135135, 645120, 2027025};

      if (x < kCacheSize) {
        return dbl_factorial_cache[x];
      } else {
        double s = dbl_factorial_cache[kCacheSize - (x % 2 == 0 ? 2 : 1)];
        double n = x;
        while (n >= kCacheSize) {
          s *= n;
          n -= 2.0;
        }
        return s;
      }
}

double GSH::evalLegendrePolynomial(int l, int m, float x)
{
    float pmm = 1.0;
      // P00 is defined as 1.0, do don't evaluate Pmm unless we know m > 0
      if (m > 0) {
        double sign = (m % 2 == 0 ? 1 : -1);
        pmm = sign * doubleFactorial(2 * m - 1) * qPow(1 - x * x, m / 2.0);
      }

      if (l == m) {
        return pmm;
      }

      float pmm1 = x * (2 * m + 1) * pmm;
      if (l == m + 1) {
        // Pml is the same as Pmm+1 so we are done as well
        return pmm1;
      }

      // Use the last two computed bands to lift up to the next band until l is
      // reached, using the recurrence relationship:
      // Pml(x) = (x(2l - 1)Pml-1 - (l + m - 1)Pml-2) / (l - m)
      for (int n = m + 2; n <= l; n++) {
        double pmn = (x * (2 * n - 1) * pmm1 - (n + m - 1) * pmm) / (n - m);
        pmm = pmm1;
        pmm1 = pmn;
      }
      // Pmm1 at the end of the above loop is equal to Pml
      return pmm1;
}

double GSH::baseSH_other(int l, int m, QVector3D& n)
{
    QVector2D v = this->toPhiTheta(n);
    float phi = v.x();
    float theta = v.y();

    double kml = sqrt((2.0 * l + 1) * factorial(l - qAbs(m)) /(4.0 * G_PI * factorial(l + qAbs(m))));
      if (m > 0) {
        return sqrt(2.0) * kml * cos(m * phi) *
            evalLegendrePolynomial(l, m, qCos(theta));
      } else if (m < 0) {
        return sqrt(2.0) * kml * sin(-m * phi) *
            evalLegendrePolynomial(l, -m, qCos(theta));
      } else {
        return kml * evalLegendrePolynomial(l, 0, qCos(theta));
      }
}
