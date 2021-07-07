#ifndef GMESH_H
#define GMESH_H

#include <QList>
#include <QVector3D>

class GMesh
{
public:
    GMesh();
    void setPoints(QList<QVector3D> &point){m_points = point;}


    QList<QVector3D> m_points;
};

#endif // GMESH_H
