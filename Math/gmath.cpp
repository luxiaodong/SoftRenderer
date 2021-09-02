#include "gmath.h"
#include <QDebug>

GMath::GMath()
{

}

void GMath::testDecompositionTRS()
{
    float xRadians = qDegreesToRadians(30.0f);
    float yRadians = qDegreesToRadians(45.0f);
    float zRadians = qDegreesToRadians(60.0f);

    QMatrix4x4 xMat(
                1.0,            0.0,             0.0, 0.0,
                0.0, qCos(xRadians), -qSin(xRadians), 0.0,
                0.0, qSin(xRadians),  qCos(xRadians), 0.0,
                0.0,            0.0,             0.0, 1.0
                );

    QMatrix4x4 yMat(
                qCos(yRadians),  0.0, qSin(yRadians), 0.0,
                0.0,             1.0,            0.0, 0.0,
                -qSin(yRadians), 0.0, qCos(yRadians), 0.0,
                0.0,             0.0,            0.0, 1.0
                );

    QMatrix4x4 zMat(
                qCos(zRadians), -qSin(zRadians), 0.0, 0.0,
                qSin(zRadians),  qCos(zRadians), 0.0, 0.0,
                0.0,            0.0,             1.0, 0.0,
                0.0,            0.0,             0.0, 1.0
                );

    QMatrix4x4 r = yMat*xMat*zMat;
    QMatrix4x4 s(1,0,0,0,
                 0,0,0,0,
                 0,0,3,0,
                 0,0,0,1);
qDebug()<<r;
    QMatrix4x4 m = r*s;
qDebug()<<m;

//方法3
    float sx = m.map(QVector4D(1,0,0,0)).toVector3D().length();
    float sy = m.map(QVector4D(0,1,0,0)).toVector3D().length();
    float sz = m.map(QVector4D(0,0,1,0)).toVector3D().length();
    qDebug()<<sx<<sy<<sz;
    //分解后的矩阵不保证唯一性,只保证乘积的矩阵是对的
    //需要检查是否为负数,用行列式检查,如果是则任意轴添加一个负数即可
    //检查是否为0.如果有两个0

//方法2,用四元素
    QVector3D forward = m.column(2).toVector3D();
    QVector3D up = m.column(1).toVector3D();
    QQuaternion q2 = QQuaternion::fromDirection(forward, up);
qDebug()<<q2.toRotationMatrix();

//方法1,迭代
    for(int i=0; i<100; ++i)
    {
        QMatrix4x4 mt = m.transposed().inverted();
        m = (m + mt)/2;
    }
qDebug()<<m;
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

QVector2D GMath::normalToUv(QVector3D n)
{
    float theta = qAcos(n.y()); //(0, pi)
    float phi = qAtan2(n.z(), n.x()); //(-pi, pi]
    phi += G_PI;
    return QVector2D(phi/2/G_PI , theta/G_PI);
}

QVector3D GMath::uvToNormal(QVector2D uv)
{
    float phi = 2*G_PI*uv.x();
//    float theta = G_PI*uv.y(); // 非均匀分布
//    float theta = qACos(1 - uv.y()); //均匀分布
//    float theta = 2*qAcos(qSqrt(1- uv.y())); // 余弦均匀分布
    float theta = qAcos(1 - 2*uv.y()); //余弦均匀分布,等同于上面

    return QVector3D( qSin(theta)*qCos(phi), qSin(theta)*qSin(phi), qCos(theta));
}



