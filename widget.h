#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Scene/gscene.h"
#include "Scene/gcamera.h"
#include "Raster/graster.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent* );

private:
    void softRneder();
    QImage genImage(int width, int height, int* data);
    QImage shadowImage(int width, int height, float* data);

    void configScene1();
    void configScene2();

    void testSH();

private:
    int m_width;
    int m_height;
    GScene* m_pScene;
    GRaster* m_raster;
    int m_drawOnce;
};

#endif // WIDGET_H
