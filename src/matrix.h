#pragma once
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
struct matrix{
  uint64_t row_c, column_c;
  size_t memb;
  void* mat;
};
typedef struct matrix matrix;

#define MATSIZE(matrix_t)        (((matrix_t->row_c) * (matrix_t->column_c)))
#define MATBYTES(matrix_t)       ((size_t)(matrix_t->row_c * matrix_t->column_c * matrix_t->memb))
#define I_LININDEX(matrix_t, i)  ((i) / (matrix_t->column_c))
#define J_LININDEX(matrix_t, i)  ((i) % (matrix_t->column_c))
#define deb  printf("--------\n%s %d \n--------\n", __FILE__, __LINE__);

matrix *create_matrix(uint64_t row_c, uint64_t column_c, size_t memb);
matrix *matrix_copy(matrix *m);
void* matref(matrix* m, int i, int j);
void matrix_map(matrix *m, void *args,
                void (*func)(matrix *status, uint64_t i, uint64_t j, void * args));
void para_matrix_map(uint32_t nproc , matrix * m, void * args,
		     void (*func)(matrix *m, uint64_t i, uint64_t j, void * args));


