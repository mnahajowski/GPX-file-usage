#ifndef PHYSICS_CHARTS_GUI_PATH_SEGMENT_H
#define PHYSICS_CHARTS_GUI_PATH_SEGMENT_H

#include "path_point.h"

typedef struct {
    path_point *points;
    int points_length;
    double avg_speed;
    double min_speed;
    double max_speed;
    double total_distance;
    double total_time;
    _Bool condition_met;
} path_segment;

#endif //PHYSICS_CHARTS_GUI_PATH_SEGMENT_H
