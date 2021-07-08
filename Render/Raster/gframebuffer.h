#ifndef GFRAMEBUFFER_H
#define GFRAMEBUFFER_H


class GFrameBuffer
{
public:
    GFrameBuffer(int, int, int);
    ~GFrameBuffer();

    void clearColor(float, float, float);

    int m_width;
    int m_height;
    int m_depth;

    float* m_data;
};

#endif // GFRAMEBUFFER_H
