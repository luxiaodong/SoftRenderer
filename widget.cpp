#include "widget.h"
#include "Scene/gscene.h"
#include "Scene/gcamera.h"
#include "Math/gsh.h"
#include "Model/gmipmap.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_width = 600;
    m_height = 600;
//    configScene1();
    //configScene2();
    configScene3();

    m_raster = new GRaster();
    m_raster->setRenderSize(QSize(m_width, m_height));

    m_drawOnce = 0;
    this->resize(m_width, m_height);
}

void Widget::configScene1()
{
    GCamera* pCamera = new GCamera();
//    pCamera->setViewMatrix(QVector3D(0,0,0), QVector3D(-0.6, 0.8, 0), QVector3D(0, -5.0f/13, 12.0f/13));
    pCamera->setViewMatrix(QVector3D(0, 10, -10), 45, 0, 0);
//    pCamera->setOrthMatrix(5,  m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setProjMatrix(60, m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setViewPortMatrix(0,0,m_width,m_height);
//    pCamera->testMatrix();

    m_pScene = new GScene();
    m_pScene->m_camera = pCamera;
//    m_pScene->loadTriangle();
    m_pScene->loadPlane();
//    m_pScene->loadModel();
//    m_pScene->loadSphere();
}

void Widget::configScene2()
{
    GCamera* pCamera = new GCamera();
    pCamera->setViewMatrix(QVector3D(0, 10, -10), 45, 0, 0);
    pCamera->setProjMatrix(60, m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setViewPortMatrix(0,0,m_width,m_height);

    m_pScene = new GScene();
    m_pScene->m_camera = pCamera;
//    m_pScene->loadSHScene();
    m_pScene->loadBRDFScene();
}

void Widget::configScene3()
{
    GCamera* pCamera = new GCamera();
    pCamera->setViewMatrix(QVector3D(0, 2, -10), 30, 0, 0);
    pCamera->setProjMatrix(60, m_width*1.0f/m_height, 0.3f, 1000.0f);
    pCamera->setViewPortMatrix(0,0,m_width,m_height);

    m_pScene = new GScene();
    m_pScene->m_camera = pCamera;
    m_pScene->loadBigPlane();
}

void Widget::paintEvent(QPaintEvent*)
{
    m_drawOnce++;
    if(m_drawOnce >= 1)
    {
        this->softRneder();
//        this->testSH();
//        this->testMipmap();
    }
}

void Widget::softRneder()
{
//        m_pScene->m_enableShadow = true;
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
                m_raster->renderGameObject(go);
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
        m_raster->renderGameObject(go, m_pScene->m_enableShadow && go.m_receiveShadow);
    }

    if(m_raster->isTileBased())
    {
        m_raster->tileBasedRendering(2);
    }
    else
    {
//        m_raster->renderGameObject( m_pScene->skybox2() );
    }

    QPainter painter(this);
    painter.drawImage( QRectF(0,0,m_width, m_height), this->genImage(m_width, m_height, m_raster->frameBuffer()));
//        painter.drawImage( QRectF(0,0,m_width, m_height), this->shadowImage(m_width, m_height, m_raster->shadowMap()));

//        QPainter painter(this);
//        QImage image;
//        image.load(":/texture/uv.tga");
//        painter.drawImage( QRectF(0,0,m_width, m_height), image);
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

void Widget::testSH()
{
    GSH sh;
    sh.test();
    QImage image = QImage(":/texture/sh.jpg", "jpg");
    QImage origin = image.copy(0, 0, image.width(), image.height()/2);
    QImage imageSH = sh.testImage(origin);
    QPainter painter(this);
    painter.drawImage(QRectF(0, 0, m_width, m_height/2), origin);
    painter.drawImage(QRectF(0, m_height/2, m_width, m_height/2), imageSH);
}

void Widget::testMipmap()
{
    this->resize(1024,1024);
//    QImage image = QImage(":/texture/rgb.png", "png");
    GMipmap mipmap = GMipmap(":/texture/grid.png");
    QPainter painter(this);
    mipmap.test();
    painter.drawImage(QPoint(0,0), mipmap.m_target);
}

Widget::~Widget()
{
}
