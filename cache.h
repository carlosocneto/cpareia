#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdlib.h>
#include <strings.h>

typedef struct cache_t {
  size_t *_data, _size, _total;
} cache_t;

cache_t *cache_new(size_t, size_t);
void cache_free(cache_t *);
int cache_has_pair(cache_t *, size_t, size_t);

#endif
