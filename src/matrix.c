#pragma once
#include "matrix.h"

matrix* create_matrix(uint64_t row_c, uint64_t column_c, size_t memb){
  matrix* m;
  m = malloc(sizeof(matrix));
  m->row_c    = row_c;
  m->column_c = column_c;
  m->memb     = memb;
  m->mat      = malloc(MATBYTES(m));
  return m;
}
void* matref(matrix* m, int i, int j){
  return m->mat + (i * m->column_c * m->memb) + (j * m->memb);
}
void matrix_map(matrix *m, void *args,
               void (*func)(matrix *, uint64_t, uint64_t, void *))
{
  for(uint64_t i = 0; i < m->row_c * m->column_c; i++){
    func(m, i / (m->column_c), i % (m->column_c), args);
  }
}

matrix* matrix_copy(matrix * m){
  matrix* res = create_matrix(m->row_c, m->column_c, m->memb);
  memcpy(res->mat, m->mat, MATBYTES(m));
  return res;
}
