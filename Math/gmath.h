#ifndef GMATH_H
#define GMATH_H

#define G_PI 3.1415926f

#include <QVector2D>
#include <QVector3D>
#include <QMatrix>
#include <QMatrix4x4>
#include <QColor>
#include <QtMath>
#include <QQuaternion>

class GMath
{
public:
    GMath();
    static void testDecompositionTRS();

    static QVector3D clamp(QVector3D);
    static float clamp(float);
    static QVector3D toVector(QColor);
    static QColor toColor(QVector3D);
    static QVector2D normalToUv(QVector3D n);
    static QVector3D uvToNormal(QVector2D uv);
};

#endif // GMATH_H
