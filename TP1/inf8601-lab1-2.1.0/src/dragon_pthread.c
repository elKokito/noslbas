/*
 * dragon_pthread.c
 *
 *  Created on: 2011-08-17
 *      Author: Francis Giraldeau <francis.giraldeau@gmail.com>
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>

#include "dragon.h"
#include "color.h"
#include "dragon_pthread.h"

pthread_mutex_t mutex_stdout;

void printf_safe(char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	pthread_mutex_lock(&mutex_stdout);
	vprintf(format, ap);
	pthread_mutex_unlock(&mutex_stdout);
	va_end(ap);
}

void *dragon_draw_worker(void *data)
{
    struct draw_data *draw = (struct draw_data*)data;
    
    /* 1. Initialiser la surface */
    uint64_t area            = draw->dragon_width * draw->dragon_height,
             area_block_size = area / draw->nb_thread,
             area_remainder  = area - (area_block_size * draw->nb_thread);

    uint64_t area_start = draw->id * area_block_size,
             area_end   = (draw->id != draw->nb_thread-1) ? 
                 (draw->id + 1) * area_block_size: 
                 (draw->id + 1) * area_block_size + area_remainder;
    
    //printf_safe((char*)"Draw_worker canvas initialization data: Start=%" PRIu64 ", \tEnd=%" PRIu64 ", \tID=%d\n", area_start, area_end, draw->id);

    init_canvas(
            area_start,
            area_end, 
            draw->dragon, 
            -1);

    if (pthread_barrier_wait(draw->barrier) == PTHREAD_BARRIER_SERIAL_THREAD)
    {
        //printf_safe((char*)"Canvas initialization completed, all draw_workers are synchronized and ready to draw.\n");
    }
    /* 2. Dessiner le dragon */
    uint64_t block_size = draw->size / draw->nb_thread,
             remainder  = draw->size - (block_size * draw->nb_thread);
    
    uint64_t start = draw->id * block_size,
             end   = (draw->id != draw->nb_thread-1) ? 
                 (draw->id + 1) * block_size: 
                 (draw->id + 1) * block_size + remainder;
   
    //printf_safe((char*)"Draw_worker data: Start=%" PRIu64 ", \tEnd=%" PRIu64 ", \tID=%d\n", start, end, draw->id);
    
    dragon_draw_raw(
            start, 
            end, 
            draw->dragon, 
            draw->dragon_width, 
            draw->dragon_height, 
            draw->limits, 
            draw->id);

    if (pthread_barrier_wait(draw->barrier) == PTHREAD_BARRIER_SERIAL_THREAD)
    {
        //printf_safe((char*)"Drawing completed, all draw_workers are synchronized and ready to scale.\n");
    }

	/* 3. Effectuer le rendu final */

    uint64_t scale_height     = draw->image_height,
             scale_block_size = scale_height / draw->nb_thread,
             scale_remainder  = scale_height - (scale_block_size * draw->nb_thread);

    uint64_t scale_start = draw->id * scale_block_size,
             scale_end   = (draw->id != draw->nb_thread-1) ? 
                 (draw->id + 1) * scale_block_size: 
                 (draw->id + 1) * scale_block_size + scale_remainder;
    
    //printf_safe((char*)"Draw_worker scale data: Start=%" PRIu64 ", \tEnd=%" PRIu64 ", \tID=%d\n", scale_start, scale_end, draw->id);

    scale_dragon(
            scale_start,
            scale_end, 
            draw->image, 
            draw->image_width,
            draw->image_height, 
            draw->dragon, 
            draw->dragon_width, 
            draw->dragon_height, 
            draw->palette);
    
    pthread_exit(NULL);
}

