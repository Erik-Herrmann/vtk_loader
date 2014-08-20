#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QDateTime>
#include "vtkpoint.h"

enum MyTypes{
    NOTYPE = -1,
    INT_T = 0,
    UINT_T = 1,
    FLOAT_T = 2,
    DOUBLE_T = 3,
    UCHAR_T = 4,
    VTKPOINT_T = 5

};

enum FileTypes{
    UNKNOWN = -1,
    MIPAS = 0,
    AIRS = 1,
    CLAMS = 2,
    TROPO = 3
};

namespace Compair {
    enum CompairFunc{
        EQUAL = 0,
        NOT_EQUAL = 1,
        GREATER = 2,
        GREATER_EQUAL = 3,
        LESS = 4,
        LESS_EQUAL = 5
    };
}

struct Point{
    Point(){
        p[0] = p[1] = p[2] = 0.0f;
    }
    Point(float v1, float v2, float v3){
        p[0] = v1;
        p[1] = v2;
        p[2] = v3;
    }
    float& x(){
        return p[0];
    }
    float& y(){
        return p[1];
    }
    float& z(){
        return p[2];
    }
    float* data(){
        return p;
    }

    float p[3];
};

struct Quad{
    Quad(){
        q[0] = q[1] = q[2] = q[3] = 0;
    }
    Quad(Point *p1, Point *p2, Point *p3, Point *p4){
        q[0] = p1;
        q[1] = p2;
        q[2] = p3;
        q[3] = p4;
    }
    Point* a(){
        return q[0];
    }
    Point* b(){
        return q[1];
    }
    Point* c(){
        return q[2];
    }
    Point* d(){
        return q[3];
    }
    Point** data(){
        return q;
    }
    Point* q[4];
};

struct WorldCoord {
    WorldCoord(){
        longitude = 0.0f;
        latitude = 0.0f;
    }
    WorldCoord(float longitude, float latitude){
        this->longitude = longitude;
        this->latitude = latitude;
    }

    float longitude;
    float latitude;
};

struct TimeStep{
    unsigned int dataIndex;
};

struct PolyLine{
    vtkPoint point;
    QDateTime time;
};

#endif // DATASTRUCT_H
