#include "gmesh.h"
#include <QDebug>

GMesh::GMesh()
{
    m_uvs.clear();
    m_vertexs.clear();
    m_normals.clear();
    m_indexs.clear();
}

void GMesh::printInfo()
{
    qDebug()<<"vertex size is "<<m_vertexs.size();
    qDebug()<<"uv size is "<<m_uvs.size();
    qDebug()<<"normal size is "<<m_normals.size();
    qDebug()<<"indexs size is "<<m_indexs.size();

//    for(int i =0;i<m_vertexs.size(); ++i)
//    {
//        qDebug()<<m_vertexs.at(i)<<m_normals.at(i);
//    }

//    foreach(QVector2D uv, m_uvs)
//    {
//        qDebug()<<uv;
//    }
}

