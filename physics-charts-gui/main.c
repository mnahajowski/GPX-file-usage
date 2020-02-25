#include <stdlib.h>
#include <math.h>
#include <slope/slope.h>
#include <memory.h>

#include "gpx_parser.h"
#include "path_generator.h"

typedef struct {
    /* gpx_data */
    gpx_data *gpx_data;

    /* path */
    path *path;

    /* chart_path */
    double **chart_path_x;
    double **chart_path_y;
    _Bool *chart_path_met_condition;
    int *chart_path_n;
    int chart_path_amount;

    /* window */
    GtkWidget *window;

    /* grid */
    GtkWidget *grid;

    /* options */
    GtkWidget *options_min_speed;
    GtkWidget *options_max_speed;
    GtkWidget *options_refresh;

    /* slope_chart */
    GtkWidget *slope_chart;
    SlopeXyScale *slope_chart_scale;
    SlopeItem **slope_chart_items;
    int slope_chart_items_amount;

    /* stats */
    GtkWidget *stat_scroll;
    GtkWidget *stat_label;
} main_data;

void free_gpx_data(gpx_data *gpx_data) {
    if (gpx_data == NULL) {
        return;
    }
    free(gpx_data->points);
}

void refresh_gpx_data(char *gpx_file, gpx_data **gpx_data) {
    free_gpx_data(*gpx_data);

    *gpx_data = parse_gpx(gpx_file);
}

void free_path(path *path) {
    if (path == NULL) {
        return;
    }
    for (int i = 0; i < path->segments_length; i++) {
        free(path->segments[i].points);
    }
    free(path->segments);
    free(path);
}

void refresh_path(path **path, const gpx_data *data, const double min_speed, const double max_speed) {
    free_path(*path);

    path_conditions path_conditions1 = {0};
    path_conditions1.min_speed = min_speed;
    path_conditions1.max_speed = max_speed;
    *path = generate_path(data->points, data->points_length, path_conditions1);
}

void free_chart_path(double **chart_path_x, double **chart_path_y, _Bool *chart_path_met_condition, int *chart_path_n,
                     int chart_path_amount) {
    if (chart_path_x == NULL) {
        return;
    }
    for (int i = 0; i < chart_path_amount; i++) {
        g_free(chart_path_x[i]);
        g_free(chart_path_y[i]);
    }

    g_free(chart_path_x);
    g_free(chart_path_y);
    g_free(chart_path_met_condition);
    g_free(chart_path_n);
}

void
refresh_chart_path(path *path, double ***chart_path_x, double ***chart_path_y, _Bool **chart_path_met_condition,
                   int **chart_path_n, int *chart_path_amount) {
    free_chart_path(*chart_path_x, *chart_path_y, *chart_path_met_condition, *chart_path_n, *chart_path_amount);

    *chart_path_amount = path->segments_length;
    *chart_path_n = malloc(*chart_path_amount * sizeof(int));
    *chart_path_met_condition = malloc(*chart_path_amount * sizeof(_Bool));
    *chart_path_x = malloc(*chart_path_amount * sizeof(double *));
    *chart_path_y = malloc(*chart_path_amount * sizeof(double *));

    for (int j = 0; j < path->segments_length; j++) {
        (*chart_path_n)[j] = path->segments[j].points_length;
        (*chart_path_met_condition)[j] = path->segments[j].condition_met;
        (*chart_path_x)[j] = malloc((*chart_path_n)[j] * sizeof(double));
        (*chart_path_y)[j] = malloc((*chart_path_n)[j] * sizeof(double));

        for (int i = 0; i < (*chart_path_n)[j]; i++) {
            (*chart_path_x)[j][i] = path->segments[j].points[i].x;
            (*chart_path_y)[j][i] = path->segments[j].points[i].y;
        }
    }
}

void free_slope_chart(SlopeXyScale *slope_chart_scale, SlopeItem **slope_chart_items, int slope_chart_items_amount) {
    for (int i = 0; i < slope_chart_items_amount; i++) {
        slope_scale_remove_item((SlopeScale *) slope_chart_scale, slope_chart_items[i]);
    }
    free(slope_chart_items);
}

void
refresh_slope_chart(SlopeXyScale *slope_chart_scale, SlopeItem ***slope_chart_items, int *slope_chart_items_amount,
                    double **chart_path_x,
                    double **chart_path_y, _Bool *chart_path_met_condition, int *chart_path_n,
                    int chart_path_amount) {
    free_slope_chart(slope_chart_scale, *slope_chart_items, *slope_chart_items_amount);

    *slope_chart_items_amount = chart_path_amount;
    *slope_chart_items = malloc((*slope_chart_items_amount) * sizeof(SlopeItem *));

    double min_x = chart_path_x[0][0];
    double max_x = chart_path_x[0][0];
    double min_y = chart_path_y[0][0];
    double max_y = chart_path_y[0][0];

    for (int j = 0; j < (*slope_chart_items_amount); j++) {
        for (int i = 0; i < chart_path_n[j]; i++) {
            if (chart_path_x[j][i] < min_x) {
                min_x = chart_path_x[j][i];
            }
            if (chart_path_x[j][i] > max_x) {
                max_x = chart_path_x[j][i];
            }
            if (chart_path_y[j][i] < min_y) {
                min_y = chart_path_y[j][i];
            }
            if (chart_path_y[j][i] > max_y) {
                max_y = chart_path_y[j][i];
            }
        }

        if (chart_path_met_condition[j]) {
            (*slope_chart_items)[j] = slope_xyseries_new_filled(NULL, chart_path_x[j], chart_path_y[j], chart_path_n[j],
                                                                "g-");
        } else {
            (*slope_chart_items)[j] = slope_xyseries_new_filled(NULL, chart_path_x[j], chart_path_y[j], chart_path_n[j],
                                                                "r-");
        }
        slope_scale_add_item((SlopeScale *) slope_chart_scale, (*slope_chart_items)[j]);
    }

    slope_xyscale_set_x_range(slope_chart_scale, min_x, max_x);
    slope_xyscale_set_y_range(slope_chart_scale, min_y, max_y);
}

