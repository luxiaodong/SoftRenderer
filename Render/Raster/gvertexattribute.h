#ifndef GVERTEXATTRIBUTE_H
#define GVERTEXATTRIBUTE_H

class GVertexAttribute
{
public:
    GVertexAttribute();

public:
    float* m_vertexsInObject;
    float* m_vertexsInClips;
    int m_vertexCount; //顶点个数, 浮点个数 = 3*顶点个数
};

#endif // GVERTEXATTRIBUTE_H
