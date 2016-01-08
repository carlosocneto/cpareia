#ifndef _BLOCKING_H_
#define _BLOCKING_H_

#include <pthread.h>

#include "array.h"
#include "record.h"
#include "project.h"
#include "field_comparator.h"
#include "hash.h"

typedef struct blocking_thread_params_t {
  project_t *project;
  int rank, total_ranks;
} blocking_thread_params_t;

void blocking_read_blocks(project_t *);
void blocking_generate_keys(project_t *, record_t *);
pthread_t **blocking_async(project_t *, int);
void blocking_print(project_t *);
void blocking_thread_params_free(blocking_thread_params_t *);

#endif
