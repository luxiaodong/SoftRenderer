#ifndef GMIPMAP_H
#define GMIPMAP_H

#include <QImage>
#include <QString>

class GMipmap
{
public:
    GMipmap();
    GMipmap(QString filePath);

public:
    void test();

public:
    QColor color(float u, float v, float ratioX, float ratioY);

public:
    float switchLod(float ratio);
    QRect areaByLod(int lodX, int lodY);

private:
    void generate();
    QColor color(float u, float v);
    QColor color(float u, float v, float level);

public:
    QImage m_source; //原始图像
    QImage m_target; //生成的mipmap图像
    int m_level; // 0 - n, 0表示原图
};

#endif // GMIPMAP_H
