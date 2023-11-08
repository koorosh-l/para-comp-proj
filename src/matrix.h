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
#define MATSIZE(matrix_t) ((matrix_t->row_c * matrix_t->column_c))
#define MATBYTES(matrix_t) ((size_t)(matrix_t->row_c * matrix_t->column_c * matrix_t->memb))
#define MATREF(matrix_t, i, j) ((matrix_t->mat+(i * j * matrix_t->memb)))

matrix* create_matrix(uint64_t row_c, uint64_t column_c, size_t memb);
int matrix_map (matrix* mat, int(func)(matrix* status , uint64_t, uint64_t, void* field));
int matrxi_fill(matrix* mat, void*);

