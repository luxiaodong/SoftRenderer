#ifndef GMATERIAL_H
#define GMATERIAL_H

#include "Shader/gshader.h"

class GMaterial
{
public:
    GMaterial();

// 各种贴图.
    GShader* m_pShader;
};

#endif // GMATERIAL_H
