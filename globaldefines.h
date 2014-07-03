#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H


#define WORLD_SPHERE_RADIUS 10
#define EARTH_RADIUS 6371.000
#define THETA(X) (float)X/180.0*M_PI-M_PI_2     // point.y(-90|90) -> -PI/2 | PI/2
#define PHI(X) (float)X/180.0*M_PI+M_PI         // point.x(-180|180) -> 0 | 2PI

/*
  Conversion londitude, latitude, altitude -> x, y, z
    x = alti*sin(THETA(i))*sin(PHI(k))
    y = alti*cos(THETA(i))
    z = alti*sin(THETA(i))*cos(PHI(k))
*/

#endif // GLOBALDEFINES_H
