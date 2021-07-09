#ifndef GVERTEXATTRIBUTEBUFFER_H
#define GVERTEXATTRIBUTEBUFFER_H

class GVertexAttributeBuffer
{
public:
    GVertexAttributeBuffer();

public:
    float* m_vertexs;
    int m_vertexCount; //顶点个数, 浮点个数 = 3*顶点个数
};

#endif // GVERTEXATTRIBUTEBUFFER_H
