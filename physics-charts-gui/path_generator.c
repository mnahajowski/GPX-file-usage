#include <math.h>
#include <glib.h>
#include <stdlib.h>
#include <printf.h>
#include "path_generator.h"
#include "latlon_convert.h"
#include "path.h"
#include "gpx_file.h"

#define CURRENT_SEGMENT result->segments[result->segments_length - 1]

double calculate_distance(path_point p1, path_point p2) {
    double dx = (p2.x - p1.x);
    double dy = (p2.y - p1.y);

    return sqrt(dx * dx + dy * dy);
}

double calculate_time(struct tm p1, struct tm p2) {
    return difftime(mktime(&p2), mktime(&p1));
}

_Bool check_condition(double speed, path_conditions conditions) {
    return speed >= conditions.min_speed && speed <= conditions.max_speed;
}

path *generate_path(const gpx_point *points, int points_length, path_conditions conditions) {
    path* result = malloc(sizeof(path));
    result->segments_length = 0;
    result->segments = NULL;

    _Bool first = TRUE;

    for (int i = 0; i < points_length - 1; i++) {
        path_point p1 = convert_latlon(points[i].lat, points[i].lon);
        path_point p2 = convert_latlon(points[i + 1].lat, points[i + 1].lon);

        double distance = calculate_distance(p1, p2);
        double time = calculate_time(points[i].time, points[i + 1].time);
        double speed = time == 0.0 ? 0.0 : distance / (time / 3600.0);

        _Bool condition_met = check_condition(speed, conditions);

        if (first || CURRENT_SEGMENT.condition_met != condition_met) {
            first = FALSE;

            result->segments_length++;
            result->segments = realloc(result->segments, result->segments_length * sizeof(path_segment));

            CURRENT_SEGMENT.condition_met = condition_met;
            CURRENT_SEGMENT.min_speed = speed;
            CURRENT_SEGMENT.max_speed = speed;
            CURRENT_SEGMENT.total_time = 0;
            CURRENT_SEGMENT.total_distance = 0;

            CURRENT_SEGMENT.points_length = 1;
            CURRENT_SEGMENT.points = malloc(sizeof(path_point));
            CURRENT_SEGMENT.points[0] = p1;
        }

        CURRENT_SEGMENT.points_length++;
        CURRENT_SEGMENT.points = realloc(CURRENT_SEGMENT.points, CURRENT_SEGMENT.points_length * sizeof(path_point));
        CURRENT_SEGMENT.points[CURRENT_SEGMENT.points_length - 1] = p2;

        if (speed > CURRENT_SEGMENT.max_speed) {
            CURRENT_SEGMENT.max_speed = speed;
        }
        if (speed < CURRENT_SEGMENT.min_speed) {
            CURRENT_SEGMENT.min_speed = speed;
        }

        CURRENT_SEGMENT.total_time += time;
        CURRENT_SEGMENT.total_distance += distance;
        CURRENT_SEGMENT.avg_speed = CURRENT_SEGMENT.total_distance / CURRENT_SEGMENT.total_time;
    }

    return result;
}