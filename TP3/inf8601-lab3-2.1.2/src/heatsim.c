/*
 * heatsim.c
 *
 *  Created on: 2011-11-17
 *      Author: francis
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "config.h"
#include "part.h"
#include "grid.h"
#include "cart.h"
#include "image.h"
#include "heat.h"
#include "memory.h"
#include "util.h"

#define PROGNAME "heatsim"
#define DEFAULT_OUTPUT_PPM "heatsim.png"
#define DEFAULT_DIMX 1
#define DEFAULT_DIMY 1
#define DEFAULT_ITER 100
#define MAX_TEMP 1000.0
#define DIM_2D 2

typedef struct ctx {
    cart2d_t *cart;
    grid_t *global_grid;
    grid_t *curr_grid;
    grid_t *next_grid;
    grid_t *heat_grid;
    int numprocs;
    int rank;
    MPI_Comm comm2d;
    FILE *log;
    int verbose;
    int dims[DIM_2D];
    int isperiodic[DIM_2D];
    int coords[DIM_2D];
    int reorder;
    int north_peer;
    int south_peer;
    int east_peer;
    int west_peer;
    MPI_Datatype vector;
} ctx_t;

typedef struct command_opts {
    int dimx;
    int dimy;
    int iter;
    char *input;
    char *output;
    int verbose;
} opts_t;

static opts_t *global_opts = NULL;

__attribute__((noreturn))
    static void usage(void) {
        fprintf(stderr, PROGNAME " " VERSION " " PACKAGE_NAME "\n");
        fprintf(stderr, "Usage: " PROGNAME " [OPTIONS] [COMMAND]\n");
        fprintf(stderr, "\nOptions:\n");
        fprintf(stderr, "  --help	this help\n");
        fprintf(stderr, "  --iter	number of iterations to perform\n");
        fprintf(stderr, "  --dimx	2d decomposition in x dimension\n");
        fprintf(stderr, "  --dimy	2d decomposition in y dimension\n");
        fprintf(stderr, "  --input  png input file\n");
        fprintf(stderr, "  --output ppm output file\n");
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }

static void dump_opts(struct command_opts *opts) {
    printf("%10s %s\n", "option", "value");
    printf("%10s %d\n", "dimx", opts->dimx);
    printf("%10s %d\n", "dimy", opts->dimy);
    printf("%10s %d\n", "iter", opts->iter);
    printf("%10s %s\n", "input", opts->input);
    printf("%10s %s\n", "output", opts->output);
    printf("%10s %d\n", "verbose", opts->verbose);
}

void default_int_value(int *val, int def) {
    if (*val == 0)
        *val = def;
}

static int parse_opts(int argc, char **argv, struct command_opts *opts) {
    int idx;
    int opt;
    int ret = 0;

    struct option options[] = { { "help", 0, 0, 'h' },
        { "iter", 1, 0, 'r' }, { "dimx", 1, 0, 'x' }, { "dimy",
                                                          1, 0, 'y' }, { "input", 1, 0, 'i' }, {
                                                              "output", 1, 0, 'o' }, { "verbose", 0,
                                                                  0, 'v' }, { 0, 0, 0, 0 } };

    memset(opts, 0, sizeof(struct command_opts));

    while ((opt = getopt_long(argc, argv, "hvx:y:l:", options, &idx)) != -1) {
        switch (opt) {
            case 'r':
                opts->iter = atoi(optarg);
                break;
            case 'y':
                opts->dimy = atoi(optarg);
                break;
            case 'x':
                opts->dimx = atoi(optarg);
                break;
            case 'i':
                if (asprintf(&opts->input, "%s", optarg) < 0)
                    goto err;
                break;
            case 'o':
                if (asprintf(&opts->output, "%s", optarg) < 0)
                    goto err;
                break;
            case 'h':
                usage();
                break;
            case 'v':
                opts->verbose = 1;
                break;
            default:
                printf("unknown option %c\n", opt);
                ret = -1;
                break;
        }
    }

    /* default values*/
    default_int_value(&opts->iter, DEFAULT_ITER);
    default_int_value(&opts->dimx, DEFAULT_DIMX);
    default_int_value(&opts->dimy, DEFAULT_DIMY);
    if (opts->output == NULL)
        if (asprintf(&opts->output, "%s", DEFAULT_OUTPUT_PPM) < 0)
            goto err;
    if (opts->input == NULL) {
        fprintf(stderr, "missing input file");
        goto err;
    }

    if (opts->dimx == 0 || opts->dimy == 0) {
        fprintf(stderr,
                "argument error: dimx and dimy must be greater than 0\n");
        ret = -1;
    }

    if (opts->verbose)
        dump_opts(opts);
    global_opts = opts;
    return ret;
