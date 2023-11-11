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
#define MATSIZE(matrix_t)        ((matrix_t->row_c * matrix_t->column_c))
#define MATBYTES(matrix_t)       ((size_t)(matrix_t->row_c * matrix_t->column_c * matrix_t->memb))
#define MATREF_P(matrix_t, i, j) ((matrix_t->mat) + ((j) * (matrix_t->memb)) + ((i) * (matrix_t->row_c) * (matrix_t->memb)))
#define LINMATREF_P(matrix_t, i, px, py) {(*px = (i / matrix_t->column_c)); *py = (i % matrix_t->column_c)}
#define I_LININDEX(matriX_t, i)  ()
#define J_LININDEX(matriX_t, i)  ()
#define deb                                                                    \
  {                                                                            \
    puts("----------------");                                                  \
    printf("%s %d \n", __FILE__, __LINE__);                                    \
    puts("----------------");                                                  \
  }

//funcs
matrix *create_matrix(uint64_t row_c, uint64_t column_c, size_t memb);
matrix* matrix_copy(matrix * m);
int matrix_map (matrix* mat, void* args, int(func)(matrix* status , uint64_t, uint64_t, void* field, void* args));
int matrxi_fill(matrix* mat, void*);

