#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Scene/gscene.h"
#include "Render/grender.h"
#include "Render/gcamear.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent* );

private:
    GRender* m_pRender;
    GScene* m_pScene;
    GCamear* m_camera;
};

#endif // WIDGET_H
