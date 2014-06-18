#ifndef HELPERS_H
#define HELPERS_H

#include "vtkpoint.h"
#include "QDateTime"

namespace Helpers {
bool equal_to(int &a, int &b);
bool equal_to(unsigned int &a, unsigned int &b);
bool equal_to(float &a, float &b);
bool equal_to(double &a, double &b);
bool equal_to(unsigned char &a, unsigned char &b);
bool equal_to(vtkPoint &a, float &b);
bool equal_to(vtkPoint &a, vtkPoint &b);

bool not_equal_to(int &a, int &b);
bool not_equal_to(unsigned int &a, unsigned int &b);
bool not_equal_to(float &a, float &b);
bool not_equal_to(double &a, double &b);
bool not_equal_to(unsigned char &a, unsigned char &b);
bool not_equal_to(vtkPoint &a, float &b);
bool not_equal_to(vtkPoint &a, vtkPoint &b);

bool greater(int &a, int &b);
bool greater(unsigned int &a, unsigned int &b);
bool greater(float &a, float &b);
bool greater(double &a, double &b);
bool greater(unsigned char &a, unsigned char &b);
bool greater(vtkPoint &a, float &b);
bool greater(vtkPoint &a, vtkPoint &b);

bool greater_equal(int &a, int &b);
bool greater_equal(unsigned int &a, unsigned int &b);
bool greater_equal(float &a, float &b);
bool greater_equal(double &a, double &b);
bool greater_equal(unsigned char &a, unsigned char &b);
bool greater_equal(vtkPoint &a, float &b);
bool greater_equal(vtkPoint &a, vtkPoint &b);

bool less(int &a, int &b);
bool less(unsigned int &a, unsigned int &b);
bool less(float &a, float &b);
bool less(double &a, double &b);
bool less(unsigned char &a, unsigned char &b);
bool less(vtkPoint &a, float &b);
bool less(vtkPoint &a, vtkPoint &b);

bool less_equal(int &a, int &b);
bool less_equal(unsigned int &a, unsigned int &b);
bool less_equal(float &a, float &b);
bool less_equal(double &a, double &b);
bool less_equal(unsigned char &a, unsigned char &b);
bool less_equal(vtkPoint &a, float &b);
bool less_equal(vtkPoint &a, vtkPoint &b);

QDateTime juliantimeToDatetime(double secSince);
QDateTime juliantimeToDatetime(float secSince);

}

#endif // HELPERS_H