int dragon_draw_pthread(char **canvas, struct rgb *image, int width, int height, uint64_t size, int nb_thread)
{
	printf("Started dragon_draw_pthread\n");
	pthread_t *threads = NULL;
	pthread_barrier_t barrier;
	limits_t limits;
	struct draw_data info;
	char *dragon = NULL;
	int scale_x;
	int scale_y;
	struct draw_data *data = NULL;
	struct palette *palette = NULL;
	int ret = 0;

	palette = init_palette(nb_thread);
	if (palette == NULL)
		goto err;

	if (pthread_barrier_init(&barrier, NULL, nb_thread) != 0) {
		printf("barrier init error\n");
		goto err;
	}

	/* 1. Calculer les limites du dragon */
	if (dragon_limits_pthread(&limits, size, nb_thread) < 0)
		goto err;

	info.dragon_width = limits.maximums.x - limits.minimums.x;
	info.dragon_height = limits.maximums.y - limits.minimums.y;

	if ((dragon = (char *) malloc(info.dragon_width * info.dragon_height)) == NULL) {
		printf("malloc error dragon\n");
		goto err;
	}

	if ((data = malloc(sizeof(struct draw_data) * nb_thread)) == NULL) {
		printf("malloc error data\n");
		goto err;
	}

	if ((threads = malloc(sizeof(pthread_t) * nb_thread)) == NULL) {
		printf("malloc error threads\n");
		goto err;
	}

	info.image_height = height;
	info.image_width = width;
	scale_x = info.dragon_width / width + 1;
	scale_y = info.dragon_height / height + 1;
	info.scale = (scale_x > scale_y ? scale_x : scale_y);
	info.deltaJ = (info.scale * width - info.dragon_width) / 2;
	info.deltaI = (info.scale * height - info.dragon_height) / 2;
	info.nb_thread = nb_thread;
	info.dragon = dragon;
	info.image = image;
	info.size = size;
	info.limits = limits;
	info.barrier = &barrier;
	info.palette = palette;

    //int area = info.dragon_width * info.dragon_height;

    //printf("-----------------\n");
    //printf("Total area size: %d\n", area);
    //printf("Draw area block size: %d\n", area / nb_thread);
    //printf("-----------------\n");
    //printf("Total draw size:%" PRIu64 "\n", size);
    //printf("Draw block size:%" PRIu64 "\n", size / nb_thread);
    //printf("-----------------\n");
    //printf("Total scale size: %d\n", info.image_height);
    //printf("Scale block size: %d\n", info.image_height / nb_thread);
    //printf("-----------------\n");
    /* 2. Lancement du calcul parallèle principal avec draw_dragon_worker */
	int i = 0;
    for (; i < nb_thread; ++i)
	{
		data[i].image_height = info.image_height;
        data[i].image_width = info.image_width;
        data[i].scale = info.scale;
        data[i].deltaJ = info.deltaJ;
        data[i].deltaI = info.deltaI;
        data[i].dragon = info.dragon;
        data[i].dragon_width = info.dragon_width;
        data[i].dragon_height = info.dragon_height;
        data[i].image = info.image;
        data[i].size = info.size;
        data[i].limits = info.limits;
        data[i].barrier = info.barrier;
        data[i].palette = info.palette;
        data[i].id = i;
        data[i].nb_thread = info.nb_thread;
		pthread_create(&threads[i], NULL, dragon_draw_worker, &data[i]); 
	}

	/* 3. Attendre la fin du traitement */
    for (i = 0; i < nb_thread; ++i)
    {
        pthread_join(threads[i], NULL);
    }

	if (pthread_barrier_destroy(&barrier) != 0) {
		printf("barrier destroy error\n");
		goto err;
	}
    printf("Finished dragon_draw_pthread\n");
   
done:
	FREE(data);
	FREE(threads);
	free_palette(palette);
	*canvas = dragon;
	return ret;

err:
	FREE(dragon);
	ret = -1;
	goto done;
}

void *dragon_limit_worker(void *data)
{
	struct limit_data *lim = (struct limit_data *) data;
	piece_limit(lim->start, lim->end, &lim->piece);
	pthread_exit(NULL);
}

/*
 * Calcule les limites en terme de largeur et de hauteur de
 * la forme du dragon. Requis pour allouer la matrice de dessin.
 */
int dragon_limits_pthread(limits_t *limits, uint64_t size, int nb_thread)
{
	printf("Started dragon_limits_pthread\n");
	uint64_t block_size = size / nb_thread;
	uint64_t remainder  = size - (block_size * nb_thread);
	//printf("Total size: %" PRIu64 "\n",size);
	//printf("Block size : %" PRIu64 "\n", block_size);
	//if (remainder != 0U)
	//	printf("Remainder: %" PRIu64 "\n", remainder);
	
	int ret = 0;
	pthread_t *threads = NULL;
	struct limit_data *thread_data = NULL;
	piece_t master;

	piece_init(&master);

	if ((threads = calloc(nb_thread, sizeof(pthread_t))) == NULL)
		goto err;

	if ((thread_data = calloc(nb_thread, sizeof(struct limit_data))) == NULL)
		goto err;

	/* 1. Lancement du calcul en parallèle avec dragon_limit_worker */
	int i, j;
	for(i = 0, j = 0; i < nb_thread; ++i) {
        piece_init(&thread_data[i].piece);
	    thread_data[i].id = i;
        thread_data[i].start = i * block_size + j;
		if (remainder != 0U)
		{
			++j;
			--remainder;
		}
        thread_data[i].end = (i+1) * block_size + j;
       	//printf("thread_data: Start=%" PRIu64 ", \tEnd=%" PRIu64 ", \tID=%d\n", thread_data[i].start, thread_data[i].end, thread_data[i].id);
        piece_init(&thread_data[i].piece);
        pthread_create(&threads[i], NULL, dragon_limit_worker, &thread_data[i]);
	};

	/* 2. Attendre la fin du traitement */
 	for(i = 0; i < nb_thread; ++i) {
        pthread_join(threads[i], NULL);
	}

    /* 3. Fusion des pièces */
	for(i = 0; i < nb_thread; ++i) {
        piece_merge(&master, thread_data[i].piece);
    };
	printf("Finished merging pieces\n");
	printf("Finished dragon_limits_pthread\n");
	
done:
	FREE(threads);
	FREE(thread_data);
	*limits = master.limits;
	return ret;
err:
	ret = -1;
	goto done;
}
