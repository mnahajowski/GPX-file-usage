#include <math.h>
#include "latlon_convert.h"
#include "path.h"

path_point convert_latlon(double lat, double lon) {
    path_point result;
    const double r = 6371.0;
    result.x = r * cos(lat/180.0) * cos(lon/180.0);
    result.y = r * cos(lat/180.0) * sin(lon/180.0);

    return result;
}