#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <pthread.h>

#include "record.h"
#include "errors.h"
#include "array.h"
#include "csv.h"

typedef struct database_t {
  array_t *records;
  unsigned char **fields, sep;
  size_t num_fields;
  char *filename;
} database_t;

typedef void (*database_cb)(void *, void *);

database_t *database_new(int num_fields, size_t num_rows);
void database_free(database_t *);
void database_read(database_t *, database_cb, void *);
pthread_t *database_read_async(database_t *);
void database_print(database_t *);
void database_fini(database_t *);

#endif