char *join_str(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);

    if (result) {
        strcpy(result, s1);
        strcat(result, s2);
    }

    return result;
}

void options_refresh_click(GtkWidget *widget,
                           gpointer g_data) {
    main_data *data = (main_data *) g_data;

    const char *min_text = gtk_entry_get_text((GtkEntry *) data->options_min_speed);
    const char *max_text = gtk_entry_get_text((GtkEntry *) data->options_max_speed);

    char *temp;

    double min = strtod(min_text, &temp);
    double max = strtod(max_text, &temp);

    refresh_path(&(data->path), data->gpx_data, min, max);

    refresh_chart_path(data->path, &(data->chart_path_x), &(data->chart_path_y), &(data->chart_path_met_condition),
                       &(data->chart_path_n),
                       &(data->chart_path_amount));

    refresh_slope_chart(data->slope_chart_scale, &(data->slope_chart_items), &(data->slope_chart_items_amount),
                        data->chart_path_x, data->chart_path_y,
                        data->chart_path_met_condition, data->chart_path_n, data->chart_path_amount);

    slope_chart_redraw((SlopeChart *) data->slope_chart);

    char *l = "no.\t \t time[s]  \t distance[km]\t min speed[km/h]\t max speed[km/h] \t avg speed[km/h]\n";

    for (int i = 0; i < data->path->segments_length; i++) {
        char b[150];
        if(data->path->segments[i].total_time >=100.0)  {
          sprintf(b, "%.4d \t %.2f \t \t %-30.2f \t %-25.2f \t %-32.2f \t %-20.2f \n", i + 1,
                  data->path->segments[i].total_time,
                  data->path->segments[i].total_distance,
                  data->path->segments[i].min_speed,
                  data->path->segments[i].max_speed,
                  data->path->segments[i].avg_speed*3600);
        }
        else  {
          sprintf(b, "%.4d \t %.2f \t\t\t %-30.2f \t %-25.2f \t %-32.2f \t %-20.2f \n", i + 1,
                  data->path->segments[i].total_time,
                  data->path->segments[i].total_distance,
                  data->path->segments[i].min_speed,
                  data->path->segments[i].max_speed,
                  data->path->segments[i].avg_speed*3600);
        }
        l = join_str(l, b);
    }

    gtk_label_set_text((GtkLabel *) data->stat_label, l);
}

int main(int argc, char *argv[]) {
    main_data *data = malloc(sizeof(main_data));

    refresh_gpx_data(argv[1], &(data->gpx_data));

    gtk_init(&argc, &argv);

    data->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (data->window), "Physics Charts");
    g_signal_connect(G_OBJECT(data->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    data->grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER (data->window), data->grid);

    data->options_min_speed = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(data->grid), gtk_label_new("Min speed [km/h]"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(data->grid), data->options_min_speed, 1, 0, 1, 1);
    gtk_entry_set_text((GtkEntry *) data->options_min_speed, "0,0");

    data->options_max_speed = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(data->grid), gtk_label_new("Max speed [km/h]"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(data->grid), data->options_max_speed, 1, 1, 1, 1);
    gtk_entry_set_text((GtkEntry *) data->options_max_speed, "0,0");

    data->options_refresh = gtk_button_new_with_label("Refresh");
    gtk_grid_attach(GTK_GRID(data->grid), data->options_refresh, 0, 2, 2, 1);
    g_signal_connect(G_OBJECT(data->options_refresh), "clicked", G_CALLBACK(options_refresh_click), data);

    data->stat_scroll = gtk_scrolled_window_new(NULL, NULL);
    data->stat_label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(data->stat_scroll), data->stat_label);
    gtk_grid_attach(GTK_GRID(data->grid), data->stat_scroll, 0, 3, 3, 10);
    gtk_widget_set_size_request(data->stat_scroll, 600, 300);


    data->slope_chart = slope_chart_new();
    g_signal_connect(G_OBJECT(data->slope_chart), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    data->slope_chart_scale = (SlopeXyScale *) slope_xyscale_new();

    slope_chart_add_scale(SLOPE_CHART(data->slope_chart), (SlopeScale *) data->slope_chart_scale);

    options_refresh_click(NULL, data);

    /*refresh_path(&(data->path), data->gpx_data, -0.1, 999999.0);

    refresh_chart_path(data->path, &(data->chart_path_x), &(data->chart_path_y), &(data->chart_path_met_condition),
                       &(data->chart_path_n),
                       &(data->chart_path_amount));

    refresh_slope_chart(data->slope_chart_scale, &(data->slope_chart_items), &(data->slope_chart_items_amount),
                        data->chart_path_x, data->chart_path_y,
                        data->chart_path_met_condition, data->chart_path_n, data->chart_path_amount);*/

    gtk_widget_show_all(data->window);
    gtk_widget_show_all(data->slope_chart);
    gtk_main();

    free_path(data->path);
    free_chart_path(data->chart_path_x, data->chart_path_y, data->chart_path_met_condition, data->chart_path_n,
                    data->chart_path_amount);
    free_slope_chart(data->slope_chart_scale, data->slope_chart_items, data->slope_chart_items_amount);

    return 0;
}
