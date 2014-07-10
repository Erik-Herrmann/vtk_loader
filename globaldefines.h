#ifndef GLOBALDEFINES_H
#define GLOBALDEFINES_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif
#ifndef M_1_PI
#define M_1_PI 0.318309886183790671538
#endif
#ifndef M_2_PI
#define M_2_PI 0.636619772367581343076
#endif

#define WORLD_SPHERE_RADIUS 10
#define EARTH_RADIUS 6371.000

// Island
#define VOLCANO_GRIMSVOTN_LATITUDE 64.42       // 64.42°N
#define VOLCANO_GRIMSVOTN_LONGITUDE -17.33     // 17.33°W
// Chile
#define VOLCANO_PUYEHUE_LATITUDE -40.59        // 40.59°S
#define VOLCANO_PUYEHUE_LONGITUDE -72.117      // 72.117°W
// Eritrea
#define VOLCANO_NABRO_LATITUDE 13.37           // 13.37°N
#define VOLCANO_NABRO_LONGITUDE 41.7           // 41.7°E

#define THETA(X) (float)X/180.0*M_PI-M_PI_2     // point.y(-90|90) -> -PI/2 | PI/2 (latitude)
#define PHI(X) (float)X/180.0*M_PI+M_PI         // point.x(-180|180) -> 0 | 2PI    (longitude)

/*
  Conversion londitude, latitude, altitude -> x, y, z
    x = alti*sin(THETA(i))*sin(PHI(k))
    y = alti*cos(THETA(i))
    z = alti*sin(THETA(i))*cos(PHI(k))
*/

#endif // GLOBALDEFINES_H
