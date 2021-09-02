#include "brdfshader.h"
#include "Math/gmath.h"

BRDFShader::BRDFShader(QColor color):GShader(color)
{
    float x = 0.3213938;
    float y = 0.7660444;
    float z = -0.5566705;
    m_lightDir = QVector3D(x, y, z);
    m_f0 = QVector3D(0.04, 0.04, 0.04);
    m_roughness = 0.5f;
    m_metalness = 0.5f;
//    m_fresnel = QVector3D(1.0f, 1.0f, 1.0f);
//    m_fresnel = QVector3D(0, 0, 0);
}

void BRDFShader::initArgs()
{}

QColor BRDFShader::fragment(float x, float y, GVertexAttribute& va, QMap<QString, QImage>& map)
{
    QVector3D lDir = m_lightDir;
    QVector3D vDir = (QVector3D(0, 10, -10) - (m_projMat.inverted()*va.m_vertex).toVector3D()).normalized();
    QVector3D hDir = (vDir + lDir).normalized();
    QVector3D nDir = va.m_normal.normalized();

    QVector3D f0 = GMath::lerp(m_f0, m_fresnel, m_metalness);
    QVector3D F = factor_f(hDir, vDir, f0);
    float D = factor_d(hDir, nDir, m_roughness);
    float G = factor_g(hDir, lDir, vDir, m_roughness);
    float denom = 4.0f*qMax(0.0f, QVector3D::dotProduct(nDir, vDir))*qMax(0.0f, QVector3D::dotProduct(nDir, lDir)) + 0.001;
    QVector3D f = (D*F*G)/denom;

    QVector3D ks = F;
    QVector3D kd = (QVector3D(1.0f, 1.0f, 1.0f) - ks)*(1.0f - m_metalness);
    QVector3D albedo = GMath::toVector(Qt::white);
    QVector3D L0 = (kd*albedo/G_PI + f)*qMax(0.0f, QVector3D::dotProduct(nDir, lDir));

    QColor finalColor = GMath::toColor(L0);
    return finalColor;
}

float BRDFShader::factor_d(QVector3D h, QVector3D n, float r)
{
    float a = r*r;
    float hdotn = qMax(0.0f, QVector3D::dotProduct(h,n));
    float denom = hdotn*(a-1) + 1;
    return a/(G_PI*denom*denom);
}

float BRDFShader::factor_g_sub(QVector3D h, QVector3D v, float r)
{
    float k = (r+1)*(r+1)/8;
    float hdotv = qMax(0.0f, QVector3D::dotProduct(h,v));
    return hdotv/(hdotv*(1-k) + k);
}

float BRDFShader::factor_g(QVector3D h, QVector3D l, QVector3D v, float r)
{
    return factor_g_sub(h,v,r)*factor_g_sub(h,l,r);
}

QVector3D BRDFShader::factor_f(QVector3D h, QVector3D v, QVector3D F0)
{
    float hdotv = qMax(0.0f, QVector3D::dotProduct(h,v));
    return F0 + (QVector3D(1.0f, 1.0f, 1.0f) - F0) * qPow(1 - hdotv, 5);
}

