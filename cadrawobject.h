#ifndef CADRAWOBJECT_H
#define CADRAWOBJECT_H

#include <vector>

class caDrawObject
{
public:
    explicit caDrawObject();
    virtual ~caDrawObject();

    virtual void drawObject()=0;
    virtual void drawIndexObject(std::vector<unsigned int> *indices)=0;
};

#endif // CADRAWOBJECT_H
