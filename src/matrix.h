#pragma  once
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
//macros
#define MATSIZE(matrix_t)        (((matrix_t->row_c) * (matrix_t->column_c)))
#define MATBYTES(matrix_t)       ((size_t)(matrix_t->row_c * matrix_t->column_c * matrix_t->memb))
//#define MATREF_P(matrix_t, __i, __j) ((matrix_t->mat) + ((j) * (matrix_t->memb)) + ((i) * (matrix_t->row_c) * (matrix_t->memb))) donesn't work no idae why might try -E later
#define I_LININDEX(matrix_t, i)  ((i) / (matrix_t->column_c))
#define J_LININDEX(matrix_t, i)  ((i) % (matrix_t->column_c))
#define deb  printf("--------\n%s %d \n--------\n", __FILE__, __LINE__);
//funcs
matrix *create_matrix(uint64_t row_c, uint64_t column_c, size_t memb);
matrix *matrix_copy(matrix *m);
void* matref(matrix* , int, int);
void matrix_map (matrix*, void*, void(func)(matrix* status , uint64_t, uint64_t, void*));
int matrxi_fill(matrix* mat, void*);

