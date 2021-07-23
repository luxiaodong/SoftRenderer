#include "gmodel.h"
#include <QVector3D>
#include <QVector2D>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

GModel::GModel()
{
//    m_resourceDir = "/Users/luxiaodong/Project/SoftRenderer/Resource";
}

GMesh GModel::loadTriangle()
{
    GMesh mesh;
    mesh.m_vertexs.append( QVector3D(5.0, 0.0, 5.0) );
    mesh.m_vertexs.append( QVector3D(-5.0, 0.0, -5.0) );
    mesh.m_vertexs.append( QVector3D(5.0, 0.0, -5.0) );
    mesh.m_normals.append( QVector3D(0.0, 1.0, 0.0) );
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_indexs.append( GVertexIndex(0, 0, 0) );
    mesh.m_indexs.append( GVertexIndex(1, 1, 0) );
    mesh.m_indexs.append( GVertexIndex(2, 2, 0) );
    return mesh;
}

GMesh GModel::loadPlane()
{
    GMesh mesh;
    mesh.m_vertexs.append( QVector3D(5.0, 0.0, 5.0) );
    mesh.m_vertexs.append( QVector3D(-5.0,0.0,-5.0) );
    mesh.m_vertexs.append( QVector3D(5.0, 0.0,-5.0) );
    mesh.m_vertexs.append( QVector3D(-5.0,0.0, 5.0) );
    mesh.m_normals.append( QVector3D(0.0, 1.0, 0.0) );
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_indexs.append( GVertexIndex(0, 0, 0) );
    mesh.m_indexs.append( GVertexIndex(1, 1, 0) );
    mesh.m_indexs.append( GVertexIndex(2, 2, 0) );
    mesh.m_indexs.append( GVertexIndex(3, 3, 0) );
    mesh.m_indexs.append( GVertexIndex(1, 1, 0) );
    mesh.m_indexs.append( GVertexIndex(0, 0, 0) );
    return mesh;
}

GMesh GModel::loadMarry()
{
//    return GModel::loadObject("/Users/luxiaodong/Project/SoftRenderer/Resource/cube.obj");
    return GModel::loadObject("C:/Users/luxia/Documents/GitHub/SoftRenderer/Resource/suzanne.obj");
}

GMesh GModel::loadObject(QString filePath)
{
    GMesh mesh;

    if(QFile::exists(filePath))
    {
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            while(!stream.atEnd())
            {
                QString line = stream.readLine();
                if (line.left(2) == "v ")
                {
                    QStringList tempList = line.split(" ");
                    float x = tempList.at(1).toFloat();
                    float y = tempList.at(2).toFloat();
                    float z = tempList.at(3).toFloat();
                    mesh.m_vertexs.append(QVector3D(x, y, z));
                }
                else if (line.left(3) == "vn ")
                {
                    QStringList tempList = line.split(" ");
                    float x = tempList.at(1).toFloat();
                    float y = tempList.at(2).toFloat();
                    float z = tempList.at(3).toFloat();
                    mesh.m_normals.append(QVector3D(x, y, z));
                }
                else if (line.left(3) == "vt ")
                {
                    QStringList tempList = line.split(" ");
                    float x = tempList.at(1).toFloat();
                    float y = tempList.at(2).toFloat();
                    mesh.m_uvs.append(QVector2D(x,y));
                }
                else if (line.left(2) == "f ")
                {
                    QStringList tempList = line.replace("/", " ").split(" ");
                    for(int i = 1; i < tempList.size(); i+=3 )
                    {
                        GVertexIndex vIndex;
                        vIndex.m_vertexIndex = tempList.at(i).toInt() - 1;
                        vIndex.m_uvIndex = tempList.at(i+1).toInt() - 1;
                        vIndex.m_normalIndex = tempList.at(i+2).toInt() - 1;
                        mesh.m_indexs.append(vIndex);
                    }
                }
            }
        }
    }

    mesh.printInfo();
    return mesh;
}
