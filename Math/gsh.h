#ifndef GSH_H
#define GSH_H

#include <QList>
#include <QVector3D>
#include <QVector2D>
#include <QImage>

class GSH
{
public:
    GSH();

    //测试函数
    QImage testImage(QImage& image);

    //利用系数重建图片
    QImage rebuildImage(QList<QVector3D>& coffs, int width, int height);

    //核心函数,计算系数
    QList<QVector3D> calculateCoff(int order, QImage& image);
    float evalSH(QList<float>& coffs, QVector3D n);
    float evalSH(QList< QList<float> >& coffss, QVector3D n);
    QVector2D normalToPhiTheta(QVector3D n);
    QVector3D phiThetaToNormal(QVector2D n);
    int indexSH(int m, int l);

    //球谐函数基函数
    float baseSH(int m, int l, QVector3D n);
    float baseSH_00(QVector3D& n);

    float baseSH_10(QVector3D& n);
    float baseSH_11(QVector3D& n);
    float baseSH_12(QVector3D& n);

    float baseSH_20(QVector3D& n);
    float baseSH_21(QVector3D& n);
    float baseSH_22(QVector3D& n);
    float baseSH_23(QVector3D& n);
    float baseSH_24(QVector3D& n);

    float baseSH_30(QVector3D& n);
    float baseSH_31(QVector3D& n);
    float baseSH_32(QVector3D& n);
    float baseSH_33(QVector3D& n);
    float baseSH_34(QVector3D& n);
    float baseSH_35(QVector3D& n);
    float baseSH_36(QVector3D& n);

    float baseSH_40(QVector3D& n);
    float baseSH_41(QVector3D& n);
    float baseSH_42(QVector3D& n);
    float baseSH_43(QVector3D& n);
    float baseSH_44(QVector3D& n);
    float baseSH_45(QVector3D& n);
    float baseSH_46(QVector3D& n);
    float baseSH_47(QVector3D& n);
    float baseSH_48(QVector3D& n);
};

#endif // GSH_H