err:
    FREE(opts->input);
    FREE(opts->output);
    return -1;
}

FILE *open_logfile(int rank) {
    char str[255];
    sprintf(str, "out-%d", rank);
    FILE *f = fopen(str, "w+");
    return f;
}

ctx_t *make_ctx() {
    ctx_t *ctx = (ctx_t *) calloc(1, sizeof(ctx_t));
    return ctx;
}

void free_ctx(ctx_t *ctx) {
    if (ctx == NULL)
        return;
    free_grid(ctx->global_grid);
    free_grid(ctx->curr_grid);
    free_grid(ctx->next_grid);
    free_grid(ctx->heat_grid);
    free_cart2d(ctx->cart);
    if (ctx->log != NULL) {
        fflush(ctx->log);
        fclose(ctx->log);
    }
    FREE(ctx);
}

int init_ctx(ctx_t *ctx, opts_t *opts) {
    MPI_Comm_size(MPI_COMM_WORLD, &ctx->numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &ctx->rank);

    if (opts->dimx * opts->dimy != ctx->numprocs) {
        fprintf(stderr,
                "2D decomposition blocks must equal number of process\n");
        goto err;
    }
    ctx->log = open_logfile(ctx->rank);
    ctx->verbose = opts->verbose;
    ctx->dims[0] = opts->dimx;
    ctx->dims[1] = opts->dimy;
    ctx->isperiodic[0] = 1;
    ctx->isperiodic[1] = 1;
    ctx->reorder = 0;
    grid_t *new_grid = NULL;

    /* FIXME: create 2D cartesian communicator */
    MPI_Cart_create(MPI_COMM_WORLD, DIM_2D, ctx->dims, ctx->isperiodic, ctx->reorder, &ctx->comm2d);

    MPI_Cart_shift(ctx->comm2d, 0, 1, &ctx->south_peer, &ctx->north_peer);
    // maybe west -> east
    MPI_Cart_shift(ctx->comm2d, 1, 1, &ctx->west_peer, &ctx->east_peer);

    MPI_Cart_coords(ctx->comm2d, ctx->rank, DIM_2D, ctx->coords);
    printf("\x1b[36m rank: %d ", ctx->rank);
    printf("\x1b[36m process id: \x1b[32m %d \x1b[36m coords[0]:\x1b[32m %d \x1b[36m coords[1]: \x1b[32m %d\n \x1b[0m", getpid(), ctx->coords[0], ctx->coords[1]);

    /*
     * FIXME: le processus rank=0 charge l'image du disque
     * et transfert chaque section aux autres processus
     */
    if (ctx->rank == 0)
    {
        // seul rank[0] peut charger l'image
        /* load input image */
        /*printf("rank 0 init_ctxi with pid: %d\n", getpid());*/
        image_t *image = load_png(opts->input);
        if (image == NULL)
            goto err;
        /* select the red channel as the heat source */
        /*printf("ctx->global_grid loading from image\n");*/
        ctx->global_grid = grid_from_image(image, CHAN_RED);

        // send global_grid to otherz

        /* grid is normalized to one, multiply by MAX_TEMP */
        grid_multiply(ctx->global_grid, MAX_TEMP);

        /* 2D decomposition */
        /*printf("ctx->cart instantiation\n");*/
        ctx->cart = make_cart2d(ctx->global_grid->width,
                ctx->global_grid->height, opts->dimx, opts->dimy);
        cart2d_grid_split(ctx->cart, ctx->global_grid);

        /*
         * FIXME: send grid dimensions and data
         * Comment traiter le cas de rank=0 ?
         */
        // we don't send to rank 0 so we skip i = 0
        /*printf("dimx = %d\n", opts->dimx);*/
        /*printf("dimy = %d\n", opts->dimy);*/
        for(int i = 0; i < opts->dimx; ++i) {
            for(int j = 0; j < opts->dimy; ++j) {
                /*
                 * MPI_Send args
                 * const void buf*, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm
                 */
                grid_t *buf = ctx->cart->grids[IX2(i, j, ctx->cart->block_x)];
                /*int count = ctx->cart->dims[i][j];*/
                int count = buf->width * buf->height;
                int dest = i*opts->dimx+j;
                int tag = 0;
                if(dest != 0) {
                    // send size of grid
                    /*printf("rank 0 sending to dest: %d with i = %d,  j = %d\n", dest, i, j);*/
                    MPI_Send(&buf->width, 1, MPI_INT, dest, tag, ctx->comm2d);
                    /*printf("MPI_Send width of %d ctx->cart->grids[IX2(%d, %d, %d)]\n", buf->width, i, j, ctx->cart->block_x);*/
                    MPI_Send(&buf->height, 1, MPI_INT, dest, tag, ctx->comm2d);
                    /*printf("MPI_Send height of %d ctx->cart->grids[IX2(%d, %d, %d)]\n", buf->height, i, j, ctx->cart->block_x);*/
                    MPI_Send(&buf->padding, 1, MPI_INT, dest, tag, ctx->comm2d);
                    /*printf("MPI_Send padding %d ctx->cart->grids[IX2(%d, %d, %d)]\n", buf->padding, i, j, ctx->cart->block_x);*/
                    MPI_Send(&count, 1, MPI_INT, dest, tag, ctx->comm2d);
                    /*printf("MPI_Send buffer size %d ctx->cart->grids[IX2(%d, %d, %d)] dest: %d\n", count, i, j, ctx->cart->block_x, dest);*/
                    /*printf("%d\n", MPI_Cart_rank(ctx->comm2d, ctx->coords, &dest));*/

                    // send grid
                    /*printf("MPI_Send buffer ctx->cart->grids[IX2(%d, %d, %d)]\n", i, j, ctx->cart->block_x);*/
                    MPI_Send(buf->data, count, MPI_INT, dest, tag, ctx->comm2d);
                }
                else {
                    ctx->global_grid = ctx->cart->grids[IX2(0, 0, 0)];
                    printf("\x1b[35m rank \x1b[32m %d ", ctx->rank);
                    printf("\x1b[35m received width \x1b[32m %d", ctx->global_grid->width);
                    printf("\x1b[35m height \x1b[32m %d\n", ctx->global_grid->height);
                }
            }
        }
    }
    else {
        /*printf("ctx_init rank %d with pid: %d\n", ctx->rank, getpid());*/
        int width, height, padding, count;
        // receive size of grid
        MPI_Recv(&width, 1, MPI_INT, 0, 0, ctx->comm2d, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv width of grid %d in rank %d\n", width, ctx->rank);*/
        MPI_Recv(&height, 1, MPI_INT, 0, 0, ctx->comm2d, MPI_STATUS_IGNORE);
        printf("\x1b[35m rank \x1b[32m %d ", ctx->rank);
        printf("\x1b[35m received width \x1b[32m %d", width);
        printf("\x1b[35m height \x1b[32m %d\n", height);
        /*printf("MPI_Recv height of grid %d in rank %d\n", height, ctx->rank);*/
        MPI_Recv(&padding, 1, MPI_INT, 0, 0, ctx->comm2d, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv padding of grid %d in rank %d\n", padding, ctx->rank);*/
        MPI_Recv(&count, 1, MPI_INT, 0, 0, ctx->comm2d, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv size of grid %d in rank %d\n", count, ctx->rank);*/

        /*MPI_Recv(ctx->current_grid, count, MPI_INT, 0, 0, ctx->comm2d, MPI_STATUS_IGNORE);*/
        // need to allocate memory for received grid
        /*printf("allocate memory for grid in rank %d\n", ctx->rank);*/
        ctx->global_grid = make_grid(width, height, padding);
        /*printf("MPI_Recv grid in rank %d\n", ctx->rank);*/
        MPI_Recv(ctx->global_grid->data, count, MPI_INT, 0, 0, ctx->comm2d, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv grid received in rank %d\n", ctx->rank);*/
    }

    new_grid = ctx->global_grid;
    if (new_grid == NULL)
        goto err;
    /* set padding required for Runge-Kutta */
    /*printf("grid_padding on ctx->curr_grid rank %d\n", ctx->rank);*/
    ctx->curr_grid = grid_padding(new_grid, 1);
    /*printf("grid_padding on ctx->next_grid rank %d\n", ctx->rank);*/
    ctx->next_grid = grid_padding(new_grid, 1);
    /*printf("grid_padding on ctx->heat_grid rank %d\n", ctx->rank);*/
    ctx->heat_grid = grid_padding(new_grid, 1);
    //free_grid(new_grid);
    //
    printf("\x1b[34m end of init_ctx rank %d with pid : %d\n", ctx->rank, getpid());

    /* FIXME: create type vector to exchange columns */
    /*MPI_Type_vector(ctx->global_grid->height, 1, ctx->global_grid->width, MPI_INTEGER, &ctx->vector);*/
    /* Make it accessible to communication */
    /*MPI_Type_commit(&ctx->vector);*/
    return 0;
err: return -1;

}

void dump_ctx(ctx_t *ctx) {
    fprintf(ctx->log, "*** CONTEXT ***\n");
    fprintf(ctx->log, "rank=%d\n", ctx->rank);
    fprintf(ctx->log, "north=%d south=%d west=%d east=%d \n",
            ctx->north_peer, ctx->south_peer,
            ctx->east_peer, ctx->west_peer);
    fprintf(ctx->log, "***************\n");
}

void exchng2d(ctx_t *ctx) {
    /*
     *  FIXME: Echanger les bordures avec les voisins
     * 4 echanges doivent etre effectues
     */

    //TODO("lab3");
    grid_t *grid = ctx->next_grid;
    int width = grid->width;
    int height = grid->height;
    int *data = grid->data;
    MPI_Status status[8];
    /*printf("grid info for rank %d\n", ctx->rank);*/
    /*printf("width: %d\nheight: %d\nnorth: %d\nsouth: %d\neast: %d\nwest: %d\n", width, height, ctx->north_peer, ctx->south_peer, ctx->east_peer, ctx->west_peer);*/

    // Peer ids
    int south = ctx->south_peer,
        north = ctx->north_peer,
        west  = ctx->west_peer,
        east  = ctx->east_peer;

    // temp array for saving borders
    int * north_border = (int *) calloc(ctx->next_grid->width, sizeof(int));
    int * south_border = (int *) calloc(ctx->next_grid->width, sizeof(int));
    int * east_border = (int *) calloc(ctx->next_grid->height, sizeof(int));
    int * west_border = (int *) calloc(ctx->next_grid->height, sizeof(int));

    for(int i = 0; i < grid->width; ++i){
        north_border[i] = grid->data[i];
        south_border[i] = grid->data[i + grid->width*(grid->height - 1)];
    }

    for(int j = 0; j < grid->height; ++j){
        west_border[j] = grid->data[j*grid->width];
        east_border[j] = grid->data[j*grid->width + grid->width - 1];
    }

    // dirty has hell
    int *recv1 = (int *) calloc(grid->width, sizeof(int));
    int *recv2 = (int *) calloc(grid->width, sizeof(int));

    // north->south
    printf("\x1b[33m rank : \x1b[32m %d", ctx->rank);
    printf("\x1b[33m pid : \x1b[32m %d", getpid());
    printf("\x1b[33m dest : \x1b[32m %d", south);
    printf("\x1b[33m source : \x1b[32m %d", north);
    printf("\x1b[33m size : \x1b[32m %d", width);
    printf("\x1b[0m\n");
    MPI_Sendrecv(south_border, width, MPI_INTEGER, south, 0, data, width, MPI_INTEGER, north, 0, ctx->comm2d, &status[0]);

    // south->nort
    printf("\x1b[33m rank : \x1b[32m %d", ctx->rank);
    printf("\x1b[33m pid : \x1b[32m %d", getpid());
    printf("\x1b[33m dest : \x1b[32m %d", north);
    printf("\x1b[33m source : \x1b[32m %d", south);
    printf("\x1b[33m size : \x1b[32m %d", width);
    printf("\x1b[0m\n");
    MPI_Sendrecv(north_border, width, MPI_INTEGER, north, 0, data + width*(height-1), width, MPI_INTEGER, south, 0, ctx->comm2d, &status[1]);

    // east->west
    printf("\x1b[33m rank : \x1b[32m %d", ctx->rank);
    printf("\x1b[33m pid : \x1b[32m %d", getpid());
    printf("\x1b[33m dest : \x1b[32m %d", west);
    printf("\x1b[33m source : \x1b[32m %d", east);
    printf("\x1b[33m size : \x1b[32m %d", height);
    printf("\x1b[0m\n");
    MPI_Sendrecv(west_border, height, MPI_INTEGER, west, 0, recv1, height, MPI_INTEGER, east, 0, ctx->comm2d, &status[2]);
    // copy recv1 to grid
    for(int i = 0; i < height; ++i){
        grid->data[i * grid->width] = recv1[i];
    }

    // west->east
    printf("\x1b[33m rank : \x1b[32m %d", ctx->rank);
    printf("\x1b[33m pid : \x1b[32m %d", getpid());
    printf("\x1b[33m dest : \x1b[32m %d", east);
    printf("\x1b[33m source : \x1b[32m %d", west);
    printf("\x1b[33m size : \x1b[32m %d", height);
    printf("\x1b[0m\n");
    MPI_Sendrecv(east_border, height, MPI_INTEGER, east, 0, recv2, height, MPI_INTEGER, west, 0, ctx->comm2d, &status[3]);
    // copy recv2 to grid
    for(int i = 0; i < height; ++i){
        grid->data[(width - 1) + i*(grid->width)] = recv2[i];
    }

    free(recv1);
    free(recv2);
    free(north_border);
    free(south_border);
    free(west_border);
    free(east_border);
}

int gather_result(ctx_t *ctx, opts_t *opts) {
    TODO("lab3");

    exit(0);
    int ret = 0;
    grid_t *local_grid = grid_padding(ctx->next_grid, 0);
    if (local_grid == NULL)
        goto err;

    /*
     * FIXME: transfer simulation results from all process to rank=0
     * use grid for this purpose
     */

    /* now we can merge all data blocks, reuse global_grid */
    //cart2d_grid_merge(ctx->cart, ctx->global_grid);
    /* temporairement copie de next_grid */
    grid_copy(ctx->next_grid, ctx->global_grid);

done: free_grid(local_grid);
      return ret;
err: ret = -1;
     goto done;
}

int main(int argc, char **argv) {
    ctx_t *ctx = NULL;
    int rep, ret;
    opts_t opts;

    if (parse_opts(argc, argv, &opts) < 0) {
        printf("Error while parsing arguments\n");
        usage();
    }
    if (opts.verbose)
        dump_opts(&opts);

    MPI_Init(&argc, &argv);

    ctx = make_ctx();
    if (init_ctx(ctx, &opts) < 0)
        goto err;
    if (opts.verbose)
        dump_ctx(ctx);

    if (ctx->verbose) {
        fprintf(ctx->log, "heat grid\n");
        fdump_grid(ctx->heat_grid, ctx->log);
    }

    for (rep = 0; rep < opts.iter; rep++) {
        if (ctx->verbose) {
            fprintf(ctx->log, "iter %d\n", rep);
            fprintf(ctx->log, "start\n");
            fdump_grid(ctx->curr_grid, ctx->log);
        }

        grid_set_min(ctx->heat_grid, ctx->curr_grid);
        if (ctx->verbose) {
            fprintf(ctx->log, "grid_set_min\n");
            fdump_grid(ctx->curr_grid, ctx->log);
        }

        exchng2d(ctx);
        if (ctx->verbose) {
            fprintf(ctx->log, "exchng2d\n");
            fdump_grid(ctx->curr_grid, ctx->log);
        }

        heat_diffuse(ctx->curr_grid, ctx->next_grid);
        if (ctx->verbose) {
            fprintf(ctx->log, "heat_diffuse\n");
            fdump_grid(ctx->next_grid, ctx->log);
        }
        SWAP(ctx->curr_grid, ctx->next_grid);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (gather_result(ctx, &opts) < 0)
        goto err;

    if (ctx->rank == 0) {
        printf("saving...\n");
        if (save_grid_png(ctx->global_grid, opts.output) < 0) {
            printf("saving failed\n");
            goto err;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    ret = EXIT_SUCCESS;
done:
    free_ctx(ctx);
    MPI_Finalize();
    FREE(opts.input);
    FREE(opts.output);
    return ret;
err:
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    ret = EXIT_FAILURE;
    goto done;
}

