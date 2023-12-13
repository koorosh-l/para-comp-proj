#include "matrix.h"
#include <time.h>
#include <string.h>
#define RLIMIT 3
#define FI 5
#define FJ 5
#define SI 2
#define SJ 2


matrix *some_opration(matrix *m1, matrix *m2);
void printer(matrix *, uint64_t i, uint64_t j, void*);
void matrand(matrix *, uint64_t i, uint64_t j, void *);

int main(int argc, char **argv){
  matrix *m1;
  matrix *m2;
  matrix *m3;
  srand(10);
  m1 = create_matrix(FI, FJ, sizeof(int));
  m2 = create_matrix(SI, SJ, sizeof(int));
  
  matrix_map(m1, NULL, matrand);
  matrix_map(m1, NULL, printer);
  deb;
  matrix_map(m2, NULL, matrand);
  matrix_map(m2, NULL, printer);
  deb;
#ifdef UGLY
  m3 = some_ugly_op(m1, m2);
#else 
  some_opration(m1, m2);
#endif
  matrix_map(m1, NULL, printer);
  return 0;
}

void printer(matrix *m, uint64_t i, uint64_t j, void* args){
  int* field = matref(m, i, j);
  printf("%5d ", *(int*)field);
  if(j == m->column_c - 1) puts("");
}
void matrand(matrix *mat, uint64_t i, uint64_t j, void *args) {
  int * f = matref(mat, i, j);
  *f = rand() % RLIMIT;
}

void inerator(matrix* m2, uint64_t i2, uint64_t j2, void * status){
   uint64_t i1, j1, *res; 
  matrix* m1;
  m1 = (matrix *)((void **)status)[0];
  i1 = *(uint64_t *)(((void **)status)[1]);
  j1 = *(uint64_t *)(((void **)status)[2]);
  res = (uint64_t *)(((void **)status)[3]);
  /* printf("%" PRIu64" %" PRIu64"\n", i1, j1);   */
  if(i1 + i2 >= m1->row_c || j1 + j2 >= m1->column_c) return;
  *res = *res + (*(int *)matref(m1, i1 + i2, j1 + j2)) * (*(int *)matref(m2, i2, j2));
  
}
void rator(matrix* m1, uint64_t i1, uint64_t j1,void* m2){
  uint64_t res = 0;
  void *status[4];
  status [0] = m1;
  status [1] = &i1;
  status [2] = &j1;
  status [3] = &res;
  matrix_map(m2, status, inerator);
  *(int*)(matref(m1, i1, j1)) = res;
}
matrix *some_opration(matrix *m1, matrix *m2){
#ifdef NPROC
  para_matrix_map(NPROC,m1, m2, rator);
#else
  matrix_map(m1, m2, rator);
#endif
  return NULL;
}
