#ifndef GRASTER_H
#define GRASTER_H

#include "Render/grender.h"

class GRaster : public GRender
{
public:
    GRaster();

    virtual QImage DoRendering();
};

#endif // GRASTER_H
