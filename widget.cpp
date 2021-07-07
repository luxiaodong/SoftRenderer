#include "widget.h"
#include "Render/Raster/graster.h"
#include "Scene/gscene.h"
#include "Render/gcamear.h"

#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//    m_pRender = new GRender();
    m_pRender = new GRaster();
    m_pRender->setRenderSize(QSize(100,100));
    m_pRender->setClearColor(Qt::black);

    m_pScene = new GScene();
    m_camera = new GCamear();
}

void Widget::paintEvent(QPaintEvent*)
{
    m_pScene->loadSceneTest();

    QPainter painter(this);
    painter.drawImage( QRectF(0,0,this->width(), this->height()), m_pRender->DoRendering());
//    qDebug()<<"xxx";
}

Widget::~Widget()
{
}
