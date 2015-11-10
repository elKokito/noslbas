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

    sinoscope_t sino = *ptr;
    /*int x, y, index, taylor;*/
    int x, y;
    int s_width = ptr->width;
    int s_height = ptr->height;
    /*struct rgb c;*/

    unsigned char* b = sino.buf;

    struct rgb c;
    int index, taylor;
    float val, px, py, s_dx, s_dy;
    int s_taylor;
    float s_time;
    float s_phase0;
    float s_phase1;
    int s_interval;
    float s_interval_inv;

    #pragma omp parallel for private(x, y, c, index, s_taylor, s_time, s_phase0, s_phase1, s_interval, s_interval_inv, val, px, py, s_dx, s_dy)
    for(x = 1; x < s_width; ++x){
        for(y = 1; y < s_height; ++y) {
            s_taylor = ptr->taylor;
            s_time = ptr->time;
            s_phase0 = ptr->phase0;
            s_phase1 = ptr->phase1;
            s_interval = ptr->interval;
            s_interval_inv = ptr->interval_inv;
            s_dx = ptr->dx;
            s_dy = ptr->dy;
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
                b[index + 0] = c.r;
                b[index + 1] = c.g;
                b[index + 2] = c.b;
            }
            /*if(x == 1 && y == 1){*/
            /*printf("here\n");*/
            /*printf("index = %d\t\t b[%d] = %u\n", index, index + 0, c.r);*/
            /*printf("index = %d\t\t b[%d] = %u\n", index, index + 1, c.g);*/
            /*printf("index = %d\t\t b[%d] = %u\n", index, index + 2, c.b);*/
            /*}*/
        }
    }

    return 0;
}
