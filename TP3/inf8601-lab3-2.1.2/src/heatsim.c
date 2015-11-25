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

    MPI_Cart_shift(ctx->comm2d, 0, 1, &ctx->north_peer, &ctx->south_peer);
    // maybe west -> east
    MPI_Cart_shift(ctx->comm2d, 0, 1, &ctx->east_peer, &ctx->west_peer);

    MPI_Cart_coords(ctx->comm2d, ctx->rank, DIM_2D, ctx->coords);

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
                MPI_Comm comm = ctx->comm2d;
                if(dest != 0) {
                    // send size of grid
                    /*printf("rank 0 sending to dest: %d with i = %d,  j = %d\n", dest, i, j);*/
                    MPI_Send(&buf->width, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                    /*printf("MPI_Send width of %d ctx->cart->grids[IX2(%d, %d, %d)]\n", buf->width, i, j, ctx->cart->block_x);*/
                    MPI_Send(&buf->height, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                    /*printf("MPI_Send height of %d ctx->cart->grids[IX2(%d, %d, %d)]\n", buf->height, i, j, ctx->cart->block_x);*/
                    MPI_Send(&buf->padding, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                    /*printf("MPI_Send padding %d ctx->cart->grids[IX2(%d, %d, %d)]\n", buf->padding, i, j, ctx->cart->block_x);*/
                    MPI_Send(&count, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
                    /*printf("MPI_Send buffer size %d ctx->cart->grids[IX2(%d, %d, %d)]\n", count, i, j, ctx->cart->block_x);*/

                    // send grid
                    /*printf("MPI_Send buffer ctx->cart->grids[IX2(%d, %d, %d)]\n", i, j, ctx->cart->block_x);*/
                    MPI_Send(buf->data, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
                }
            }
        }
    }
    else {
        /*printf("ctx_init rank %d with pid: %d\n", ctx->rank, getpid());*/
        int width, height, padding, count;
        // receive size of grid
        MPI_Recv(&width, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv width of grid %d in rank %d\n", width, ctx->rank);*/
        MPI_Recv(&height, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv height of grid %d in rank %d\n", height, ctx->rank);*/
        MPI_Recv(&padding, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv padding of grid %d in rank %d\n", padding, ctx->rank);*/
        MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv size of grid %d in rank %d\n", count, ctx->rank);*/

        /*MPI_Recv(ctx->current_grid, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);*/
        // need to allocate memory for received grid
        /*printf("allocate memory for grid in rank %d\n", ctx->rank);*/
        ctx->global_grid = make_grid(width, height, padding);
        /*printf("MPI_Recv grid in rank %d\n", ctx->rank);*/
        MPI_Recv(ctx->global_grid->data, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /*printf("MPI_Recv grid received in rank %d\n", ctx->rank);*/
        /*
         * FIXME: receive dimensions of the grid
         * store into new_grid
         */

        /* Utilisation temporaire de global_grid */
    }

    new_grid = ctx->global_grid;
    if (new_grid == NULL)
        goto err;
    /* set padding required for Runge-Kutta */
    printf("grid_padding on ctx->curr_grid rank %d\n", ctx->rank);
    ctx->curr_grid = grid_padding(new_grid, 1);
    printf("grid_padding on ctx->next_grid rank %d\n", ctx->rank);
    ctx->next_grid = grid_padding(new_grid, 1);
    printf("grid_padding on ctx->heat_grid rank %d\n", ctx->rank);
    ctx->heat_grid = grid_padding(new_grid, 1);
    //free_grid(new_grid);
    //
    printf("end of init_ctx rank %d\n", ctx->rank);

    /* FIXME: create type vector to exchange columns */
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
    // TODO : Figure c'est quoi le but des MPI_Datatype (vector n shit)
    // row_end et row_start sont affecté via les rank dans decomp1d dans l'Exemple exchng
    // l'affectation doit se faire dans init, mais comme on y accède ici
    grid_t *grid = ctx->next_grid;
    int width = grid->pw;
    int height = grid->ph;
    int *data = grid->data;
    MPI_Comm comm = ctx->comm2d;
    MPI_Request req[8];
    MPI_Status status[8];

    // Peer ids
    int south = ctx->south_peer,
        north = ctx->north_peer,
        west  = ctx->west_peer,
        east  = ctx->east_peer;

    // Row start et end
    int size = width * height,
        row_start = size * ctx->rank / ctx->numprocs,
        row_end   = (size * (ctx->rank + 1)) / ctx->numprocs;

    // Oi mate, gotta send rows, not a problem mate
    // Exchange north->south
    int *offset_send = data + (row_end - 2) * width;
    int *offset_recv = data + row_start * width;
    MPI_Sendrecv(offset_send, width, MPI_INTEGER, south, 0, offset_recv, width, MPI_INTEGER, north, 0, comm1d, &status[0]);
    if (ctx->verbose) {
        fprintf(ctx->log, "after exchange north->south\n");
        fprint_matrix(data, width, row_start, row_end, ctx->log);
    }
    
    // Exchange south->north
    int *offset_send = data + (row_end - 2) * width;
    int *offset_recv = data + row_start * width;
    MPI_Sendrecv(offset_send, width, MPI_INTEGER, north, 0, offset_recv, width, MPI_INTEGER, south, 0, comm1d, &status[1]);
    if (ctx->verbose) {
        fprintf(ctx->log, "after exchange south->north\n");
        fprint_matrix(data, width, row_start, row_end, ctx->log);
    }
    
    // Holy fuck, pass column mate - use vector
    // Exchange east->west
    MPI_Sendrecv(offset_send, width, ctx->vector, west, 0, offset_recv, width, ctx->vector, east, 0, comm1d, &status[2]);
    if (ctx->verbose) {
        fprintf(ctx->log, "after exchange east->west\n");
        fprint_matrix(data, width, row_start, row_end, ctx->log);
    }
    
    // Exchange west->east
    int *offset_send = data + (row_end - 2) * width;
    int *offset_recv = data + row_start * width;
    MPI_Sendrecv(offset_send, width, ctx->vector, east, 0, offset_recv, width, ctx->vector, west, 0, comm1d, &status[3]);
    if (ctx->verbose) {
        fprintf(ctx->log, "after exchange west->east\n");
        fprint_matrix(data, width, row_start, row_end, ctx->log);
    }
}

int gather_result(ctx_t *ctx, opts_t *opts) {
    TODO("lab3");

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

