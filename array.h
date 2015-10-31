#ifndef _ARRAY_H_
#define _ARRAY_H_
#define INITIAL_SIZE 10000
#define BUCKET_SIZE 20

#include <stdlib.h>
#include <math.h>

typedef struct array {
  void ***_buckets;
  size_t _size, _total_size, _total_buckets_size, _bucket_size;
} array;

typedef void (*array_fn_print)(void *);

array * array_new();
array * array_new_prealloc(size_t);
void array_print(array *, array_fn_print);
void array_fini(array *);
void array_free(array *);
void *array_remove_last(array *);
void array_append(array *, void *);
void array_add_at(array *, void *, size_t);
void *array_get(array *, size_t);
void *array_get_last(array *);
size_t array_size(array *);

#endif
