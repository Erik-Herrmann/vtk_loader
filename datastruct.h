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

struct PolyLine{
    vtkPoint point;
    QDateTime time;
};

#endif // DATASTRUCT_H
