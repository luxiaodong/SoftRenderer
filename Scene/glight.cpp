#include "glight.h"

GLight::GLight()
{

}

QVector3D GLight::dir()
{
    QMatrix4x4 mat = this->objectToWorldMatrix();
    return -mat.column(2).toVector3D().normalized();
}
