#include "cache.h"

cache_t *
cache_new(size_t size, size_t total) {
  cache_t *cache;

  cache = malloc(sizeof(cache_t));
  cache->_data = malloc(sizeof(size_t) * size);
  bzero(cache->_data, sizeof(size_t) * size);
  cache->_total = total;
  cache->_size = size;

  return cache;
}

void
cache_free(cache_t *cache) {
  free(cache->_data);
  free(cache);
}

size_t
cache_hash(size_t key) {
  key += ~(key << 15);
  key ^=  (key >> 10);
  key +=  (key << 3);
  key ^=  (key >> 6);
  key += ~(key << 11);
  key ^=  (key >> 16);
  return key;
}

int
cache_has_pair(cache_t *cache, size_t i, size_t j) {
  size_t hash, index, n, min, max, el;

  min = i < j ? i : j;
  max = i > j ? i : j;

  n = cache->_total;

  index = (n*(n-1)/2)-(n-min)*(n-min-1)/2+max-min-1;
  hash = cache_hash(index) % n;

  el = cache->_data[hash];

  if(el == index) {
    return 1;
  } else {
    cache->_data[hash] = index;
    return 0;
  }
}
