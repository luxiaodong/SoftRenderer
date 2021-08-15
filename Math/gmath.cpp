#include "gmath.h"

GMath::GMath()
{

}

float GMath::clamp(float f)
{
    if(f < 0) return 0;
    if(f > 1) return 1;
    return f;
}

QVector3D GMath::clamp(QVector3D v)
{
    float x = clamp(v.x());
    float y = clamp(v.y());
    float z = clamp(v.z());
    return QVector3D(x,y,z);
}

QVector3D GMath::toVector(QColor c)
{
    float x = c.redF();
    float y = c.greenF();
    float z = c.blueF();
    return QVector3D(x,y,z);
}

QColor GMath::toColor(QVector3D v)
{
    QVector3D cv = clamp(v);
    int r = cv.x()*255;
    int g = cv.y()*255;
    int b = cv.z()*255;
    return QColor(r,g,b);
}


