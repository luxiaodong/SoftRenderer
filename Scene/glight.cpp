#include "glight.h"

GLight::GLight()
{

}

QVector3D GLight::dir()
{
    return QVector3D(0,0,1);
//    QMatrix4x4 mat = this->objectToWorldMatrix();
//    return -mat.column(2).toVector3D().normalized();
}
