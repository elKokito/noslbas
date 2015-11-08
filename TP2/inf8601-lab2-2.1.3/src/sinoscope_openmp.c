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
    int x, y, index, taylor;
    struct rgb c;
    float val, px, py;
    unsigned char* b = sino.buf;

    /*printf("width: %d\n", sino.width);*/
    /*printf("height: %d\n", sino.height);*/
    /*printf("taylor: %d\n", sino.taylor);*/
    /*printf("time: %f\n", sino.time);*/

    #pragma omp parallel for private(x, y, c)
    for(x = 1; x < sino.width; ++x){
        for(y = 1; y < sino.height; ++y) {
            px = sino.dx * y - 2 * M_PI;
            py = sino.dy * x - 2 * M_PI;
            val = 0.0f;
            for (taylor = 1; taylor <= sino.taylor; taylor += 2) {
                val += sin(px * taylor * sino.phase1 + sino.time) / taylor + cos(py * taylor * sino.phase0) / taylor;
            }
            val = (atan(1.0 * val) - atan(-1.0 * val)) / (M_PI);
            val = (val + 1) * 100;
            value_color(&c, val, sino.interval, sino.interval_inv);
            index = (y * 3) + (x * 3) * sino.width;
            b[index + 0] = c.r;
            b[index + 1] = c.g;
            b[index + 2] = c.b;
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
