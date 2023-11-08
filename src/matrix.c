#include "matrix.h"

matrix* create_matrix(uint64_t row_c, uint64_t column_c, size_t memb){
  matrix* mat;
  mat = malloc(sizeof(matrix));
  mat->row_c    = row_c;
  mat->column_c = column_c;
  mat->memb     = memb;
  mat->mat      = malloc(row_c * column_c * memb);
  return mat;
}
int matrix_map(matrix *mat, int (*func)(matrix * status,
					uint64_t i, uint64_t j,
					void * field)){
  void* tmp = mat->mat;
  int status = 0;
  for(int i = 0; i < mat->row_c; i++){
    for(int j = 0; j < mat->column_c; j++){
      func(mat, i, j, tmp + (i * mat->column_c * mat->memb) + (j * mat->memb));
    }
  }
}
