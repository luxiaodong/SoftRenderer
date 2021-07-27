#ifndef GFRAMEBUFFER_H
#define GFRAMEBUFFER_H

#include <QColor>

class GFrameBuffer
{
public:
    GFrameBuffer(int, int, int);
    ~GFrameBuffer();

    void clearColor(QColor color);
    void setPixel(int x, int y, QColor color);
    QColor pixel(int x, int y);


    int m_width;
    int m_height;
    int m_depth;

    int* m_data;
};

#endif // GFRAMEBUFFER_H
