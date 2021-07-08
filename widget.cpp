#include "widget.h"
#include "Scene/gscene.h"
#include "Scene/gcamear.h"
#include "Render/ggraphicsapi.h"
#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_pScene = new GScene();
    m_camera = new GCamear();
    m_graphicsApi = new GGraphicsAPI();
    m_graphicsApi->setRenderSize(QSize(100,100));
}

void Widget::paintEvent(QPaintEvent*)
{
    m_pScene->loadSceneTest();
    m_graphicsApi->setClearColor(Qt::red);

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
