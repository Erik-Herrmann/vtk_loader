#include "helpers.h"

namespace Helpers{

bool equal_to(int &a, int &b){
    return a == b;
}

bool equal_to(unsigned int &a, unsigned int &b){
    return a == b;
}

bool equal_to(float &a, float &b){
    return a == b;
}

bool equal_to(double &a, double &b){
    return a == b;
}

bool equal_to(unsigned char &a, unsigned char &b){
    return a == b;
}

bool equal_to(vtkPoint &a, float &b){
    return (a.x == b) &&
            (a.y == b) &&
            (a.z == b);
}

bool equal_to(vtkPoint &a, vtkPoint &b){
    return (a.x == b.x) &&
            (a.y == b.y) &&
            (a.z == b.z);
}

bool not_equal_to(int &a, int &b){
    return a != b;
}

bool not_equal_to(unsigned int &a, unsigned int &b){
    return a != b;
}

bool not_equal_to(float &a, float &b){
    return a != b;
}

bool not_equal_to(double &a, double &b){
    return a != b;
}

bool not_equal_to(unsigned char &a, unsigned char &b){
    return a != b;
}

bool not_equal_to(vtkPoint &a, float &b){
    return (a.x != b) &&
            (a.y != b) &&
            (a.z != b);
}

bool not_equal_to(vtkPoint &a, vtkPoint &b){
    return (a.x != b.x) &&
            (a.y != b.y) &&
            (a.z != b.z);
}


bool greater(int &a, int &b){
    return a > b;
}

bool greater(unsigned int &a, unsigned int &b){
    return a > b;
}

bool greater(float &a, float &b){
    return a > b;
}

bool greater(double &a, double &b){
    return a > b;
}

bool greater(unsigned char &a, unsigned char &b){
    return a > b;
}

bool greater(vtkPoint &a, float &b){
    return (a.x > b) &&
            (a.y > b) &&
            (a.z > b);
}

bool greater(vtkPoint &a, vtkPoint &b){
    return (a.x > b.x) &&
            (a.y > b.y) &&
            (a.z > b.z);
}

bool greater_equal(int &a, int &b){
    return a >= b;
}

bool greater_equal(unsigned int &a, unsigned int &b){
    return a >= b;
}

bool greater_equal(float &a, float &b){
    return a >= b;
}

bool greater_equal(double &a, double &b){
    return a >= b;
}

bool greater_equal(unsigned char &a, unsigned char &b){
    return a >= b;
}

bool greater_equal(vtkPoint &a, float &b){
    return (a.x >= b) &&
            (a.y >= b) &&
            (a.z >= b);
}

bool greater_equal(vtkPoint &a, vtkPoint &b){
    return (a.x >= b.x) &&
            (a.y >= b.y) &&
            (a.z >= b.z);
}


bool less(int &a, int &b){
    return a < b;
}

bool less(unsigned int &a, unsigned int &b){
    return a < b;
}

bool less(float &a, float &b){
    return a < b;
}

bool less(double &a, double &b){
    return a < b;
}

bool less(unsigned char &a, unsigned char &b){
    return a < b;
}

bool less(vtkPoint &a, float &b){
    return (a.x < b) &&
            (a.y < b) &&
            (a.z < b);
}


bool less(vtkPoint &a, vtkPoint &b){
    return (a.x < b.x) &&
            (a.y < b.y) &&
            (a.z < b.z);
}


bool less_equal(int &a, int &b){
    return a <= b;
}

bool less_equal(unsigned int &a, unsigned int &b){
    return a <= b;
}

bool less_equal(float &a, float &b){
    return a <= b;
}

bool less_equal(double &a, double &b){
    return a <= b;
}

bool less_equal(unsigned char &a, unsigned char &b){
    return a <= b;
}

bool less_equal(vtkPoint &a, float &b){
    return (a.x <= b) &&
            (a.y <= b) &&
            (a.z <= b);
}

bool less_equal(vtkPoint &a, vtkPoint &b){
    return (a.x <= b.x) &&
            (a.y <= b.y) &&
            (a.z <= b.z);
}

}
