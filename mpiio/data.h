#ifndef __DATA_H_
#define __DATA_H_

double* allocate_block(int dims, uint64_t* spdim, size_t elem_size);

void set_offset(uint64_t *offset, int rank, int dims, int* npdim, uint64_t* spdim);

int generate_block(double *mnd, int rank, int dims, uint64_t* spdim, size_t elem_size);

#endif