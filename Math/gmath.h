#ifndef GMATH_H
#define GMATH_H

#define G_PI 3.1415926f

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
};

#endif // GMATH_H
