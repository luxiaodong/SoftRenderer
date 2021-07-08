#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Scene/gscene.h"
#include "Scene/gcamear.h"
#include "Render/ggraphicsapi.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent* );

private:
    QImage genImage(int width, int height, float* data);

private:
    GScene* m_pScene;
    GCamear* m_camera;
    GGraphicsAPI* m_graphicsApi;
};

#endif // WIDGET_H
