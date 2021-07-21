#ifndef GMESH_H
#define GMESH_H

#include <QList>
#include <QVector3D>
#include <QColor>

class GMesh
{
public:
    GMesh();
    void setPoints(QList<QVector3D> &point){m_points = point;}

    QList<QColor> m_colors;
    QList<QVector3D> m_points;
    QList<QVector3D> m_normals;
};

#endif // GMESH_H
