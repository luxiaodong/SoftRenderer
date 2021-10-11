#include "widget.h"
#include "Scene/gscene.h"
#include "Scene/gcamera.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_width = 512;
    m_height = 512;

    GCamera* pCamera = new GCamera();
//    pCamera->setViewMatrix(QVector3D(0,0,0), QVector3D(-0.6, 0.8, 0), QVector3D(0, -5.0f/13, 12.0f/13));
    pCamera->setViewMatrix(QVector3D(0, 10, -10), 45, 0, 0);
//    pCamera->setOrthMatrix(5,  m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setProjMatrix(60, m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setViewPortMatrix(0,0,m_width,m_height);

    m_raster = new GRaster();
    m_raster->setRenderSize(QSize(m_width, m_height));

    m_pScene = new GScene();
    m_pScene->m_camera = pCamera;
//    m_pScene->loadTriangle();
    m_pScene->loadPlane();
    m_pScene->loadModel();
//    m_pScene->loadSphere();

    m_drawOnce = 0;
    this->resize(m_width, m_height);
}

void Widget::paintEvent(QPaintEvent*)
{
    m_drawOnce++;
    if(m_drawOnce >= 3)
    {
        m_pScene->m_enableShadow = true;
        if(m_pScene->m_enableShadow)
        {
            m_raster->clearColor(Qt::black);
            m_raster->clearDepth();
            m_raster->clearShadowMap();
            m_raster->enableShadow(true);
            m_raster->setCamera(m_pScene->getLightCamera());
            m_raster->setLight(m_pScene->m_light);

            qDebug()<<"gameobject count is "<<m_pScene->m_gameObjects.size();
            foreach (GGameObject go, m_pScene->m_gameObjects)
            {
                if(go.m_castShadow)
                {
                    m_raster->renderGameObject(go, go.m_castShadow, false);
                }
            }
        }

        m_raster->clearColor(Qt::black);
        m_raster->clearDepth();
        m_raster->enableShadow(false);
        m_raster->setCamera(m_pScene->m_camera);
        qDebug()<<"gameobject count is "<<m_pScene->m_gameObjects.size();
        foreach (GGameObject go, m_pScene->m_gameObjects)
        {
            m_raster->renderGameObject(go, false, go.m_receiveShadow);
        }

//        m_raster->renderGameObject( m_pScene->skybox2() );

        QPainter painter(this);
        painter.drawImage( QRectF(0,0,m_width, m_height), this->genImage(m_width, m_height, m_raster->frameBuffer()));
//        painter.drawImage( QRectF(0,0,m_width, m_height), this->shadowImage(m_width, m_height, m_raster->shadowMap()));

//        QPainter painter(this);
//        QImage image;
//        image.load(":/texture/uv.tga");
//        painter.drawImage( QRectF(0,0,m_width, m_height), image);
    }
}

QImage Widget::shadowImage(int width, int height, float* data)
{
    QImage image(width, height, QImage::Format_RGBA8888);

    for(int j=0; j < height; ++j)
    {
        for(int i=0; i < width; ++i)
        {
            float r = data[j*width + i];
            int c = r*255;
            image.setPixelColor(i,j, QColor(c,c,c));
        }
    }

    return image.mirrored();
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
