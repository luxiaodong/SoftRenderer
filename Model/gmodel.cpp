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
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, 0.5));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));

//    mesh.m_indexs.append(GVertexIndex(0));
//    mesh.m_indexs.append(GVertexIndex(1));
//    mesh.m_indexs.append(GVertexIndex(2));

    mesh.m_indexs.append(GVertexIndex(0));
    mesh.m_indexs.append(GVertexIndex(2));
    mesh.m_indexs.append(GVertexIndex(3));

//    mesh.m_vertexs.append( QVector3D(5.0, 0.0, 5.0) );
//    mesh.m_vertexs.append( QVector3D(-5.0, 0.0, -5.0) );
//    mesh.m_vertexs.append( QVector3D(5.0, 0.0, -5.0) );
//    mesh.m_normals.append( QVector3D(0.0, 1.0, 0.0) );
//    mesh.m_uvs.append(QVector2D(1.0, 1.0f - 1.0));
//    mesh.m_uvs.append(QVector2D(0.0, 1.0f - 0.0));
//    mesh.m_uvs.append(QVector2D(1.0, 1.0f - 0.0));
//    mesh.m_indexs.append( GVertexIndex(0, 0, 0) );
//    mesh.m_indexs.append( GVertexIndex(1, 1, 0) );
//    mesh.m_indexs.append( GVertexIndex(2, 2, 0) );
    return mesh;
}

GMesh GModel::loadPlane()
{
    GMesh mesh;
    mesh.m_vertexs.append( QVector3D(-0.5, -0.5,  0.0) );
    mesh.m_vertexs.append( QVector3D( 0.5, -0.5,  0.0) );
    mesh.m_vertexs.append( QVector3D(-0.5,  0.5,  0.0) );
    mesh.m_vertexs.append( QVector3D( 0.5,  0.5,  0.0) );
    mesh.m_normals.append( QVector3D( 0.0,  0.0, -1.0) );
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_indexs.append( GVertexIndex(0, 0, 0) );
    mesh.m_indexs.append( GVertexIndex(1, 1, 0) );
    mesh.m_indexs.append( GVertexIndex(3, 3, 0) );
    mesh.m_indexs.append( GVertexIndex(0, 0, 0) );
    mesh.m_indexs.append( GVertexIndex(3, 3, 0) );
    mesh.m_indexs.append( GVertexIndex(2, 2, 0) );
    return mesh;
}

