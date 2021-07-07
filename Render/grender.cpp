#include "grender.h"

GRender::GRender()
{

}

QImage GRender::DoRendering()
{
    QImage image(m_size, QImage::Format_RGBA8888);
    image.fill(m_color);
    return image;
}
