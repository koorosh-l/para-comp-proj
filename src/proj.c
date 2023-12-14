#include "matrix.h"
#include <time.h>
#include <string.h>
#include <unistd.h>
#define RLIMIT 3
#define FI 10
#define FJ 10
#define SI 2
#define SJ 2
#define cnt printf("%d",++counter);
int counter = 0;

pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

void some_op(matrix *m1, matrix *m2);
void psome_op(uint32_t nproc, matrix *m1, matrix *m2);
matrix *some_ugly_op(matrix *m1, matrix *m2);
void printer(matrix *, uint64_t i, uint64_t j, void *);
  
void matrand(matrix *, uint64_t i, uint64_t j, void *);

int main(int argc, char **argv){
  matrix *m1;
  matrix *m2;
  matrix *m3;
  srand(10);
  m1 = create_matrix(FI, FJ, sizeof(int));
  m2 = create_matrix(SI, SJ, sizeof(int));
  puts("");
  matrix_map(m1, NULL, matrand);
  matrix_map(m1, NULL, printer);
  puts("");
  matrix_map(m2, NULL, matrand);
  matrix_map(m2, NULL, printer);
  puts("");
#ifdef UGLY
  puts("ugly");
  m3 = some_ugly_op(m1, m2);
  matrix_map(m3, NULL, printer);  
#endif
#ifdef NPROC
  puts("nproc");
  psome_op(NPROC, m1, m2);
  matrix_map(m1, NULL, printer);
#endif
#ifdef P1
  puts("linear");
  some_op(m1, m2);
  matrix_map(m1, NULL, printer);
#endif
  //fprintf(stderr, "%p %p\n", m1, ((matrix *)m1)->mat);
  free(m1);
  //fprintf(stderr, "%p %p\n", m2, ((matrix *)m2)->mat);  
  free(m2);
#ifdef UGLY
  free(m3);
#endif
  return 0;
}

void printer(matrix *m, uint64_t i, uint64_t j, void* args){
  if(i == 0 && j == 0) {cnt; puts("------------------------------------------------------------");}
  int* field = matref(m, i, j);
  printf("%5d ", *(int*)field);
  if (j == m->column_c - 1) puts("");
}
void matrand(matrix *mat, uint64_t i, uint64_t j, void *args) {
  int * f = matref(mat, i, j);
  *f = rand() % RLIMIT;
}

void inerator(matrix* m2, uint64_t i2, uint64_t j2, void * status){
  uint64_t i1, j1, *res;
  matrix *m1, *m3;
  m1 = (matrix *)((void **)status)[0];
  m3 = ((void **)status)[4];
  i1 = *(uint64_t *)(((void **)status)[1]);
  j1 = *(uint64_t *)(((void **)status)[2]);
  res = (uint64_t *)(((void **)status)[3]);
  /* printf("%" PRIu64" %" PRIu64"\n", i1, j1);   */
  if(i1 + i2 >= m1->row_c || j1 + j2 >= m1->column_c) return;
  *res = *res +
    (*(int *)matref(m1, i1 + i2, j1 + j2)) * (*(int *)matref(m2, i2, j2));
}
void rator(matrix* m1, uint64_t i1, uint64_t j1,void* m2){
  uint64_t res = 0;
  void *status[4];
  status[0] = m1;
  status[1] = &i1;
  status[2] = &j1;
  status[3] = &res;
  if(((matrix* )m2)->mat == 0) fprintf(stderr, "%d %d %p %p\n",(int)i1 , (int)j1, m2, ((matrix *)m2)->mat);  
  matrix_map((matrix*)m2, status, inerator);
  *(int*)(matref(m1, i1, j1)) = res;
}
void some_op(matrix *m1, matrix *m2){
  matrix_map(m1, m2, rator);
}
void psome_op(uint32_t nproc, matrix *m1, matrix *m2){
  para_matrix_map(nproc,m1, m2, rator);
}

void ugly_rator(matrix *m1, uint64_t i1, uint64_t j1, void *st) {
  matrix *m2, *m3;
  m2 = ((void **)st)[0];
  m3 = ((void **)st)[1];  
  void *args[5];
  uint64_t res = 0;
  args[0] = m1;
  args[1] = &i1;
  args[2] = &j1;
  args[3] = &res;
  matrix_map(m2, args, inerator);
  *(int *)(matref(m3, i1, j1)) = res;
  pthread_mutex_lock(&print_lock);
  matrix_map(m3, NULL, printer);
  pthread_mutex_unlock(&print_lock);
  pthread_exit(NULL);
}
void minus1 (matrix* m, uint64_t i, uint64_t j, void* arg){
  *(int *)matref(m, i, j) = -1;
}
matrix *some_ugly_op(matrix* m1, matrix* m2){
  matrix *m3 = matrix_copy(m1);
  matrix_map(m3, NULL, minus1);
  void *st[2];
  st[0] = m2;
  st[1] = m3;
  para_matrix_map(MATSIZE(m1), m1, st, ugly_rator);
  return m3;
}
