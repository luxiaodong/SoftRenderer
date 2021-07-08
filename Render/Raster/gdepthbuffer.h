#ifndef GDEPTHBUFFER_H
#define GDEPTHBUFFER_H


class GDepthBuffer
{
public:
    GDepthBuffer(int, int);
    ~GDepthBuffer();

    int m_width;
    int m_height;

    float* m_data;
};

#endif // GDEPTHBUFFER_H
