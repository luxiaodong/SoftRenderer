#include "widget.h"
#include "Scene/gscene.h"
#include "Scene/gcamear.h"
#include "Render/ggraphicsapi.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_width = 649;
    m_height = m_width;

    m_camera = new GCamear();
    m_pScene = new GScene();
    m_pScene->loadSceneTest();

    m_graphicsApi = new GGraphicsAPI();
    m_graphicsApi->setRenderSize(QSize(m_width, m_height));
    m_graphicsApi->setViewMatrix(QVector3D(0, 10, -10), 45, 0, 0);
    //    m_graphicsApi->setViewMatrix(QVector3D(0,0,0), QVector3D(-0.6, 0.8, 0), QVector3D(0, -5.0f/13, 12.0f/13));
    //    m_graphicsApi->setOrthMatrix(5, 1.892934, 0.3, 1000.0f);
    m_graphicsApi->setProjMatrix(60, 1, 0.3f, 1000.0f);
    m_graphicsApi->setViewPortMatrix(0,0,m_width,m_height);

    int i = 1;
    foreach (GGameObject go, m_pScene->m_gameObjects)
    {
        m_graphicsApi->setModelMatrix(go.m_position, go.m_rotate, go.m_scale);
        m_graphicsApi->setVertexAttribute(i++, go.m_mesh);
    }

    m_drawOnce = 0;
    this->resize(600,600);
//    m_graphicsApi->doRendering();
}

void Widget::paintEvent(QPaintEvent*)
{
    m_drawOnce++;
    if(m_drawOnce == 3)
    {
        m_graphicsApi->setClearColor(Qt::black);
        m_graphicsApi->setClearDepth();

        int* data = m_graphicsApi->doRendering();
        QPainter painter(this);
        painter.drawImage( QRectF(0,0,this->width(), this->height()), this->genImage(m_width, m_height, data));
    }
}

QImage Widget::genImage(int width, int height, int* data)
{
    QImage image(width, height, QImage::Format_RGBA8888);

    for(int j=0; j < height; ++j)
    {
        for(int i=0; i < width; ++i)
        {
            int r = data[((height-1 -j)*width + i)*3];
            int g = data[((height-1 -j)*width + i)*3 + 1];
            int b = data[((height-1 -j)*width + i)*3 + 2];
            image.setPixelColor(i,j, QColor(r,g,b));
        }
    }

    return image;
}

Widget::~Widget()
{
}
