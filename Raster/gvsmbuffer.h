#ifndef GVSMBUFFER_H
#define GVSMBUFFER_H

// Variance Shadow mapping 使用的buffer
class GVSMBuffer
{
public:
    GVSMBuffer(int, int);
    ~GVSMBuffer();

    void clear(float depth = 0.0f);
    void setDepth(int x, int y, float depth);
    float depth(int x, int y);
    float depthAverage(int x, int y);
    float depthSquare(int x, int y);
    void genAverageDepth();
    void setAverageDepth(int x, int y, int range);

    //切比雪夫不等式
    float chebyshev(int x, int y, float t);

    int m_width;
    int m_height;
    int m_radius;

    float* m_depth;
    float* m_depthSquare;
    float* m_depthAverage;
    float* m_depthSquareAverage;
};

#endif // GVSMBUFFER_H
