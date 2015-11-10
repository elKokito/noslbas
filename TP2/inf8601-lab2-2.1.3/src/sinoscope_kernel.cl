/*
 * bilinear_kernel.cl
 *
 *  Created on: 2011-10-14
 *      Author: francis
 */

/*
 * FIXME: Determinez les arguments a passer au noyau
 *        Le code de la fonction value_color doit etre recopie dans ce fichier et adapte
 *
 *        Pour traiter des type de donnees char, le pragma suivant doit etre utilise:
 *        #pragma OPENCL EXTENSION cl_khr_byte_addressable_store: enable
 */

#ifndef M_PI
#define M_PI 3.14159265358979323846264338328
#endif

typedef struct sinoscope sinoscope_t;

struct sinoscope {
	unsigned char *buf;
	int width;
	int height;
	int interval;
	int taylor;
	float interval_inv;
	float time;
	float max;
	float phase0;
	float phase1;
	float dx;
	float dy;
};

struct rgb {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

__constant struct rgb black = { .r = 0, .g = 0, .b = 0 };
__constant struct rgb white = { .r = 255, .g = 255, .b = 255 };

void value_color(struct rgb *color, float value, int interval, float interval_inv)
{
	#pragma OPENCL EXTENSION cl_khr_byte_addressable_store: enable
	if (isnan(value)) {
		*color = black;
		return;
	}
	struct rgb c;
	int x = (((int)value % interval) * 255) * interval_inv;
	int i = value * interval_inv;
	switch(i) {
	case 0:
		c.r = 0;
		c.g = x;
		c.b = 255;
		break;
	case 1:
		c.r = 0;
		c.g = 255;
		c.b = 255 - x;
		break;
	case 2:
		c.r = x;
		c.g = 255;
		c.b = 0;
		break;
	case 3:
		c.r = 255;
		c.g = 255 - x;
		c.b = 0;
		break;
	case 4:
		c.r = 255;
		c.g = 0;
		c.b = x;
		break;
	default:
		c = white;
		break;
	}
	*color = c;
}

__kernel void sinoscope_kernel(__global unsigned char* output, __global struct sinoscope* sino, width)
{
	int x = get_global_id(0);
	int y = get_global_id(1);
	int index = (y * 3) + (x * 3) * width;
	/*if (x >255)
	{
		output[x] = (unsigned char)0;
        }
	else
	{
		output[x] = (unsigned char)1;
	}*/ 
	output[index] = (unsigned char)(width/512);
	output[index+1] = (unsigned char)(width%512);
	output[index+2] = (unsigned char)255;
	
	//for(y = 1; y < sino->height; ++y) {
        //    index = (y * 3) + (x * 3) * sino->width;
        //    output[index + 0] = 0;
        //    output[index + 1] = 0;
        //    output[index + 2] = 0;
		/*struct rgb c;
            int index, taylor;
            float val, px, py;
            int s_taylor = ptr->taylor;
            float s_time = ptr->time;
            float s_phase0 = ptr->phase0;
            float s_phase1 = ptr->phase1;
            int s_interval = ptr->interval;
            float s_interval_inv = ptr->interval_inv;
            px = sino.dx * y - 2 * M_PI;
            py = sino.dy * x - 2 * M_PI;
            val = 0.0f;
            for (taylor = 1; taylor <= s_taylor; taylor += 2) {
                val += sin(px * taylor * s_phase1 + s_time) / taylor + cos(py * taylor * s_phase0) / taylor;
            }
            val = (atan(1.0 * val) - atan(-1.0 * val)) / (M_PI);
            val = (val + 1) * 100;
            value_color(&c, val, s_interval, s_interval_inv);
            index = (y * 3) + (x * 3) * s_width;
            b[index + 0] = c.r;
            b[index + 1] = c.g;
            b[index + 2] = c.b;
            /*if(x == 1 && y == 1){*/
            /*printf("here\n");*/
            /*printf("index = %d\t\t b[%d] = %u\n", index, index + 0, c.r);*/
            /*printf("index = %d\t\t b[%d] = %u\n", index, index + 1, c.g);*/
            /*printf("index = %d\t\t b[%d] = %u\n", index, index + 2, c.b);*/
            /*}*/
        //}
}
