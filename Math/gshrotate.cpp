#include "gshrotate.h"

GSHRotate::GSHRotate()
{
    m_r0.fill(0);
    m_r1.fill(0);
    m_r2.fill(0);
    m_r3.fill(0);
    m_r4.fill(0);
}

QList<QVector3D> GSHRotate::getCoff(QVector3D n)
{
    QList<QVector3D> coffs;
    for(int i = 0; i < 25; ++i)
    {
        coffs.append(QVector3D(0.0f, 0.0f, 0.0f));
    }

    return coffs;
}

void GSHRotate::createMatrix(QVector3D normal)
{
    QQuaternion q = QQuaternion::rotationTo(QVector3D(0,0,1), normal).normalized();
    //0阶 1x1
    m_r0(0, 0) = 1.0f;
    //1阶 3x3
    QMatrix3x3 r_mat = q.toRotationMatrix();
    m_r1(0, 0) = r_mat(1, 1);
    m_r1(0, 1) = -r_mat(1, 2);
    m_r1(0, 2) = r_mat(1, 0);
    m_r1(1, 0) = -r_mat(2, 1);
    m_r1(1, 1) = r_mat(2, 2);
    m_r1(1, 2) = -r_mat(2, 0);
    m_r1(2, 0) = r_mat(0, 1);
    m_r1(2, 1) = -r_mat(0, 2);
    m_r1(2, 2) = r_mat(0, 0);
    //2阶 5x5, 3阶7x7, 4阶9x9
    for (int l = 2; l < 5; ++l) {
        for(int m = -l; m<=l; ++m){
            for(int n = -l; n <=l; ++n)
            {
                QVector3D uvw = computeUVWCoeff(m, n, l);
                float u = uvw.x();
                float v = uvw.y();
                float w = uvw.z();

                if (!isNearByMargin(u, 0.0))
                    u *= uCoff(m, n, l);
                if (!isNearByMargin(v, 0.0))
                    v *= vCoff(m, n, l);
                if (!isNearByMargin(w, 0.0))
                    w *= wCoff(m, n, l);

                setMatrixElement(l, m, n, u + v + w);
            }
        }
    }
}

QVector3D GSHRotate::computeUVWCoeff(int m, int n, int l)
{
    float d = kroneckerDelta(m, 0);
    float denom = (qAbs(n) == l ? 2.0 * l * (2.0 * l - 1) : (l + n) * (l - n));
    float u = qSqrt((l + m) * (l - m) / denom);
    float v = 0.5 * qSqrt((1 + d) * (l + qAbs(m) - 1.0) * (l + qAbs(m)) / denom) * (1 - 2 * d);
    float w = -0.5 * qSqrt((l - qAbs(m) - 1) * (l - qAbs(m)) / denom) * (1 - d);
    return QVector3D(u, v, w);
}

float GSHRotate::uCoff(int m, int n, int l)
{
    return pCoff(0, m, n, l);
}

float GSHRotate::vCoff(int m, int n, int l)
{
    if (m == 0) {
        return pCoff(1, 1, n, l) + pCoff(-1, -1, n, l);
      } else if (m > 0) {
        return pCoff(1, m - 1, n, l) * qSqrt(1 + kroneckerDelta(m, 1)) -
            pCoff(-1, -m + 1, n, l) * (1 - kroneckerDelta(m, 1));
      } else {
        return pCoff(1, m + 1, n, l) * (1 - kroneckerDelta(m, -1)) +
            pCoff(-1, -m - 1, n, l) * qSqrt(1 + kroneckerDelta(m, -1));
      }
}

float GSHRotate::wCoff(int m, int n, int l)
{
    if (m == 0) {
        return 0.0;
      } else if (m > 0) {
        return pCoff(1, m + 1, n, l) + pCoff(-1, -m - 1, n, l);
      } else {
        return pCoff(1, m - 1, n, l) - pCoff(-1, -m + 1, n, l);
      }
}

float GSHRotate::pCoff(int i, int a, int b, int l)
{
    if (b == l) {
        return getMatrixElement(1, i, 1) *
            getMatrixElement(l - 1, a, l - 1) -
            getMatrixElement(1, i, -1) *
            getMatrixElement(l - 1, a, -l + 1);
      } else if (b == -l) {
        return getMatrixElement(1, i, 1) *
            getMatrixElement(l - 1, a, -l + 1) +
            getMatrixElement(1, i, -1) *
            getMatrixElement(l - 1, a, l - 1);
      } else {
        return getMatrixElement(1, i, 0) * getMatrixElement(l - 1, a, b);
      }
}

float GSHRotate::getMatrixElement(int matIndex, int i, int j)
{
    if(matIndex == 0)
    {
        return m_r0(i+matIndex, j+matIndex);
    }
    else if(matIndex == 1)
    {
        return m_r1(i+matIndex, j+matIndex);
    }
    else if(matIndex == 2)
    {
        return m_r2(i+matIndex, j+matIndex);
    }
    else if(matIndex == 3)
    {
        return m_r3(i+matIndex, j+matIndex);
    }
    else if(matIndex == 4)
    {
        return m_r4(i+matIndex, j+matIndex);
    }

    return 0.0;
}

void GSHRotate::setMatrixElement(int matIndex, int i, int j, float value)
{
    if(matIndex == 0)
    {
        m_r0(i+matIndex, j+matIndex) = value;
    }
    else if(matIndex == 1)
    {
        m_r1(i+matIndex, j+matIndex) = value;
    }
    else if(matIndex == 2)
    {
        m_r2(i+matIndex, j+matIndex) = value;
    }
    else if(matIndex == 3)
    {
        m_r3(i+matIndex, j+matIndex) = value;
    }
    else if(matIndex == 4)
    {
        m_r4(i+matIndex, j+matIndex) = value;
    }
}

bool GSHRotate::isNearByMargin(float value, float margin)
{
    float diff = qAbs(value - margin);
    return diff < 0.0000001f;
}

float GSHRotate::kroneckerDelta(int i, int j)
{
    if (i == j) return 1.0;
    return 0.0f;
}

