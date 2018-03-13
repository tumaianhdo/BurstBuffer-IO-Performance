#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mpi.h"
#include "data.h"

int mpi_write(int rank, const char *filename, int dims, int* npdim, uint64_t* spdim, size_t elem_size, MPI_Comm gcomm) {
	MPI_File file;
	MPI_Offset offset;
	MPI_Status status;
	double *data;
	int n;

	data = allocate_block(dims, spdim, elem_size);
	n = generate_block(data, rank, dims, spdim, elem_size);
	offset = (long long)rank * (long long)n * (long long)sizeof(double);

	// printf("FileName : %s\n", filename);

  MPI_File_open(gcomm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);
	MPI_File_seek(file, offset, MPI_SEEK_SET);
	MPI_File_write(file, data, n, MPI_DOUBLE, &status);
	MPI_File_close(&file);

	return 0;
}