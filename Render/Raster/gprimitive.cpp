#include "gprimitive.h"

GPrimitive::GPrimitive()
{

}

void GPrimitive::setTriangle(QVector4D a, QVector4D b, QVector4D c)
{
    m_a = a;
    m_b = b;
    m_c = c;
}

void GPrimitive::homogeneousDiv()
{
    //z in [-1,1]
    QVector4D a = QVector4D(m_a.x()/m_a.w(), m_a.y()/m_a.w(), m_a.z()/m_a.w(), 1.0f);
    QVector4D b = QVector4D(m_b.x()/m_a.w(), m_b.y()/m_b.w(), m_b.z()/m_b.w(), 1.0f);
    QVector4D c = QVector4D(m_c.x()/m_a.w(), m_c.y()/m_c.w(), m_c.z()/m_c.w(), 1.0f);

    m_a = a;
    m_b = b;
    m_c = c;
}

bool GPrimitive::isDiscardCullingSuccess()
{
    float value = (m_b.x() - m_a.x())*(m_c.y() - m_a.y()) - (m_b.y() - m_a.y())*(m_c.x() - m_a.x());
    return value < 0.0f;
}
