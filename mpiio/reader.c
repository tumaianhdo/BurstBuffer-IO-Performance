#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mpi.h"
#include "utils.h"
#include "debug.h"

extern int mpi_read(int rank, const char *filename, int dims, uint64_t* spdim, size_t elem_size, MPI_Comm gcomm);

int main (int argc, char **argv) {
	int dims; // number of dimensions
	int np[10] = {0};	//number of processes in each dimension
    uint64_t sp[10] = {0}; //block size per process in each dimesion
    uint64_t off[10] = {0};
    size_t elem_size; // size of one element in the global array. Default value is 8 (bytes).
    char filename[128];

    int err;
	int nprocs, rank;
	MPI_Comm gcomm;


	if (parse_args(argc, argv, filename, &dims, np, sp, &elem_size) != 0) {
		goto err_out;
	}

	// Using SPMD style programming
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int color = 2;
	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &gcomm);

	mpi_read(rank, filename, dims, sp, elem_size, gcomm);

	MPI_Finalize();
	return 0;
err_out:
	uloga("error out!\n");
	return -1;	
}