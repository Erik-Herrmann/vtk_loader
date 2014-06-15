#ifndef VTKPOINT_H
#define VTKPOINT_H

struct vtkPoint{
    vtkPoint(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    float x, y, z;
};

#endif // VTKPOINT_H
