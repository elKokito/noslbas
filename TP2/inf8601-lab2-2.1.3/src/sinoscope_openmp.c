/*
 * sinoscope_openmp.c
 *
 *  Created on: 2011-10-14
 *      Author: francis
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "sinoscope.h"
#include "color.h"
#include "util.h"

int sinoscope_image_openmp(sinoscope_t *ptr)
{
    /*TODO("sinoscope_image_openmp");*/

    if (ptr == NULL)
        return -1;

    struct rgb c;
    int index, taylor, x, y;
    float val, px, py;
    const int s_width = ptr->width;
    const int s_height = ptr->height;
    const float s_dx = ptr->dx, s_dy= ptr->dy;
    const int s_taylor = ptr->taylor;
    const float s_time = ptr->time;
    const float s_phase0 = ptr->phase0;
    const float s_phase1 = ptr->phase1;
    const int s_interval = ptr->interval;
    const float s_interval_inv = ptr->interval_inv;

    #pragma omp parallel for private(x, y, c, index, val, px, py, )
    for(x = 1; x < s_width; ++x){
        for(y = 1; y < s_height; ++y) {
            px = s_dx * y - 2 * M_PI;
            py = s_dy * x - 2 * M_PI;
            val = 0.0f;
            for (taylor = 1; taylor <= s_taylor; taylor += 2) {
                /*val += sin(px * taylor * s_phase1 + s_time) / taylor + cos(py * taylor * s_phase0) / taylor;*/
                val += (sin(px * taylor * s_phase1 + s_time) + cos(py * taylor * s_phase0)) / taylor;
            }
            val = (atan(1.0 * val) - atan(-1.0 * val)) / (M_PI);
            val = (val + 1) * 100;
            value_color(&c, val, s_interval, s_interval_inv);
            index = (y * 3) + (x * 3) * s_width;
            #pragma omp critical
            {
                ptr->buf[index + 0] = c.r;
                ptr->buf[index + 1] = c.g;
                ptr->buf[index + 2] = c.b;
            }
        }
    }

    return 0;
}
