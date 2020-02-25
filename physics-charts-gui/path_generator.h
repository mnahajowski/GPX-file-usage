#include "path.h"
#include "path_conditions.h"
#include "gpx_point.h"

#ifndef PHYSICS_CHARTS_GUI_PATH_GENERATOR_H
#define PHYSICS_CHARTS_GUI_PATH_GENERATOR_H

path *generate_path(const gpx_point *points, int points_length, path_conditions conditions);

#endif //PHYSICS_CHARTS_GUI_PATH_GENERATOR_H
