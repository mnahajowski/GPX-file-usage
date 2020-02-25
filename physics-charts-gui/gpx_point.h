#ifndef PHYSICS_CHARTS_GUI_GPX_POINT_H
#define PHYSICS_CHARTS_GUI_GPX_POINT_H

typedef struct {
    double lat, lon;
    int valid;
    struct tm time;
} gpx_point;

#endif //PHYSICS_CHARTS_GUI_GPX_POINT_H
