#ifndef GSHROTATE_H
#define GSHROTATE_H

#include <QtMath>
#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include <QGenericMatrix>

class GSHRotate
{
public:
    GSHRotate();

    QList<QVector3D> getCoff(QVector3D n);
    void createMatrix(QVector3D n);
    QList<QVector3D> apply();

    QVector3D computeUVWCoeff(int m, int n, int l);

    float uCoff(int m, int n, int l);
    float vCoff(int m, int n, int l);
    float wCoff(int m, int n, int l);
    float pCoff(int i, int a, int b, int l);
//    float getCenteredElement(int matIndex, int i, int j);
    float getMatrixElement(int matIndex, int i, int j);
    void setMatrixElement(int matIndex, int i, int j, float value);

    float kroneckerDelta(int i, int j);
    bool isNearByMargin(float value, float margin);

private:
    QGenericMatrix<1, 1, float> m_r0;
    QGenericMatrix<3, 3, float> m_r1;
    QGenericMatrix<5, 5, float> m_r2;
    QGenericMatrix<7, 7, float> m_r3;
    QGenericMatrix<9, 9, float> m_r4;
};

#endif // GSHROTATE_H
