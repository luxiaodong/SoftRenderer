#include "widget.h"
#include "Scene/gscene.h"
#include "Scene/gcamera.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_width = 600;
    m_height = 600;

    GCamera* pCamera = new GCamera();
    pCamera->setViewMatrix(QVector3D(0, 10, -10), 45, 0, 0);
    //pCamera->setViewMatrix(QVector3D(0,0,0), QVector3D(-0.6, 0.8, 0), QVector3D(0, -5.0f/13, 12.0f/13));
    pCamera->setProjMatrix(60, m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setViewPortMatrix(0,0,m_width,m_height);

    m_raster = new GRaster();
    m_raster->setCamera(pCamera);
    m_raster->setRenderSize(QSize(m_width, m_height));

    m_pScene = new GScene();
    m_pScene->loadSceneTest();

    m_drawOnce = 0;
    this->resize(m_width, m_height);
}

void Widget::paintEvent(QPaintEvent*)
{
    m_drawOnce++;
    if(m_drawOnce >= 3)
    {
        m_raster->clearColor(Qt::black);
        m_raster->clearDepth();

        qDebug()<<"gameobject count is "<<m_pScene->m_gameObjects.size();
        foreach (GGameObject go, m_pScene->m_gameObjects)
        {
            m_raster->renderGameObject(go);
        }

        QPainter painter(this);
        painter.drawImage( QRectF(0,0,m_width, m_height), this->genImage(m_width, m_height, m_raster->frameBuffer()));

//        QPainter painter(this);
//        QImage image;
//        image.load(":/texture/uv.tga");
//        painter.drawImage( QRectF(0,0,m_width, m_height), image);
    }
}

QImage Widget::genImage(int width, int height, int* data)
{
    QImage image(width, height, QImage::Format_RGBA8888);

    for(int j=0; j < height; ++j)
    {
        for(int i=0; i < width; ++i)
        {
            int r = data[(j*width + i)*3];
            int g = data[(j*width + i)*3 + 1];
            int b = data[(j*width + i)*3 + 2];
            image.setPixelColor(i,j, QColor(r,g,b));
        }
    }

//    image = image.copy(450,750,50,50);
    return image.mirrored();
}

Widget::~Widget()
{
}
