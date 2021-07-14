#include "widget.h"
#include "Scene/gscene.h"
#include "Scene/gcamear.h"
#include "Render/ggraphicsapi.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_camera = new GCamear();

    m_pScene = new GScene();
    m_pScene->loadSceneTest();

    m_graphicsApi = new GGraphicsAPI();
    m_graphicsApi->setRenderSize(QSize(100,100));
    m_graphicsApi->setClearColor(Qt::black);
    m_graphicsApi->setViewMatrix(QVector3D(0,0,0), 0, 0, 0);
    //    m_graphicsApi->setViewMatrix(QVector3D(0,0,0), QVector3D(-0.6, 0.8, 0), QVector3D(0, -5.0f/13, 12.0f/13));
    //    m_graphicsApi->setOrthMatrix(5, 1.892934, 0.3, 1000.0f);
    m_graphicsApi->setProjMatrix(60, 1.755299, 0.3, 1000.0f);
    m_graphicsApi->setViewPortMatrix(0,0,100,100);

    int i = 1;
    foreach (GGameObject go, m_pScene->m_gameObjects)
    {
        m_graphicsApi->setVertexAttribute(i, go.m_mesh);
    }

    m_graphicsApi->doRendering();
}

void Widget::paintEvent(QPaintEvent*)
{
    float* data = m_graphicsApi->doRendering();
    QPainter painter(this);
    painter.drawImage( QRectF(0,0,this->width(), this->height()), this->genImage(100, 100, data));
}

QImage Widget::genImage(int width, int height, float* data)
{
    QImage image(width, height, QImage::Format_RGBA8888);

    for(int j=0; j < height; ++j)
    {
        for(int i=0; i < width; ++i)
        {
            float r = data[(j*width + i)*3];
            float g = data[(j*width + i)*3 + 1];
            float b = data[(j*width + i)*3 + 2];
            QColor color;
            color.setRgbF(r, g, b);
            image.setPixelColor(i,j, color);
        }
    }

    return image;
}

Widget::~Widget()
{
}
