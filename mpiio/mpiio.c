#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mpi.h"
#include "data.h"

int mpi_write(int rank, const char *filename, int dims, uint64_t* spdim, size_t elem_size, MPI_Comm gcomm) {
	MPI_File file;
	MPI_Offset offset;
	MPI_Status status;
	double *data;
	int n;

	data = allocate_block(dims, spdim, elem_size);
	n = generate_block(data, rank, dims, spdim, elem_size);
	offset = (long long)rank * (long long)n * (long long)sizeof(double);

	MPI_File_open(gcomm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file);
	MPI_File_seek(file, offset, MPI_SEEK_SET);
	MPI_File_write(file, data, n, MPI_DOUBLE, &status);
	MPI_File_close(&file);

	return 0;
}

int mpi_read(int rank, const char *filename, int dims, uint64_t* spdim, size_t elem_size, MPI_Comm gcomm) {
	MPI_File file;
	MPI_Offset offset;
	MPI_Status status;
	double *data;
	int n;

	int err;
	err = MPI_File_open(gcomm, filename,
                          MPI_MODE_RDONLY, MPI_INFO_NULL, &file);

	if (err != MPI_SUCCESS) {
		char err_str[MPI_MAX_ERROR_STRING];
		int lerr_str;

		MPI_Error_string(err, err_str, &lerr_str);
		printf("%3d: %s\n", rank, err_str);
		MPI_Abort(gcomm, err);
  	}

  	data = allocate_block(dims, spdim, elem_size);
  	n = block_size(dims, spdim, elem_size) / sizeof(double);
  	offset = rank * n * sizeof(double);

  	MPI_File_seek(file, offset, MPI_SEEK_SET);

  	MPI_File_read(file, data, n, MPI_DOUBLE, &status);

  	/*int i;
  	if (rank == 1) {
  		for (i = 0; i < n; i++) {
  			printf("%f ", data[i]);
  		}
  		printf("\n");
  	}*/
  	MPI_File_close(&file);
}