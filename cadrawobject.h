#ifndef CADRAWOBJECT_H
#define CADRAWOBJECT_H

#include <vector>

enum DrawObjectTypes {
    DrawObject_PointsMIPAS = 0,
    DrawObject_QuadSpheres = 1
};

class caDrawObject
{
public:
    explicit caDrawObject(DrawObjectTypes type);
    virtual ~caDrawObject();

    virtual void drawObject()=0;
    virtual void drawIndexObject(std::vector<unsigned int> *indices)=0;

protected:
    DrawObjectTypes m_TypeOfObject;
};

#endif // CADRAWOBJECT_H