GMesh GModel::loadUnityCube()
{
    GMesh mesh;
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, -0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, -0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, -0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(-0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, -0.5));
    mesh.m_vertexs.append(QVector3D(0.5, 0.5, 0.5));
    mesh.m_vertexs.append(QVector3D(0.5, -0.5, 0.5));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, 1.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, 1.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, 1.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, 1.0));
    mesh.m_normals.append(QVector3D(0.0, 1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, 1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, -1.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, -1.0));
    mesh.m_normals.append(QVector3D(0.0, 1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, 1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, -1.0));
    mesh.m_normals.append(QVector3D(0.0, 0.0, -1.0));
    mesh.m_normals.append(QVector3D(0.0, -1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, -1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, -1.0, 0.0));
    mesh.m_normals.append(QVector3D(0.0, -1.0, 0.0));
    mesh.m_normals.append(QVector3D(-1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(-1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(-1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(-1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));
    mesh.m_normals.append(QVector3D(1.0, 0.0, 0.0));

    mesh.m_indexs.append(GVertexIndex(0));
    mesh.m_indexs.append(GVertexIndex(2));
    mesh.m_indexs.append(GVertexIndex(3));
    mesh.m_indexs.append(GVertexIndex(0));
    mesh.m_indexs.append(GVertexIndex(3));
    mesh.m_indexs.append(GVertexIndex(1));

    mesh.m_indexs.append(GVertexIndex(8));
    mesh.m_indexs.append(GVertexIndex(4));
    mesh.m_indexs.append(GVertexIndex(5));
    mesh.m_indexs.append(GVertexIndex(8));
    mesh.m_indexs.append(GVertexIndex(5));
    mesh.m_indexs.append(GVertexIndex(9));

    mesh.m_indexs.append(GVertexIndex(10));
    mesh.m_indexs.append(GVertexIndex(6));
    mesh.m_indexs.append(GVertexIndex(7));
    mesh.m_indexs.append(GVertexIndex(10));
    mesh.m_indexs.append(GVertexIndex(7));
    mesh.m_indexs.append(GVertexIndex(11));

    mesh.m_indexs.append(GVertexIndex(12));
    mesh.m_indexs.append(GVertexIndex(13));
    mesh.m_indexs.append(GVertexIndex(14));
    mesh.m_indexs.append(GVertexIndex(12));
    mesh.m_indexs.append(GVertexIndex(14));
    mesh.m_indexs.append(GVertexIndex(15));

    mesh.m_indexs.append(GVertexIndex(16));
    mesh.m_indexs.append(GVertexIndex(17));
    mesh.m_indexs.append(GVertexIndex(18));
    mesh.m_indexs.append(GVertexIndex(16));
    mesh.m_indexs.append(GVertexIndex(18));
    mesh.m_indexs.append(GVertexIndex(19));

    mesh.m_indexs.append(GVertexIndex(20));
    mesh.m_indexs.append(GVertexIndex(21));
    mesh.m_indexs.append(GVertexIndex(22));
    mesh.m_indexs.append(GVertexIndex(20));
    mesh.m_indexs.append(GVertexIndex(22));
    mesh.m_indexs.append(GVertexIndex(23));

    return mesh;
}

GMesh GModel::loadCube(bool isSkybox)
{
    GMesh mesh;
    mesh.m_vertexs.append( QVector3D(-0.5,-0.5,-0.5) );
    mesh.m_vertexs.append( QVector3D(0.5,-0.5,-0.5) );
    mesh.m_vertexs.append( QVector3D(0.5,0.5,-0.5) );
    mesh.m_vertexs.append( QVector3D(-0.5,0.5,-0.5) );
    mesh.m_vertexs.append( QVector3D(-0.5,-0.5,0.5) );
    mesh.m_vertexs.append( QVector3D(0.5,-0.5,0.5) );
    mesh.m_vertexs.append( QVector3D(0.5,0.5,0.5) );
    mesh.m_vertexs.append( QVector3D(-0.5,0.5,0.5) );
    mesh.m_normals.append( QVector3D(0,0,-1) );
    mesh.m_normals.append( QVector3D(1,0,0) );
    mesh.m_normals.append( QVector3D(0,0,1) );
    mesh.m_normals.append( QVector3D(-1,0,0) );
    mesh.m_normals.append( QVector3D(0,1,0) );
    mesh.m_normals.append( QVector3D(0,-1,0) );
    mesh.m_uvs.append(QVector2D(0.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 0.0));
    mesh.m_uvs.append(QVector2D(1.0, 1.0));
    mesh.m_uvs.append(QVector2D(0.0, 1.0));

    mesh.m_indexs.append( GVertexIndex(0,0,0) );
    mesh.m_indexs.append( GVertexIndex(1,1,0) );
    mesh.m_indexs.append( GVertexIndex(2,2,0) );
    mesh.m_indexs.append( GVertexIndex(0,0,0) );
    mesh.m_indexs.append( GVertexIndex(2,2,0) );
    mesh.m_indexs.append( GVertexIndex(3,3,0) );

    mesh.m_indexs.append( GVertexIndex(1,0,1) );
    mesh.m_indexs.append( GVertexIndex(5,1,1) );
    mesh.m_indexs.append( GVertexIndex(6,2,1) );
    mesh.m_indexs.append( GVertexIndex(1,0,1) );
    mesh.m_indexs.append( GVertexIndex(6,2,1) );
    mesh.m_indexs.append( GVertexIndex(2,3,1) );

    mesh.m_indexs.append( GVertexIndex(6,0,2) );
    mesh.m_indexs.append( GVertexIndex(5,1,2) );
    mesh.m_indexs.append( GVertexIndex(4,2,2) );
    mesh.m_indexs.append( GVertexIndex(7,3,2) );
    mesh.m_indexs.append( GVertexIndex(6,0,2) );
    mesh.m_indexs.append( GVertexIndex(4,2,2) );

    mesh.m_indexs.append( GVertexIndex(4,0,3) );
    mesh.m_indexs.append( GVertexIndex(0,1,3) );
    mesh.m_indexs.append( GVertexIndex(3,2,3) );
    mesh.m_indexs.append( GVertexIndex(7,3,3) );
    mesh.m_indexs.append( GVertexIndex(4,0,3) );
    mesh.m_indexs.append( GVertexIndex(3,2,3) );

    mesh.m_indexs.append( GVertexIndex(3,0,4) );
    mesh.m_indexs.append( GVertexIndex(2,1,4) );
    mesh.m_indexs.append( GVertexIndex(6,2,4) );
    mesh.m_indexs.append( GVertexIndex(3,0,4) );
    mesh.m_indexs.append( GVertexIndex(6,2,4) );
    mesh.m_indexs.append( GVertexIndex(7,3,4) );

    mesh.m_indexs.append( GVertexIndex(5,0,5) );
    mesh.m_indexs.append( GVertexIndex(1,1,5) );
    mesh.m_indexs.append( GVertexIndex(0,2,5) );
    mesh.m_indexs.append( GVertexIndex(4,3,5) );
    mesh.m_indexs.append( GVertexIndex(5,0,5) );
    mesh.m_indexs.append( GVertexIndex(0,2,5) );

    if(isSkybox)
    {
        for(int i = 0; i<mesh.m_indexs.size()/3; ++i)
        {
            GVertexIndex one = mesh.m_indexs.at(3*i);
            GVertexIndex two = mesh.m_indexs.at(3*i+1);
            mesh.m_indexs.replace(3*i, two);
            mesh.m_indexs.replace(3*i+1, one);
        }
    }

    return mesh;
}

GMesh GModel::loadMarry()
{
    return GModel::loadObject(":/mesh/cube.obj");
//    return GModel::loadObject("/Users/luxiaodong/Project/SoftRenderer/Resource/suzanne.obj");
//    return GModel::loadObject("C:/Users/luxia/Documents/GitHub/SoftRenderer/Resource/suzanne.obj");
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
                    //这里的模型坐标系和unity互为镜像.
                    mesh.m_vertexs.append(QVector3D(-x, y, z));
                }
                else if (line.left(3) == "vn ")
                {
                    QStringList tempList = line.split(" ");
                    float x = tempList.at(1).toFloat();
                    float y = tempList.at(2).toFloat();
                    float z = tempList.at(3).toFloat();
                    mesh.m_normals.append(QVector3D(-x, y, z));
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
