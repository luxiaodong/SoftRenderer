#ifndef GDEPTHBUFFER_H
#define GDEPTHBUFFER_H


class GDepthBuffer
{
public:
    GDepthBuffer(int, int);
    ~GDepthBuffer();

    void clear();

    void setDepth(int x, int y, float depth);
    float depth(int x, int y);

    int m_width;
    int m_height;

    float* m_data;
};

#endif // GDEPTHBUFFER_H
