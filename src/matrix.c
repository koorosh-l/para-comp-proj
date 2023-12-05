#include "matrix.h"

struct interval {
  matrix *m;  
  uint64_t start;
  uint64_t end;
  void *pass;
  void (*func) (matrix*,uint64_t,uint64_t,void*);
};
typedef struct interval interval;

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

static inline void* partial_map(interval* args){
  for(uint64_t i = ((interval*)args)->start; i < ((interval*)args)->end; i++){
    args->func(args->m, i/args->m->column_c, i%args->m->column_c, args->pass);
  }
  pthread_exit(NULL);
  return NULL;
}

void para_matrix_map(uint8_t nproc, matrix *m, void *arg,
                      void (*func)(matrix *, uint64_t, uint64_t, void *))
{
  uint64_t step = MATSIZE(m)/nproc;
  pthread_t threads[nproc];
  interval  inter[nproc];
  for(uint8_t i = 0; i < nproc - 1; i++){
    inter[i].m     = m;
    inter[i].start = i * step;
    inter[i].end   = (inter[i].start+step);
    inter[i].pass  = arg;
    inter[i].func  = func;
    pthread_create(&threads[i], NULL, ((void* (*)(void*))partial_map), &inter[i]);
  }
  inter[nproc - 1].m     = m;
  inter[nproc - 1].start = (nproc - 1) * step;
  inter[nproc - 1].end   = (inter[nproc - 1].start+step) + ((nproc - 1) % nproc);
  inter[nproc - 1].pass  = arg;
  inter[nproc - 1].func  = func;
  pthread_create(&threads[nproc - 1], NULL, ((void *(*)(void *))partial_map),
                 &inter[nproc - 1]);
    
  for(uint8_t i = 0; i < nproc; i++){
    pthread_join(threads[i], NULL);
  }
  return ;
}
matrix* matrix_copy(matrix * m){
  matrix* res = create_matrix(m->row_c, m->column_c, m->memb);
  memcpy(res->mat, m->mat, MATBYTES(m));
  return res;
}
