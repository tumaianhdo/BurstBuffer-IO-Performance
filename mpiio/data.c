#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

double* allocate_block(int dims, uint64_t* spdim, size_t elem_size) {
        double* block = NULL;
        int i = 0;
        uint64_t size = 1;
        for(i = 0; i < dims; i++){
                size *= spdim[i];
        }
        
        if (size == 0) 
        	block = NULL;
        else	
        	block = (double*) malloc(elem_size * size);
        
        return block;
}

void set_offset(uint64_t *offset, int rank, int dims, int* npdim, uint64_t* spdim) {
        int i = 0, j = 0;
        for(i = 0; i < dims; i++) {
                int tmp = rank;
                for(j = 0; j < i; j++)
                        tmp /= npdim[j];

                *(offset+i) = tmp % npdim[i] * spdim[i];
        }
}

int generate_block(double *mnd, int rank, int dims, uint64_t* spdim, size_t elem_size) {
        int i;
        double value = (double) rank;
        uint64_t mnd_size = 1;
        for(i = 0; i < dims; i++)
                mnd_size *= spdim[i];
        mnd_size = mnd_size * elem_size / sizeof(double); 
        for(i = 0; i < mnd_size; i++)
                *(mnd+i) = value;
        return mnd_size;
}