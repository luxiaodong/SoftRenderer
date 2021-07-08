#ifndef GRASTER_H
#define GRASTER_H

#include "Render/grender.h"
#include "Render/Raster/gframebuffer.h"
#include "Render/Raster/gdepthbuffer.h"
#include <QMatrix4x4>

class GRaster : public GRender
{
public:
    GRaster();

    void createBuffer();
    virtual float* doRendering();

    void clearColor();


private:
    GFrameBuffer* m_frameBuffer;
    GDepthBuffer* m_depthBuffer;
};

#endif // GRASTER_H
