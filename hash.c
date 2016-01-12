#include "hash.h"

hash_t *
hash_new() {
  hash_t *hash;

  hash = malloc(sizeof(hash_t));

  hash->table = kh_init(str);

  pthread_mutex_init(&hash->mutex, NULL);

  return hash;
}

void
hash_free(hash_t *hash) {
  array_t *array;
  const char *key;

  kh_foreach(
      hash->table,
      key,
      array,
      array_free(array);free((void *) key));

  kh_destroy(str, hash->table);

  pthread_mutex_destroy(&hash->mutex);

  free(hash);
}

void
hash_insert(hash_t *hash, char *key, void *record) {
  int absent;
  array_t *array;
  khint_t k;
  char *keydup;

  pthread_mutex_lock(&hash->mutex);

  keydup = strdup(key);

  if((k = kh_get(str, hash->table, keydup)) == kh_end(hash->table)) {
    k = kh_put(str, hash->table, keydup, &absent);
    kh_value(hash->table, k) = array_new(1);
  } else {
    free(keydup);
    keydup = key;
  }

  k = kh_get(str, hash->table, keydup);

  array = kh_val(hash->table, k);

  array_append(array, record);

  pthread_mutex_unlock(&hash->mutex);
}

void
record_void_print(void *record) {
  record_print(record);
}

array_t *
hash_get(hash_t *hash, char *key) {
  khint_t k;

  k = kh_get(str, hash->table, key);
  return kh_val(hash->table, k);
}

void
hash_print_pair(const char *key, array_t *array, void *data) {
  (void) data;
  printf("  %s =>\n", key);
  array_print(array, record_void_print);
}

void
hash_internal_remove(hash_t *hash, const char *key, array_t *array) {
  khint_t k;

  k = kh_get(str, hash->table, key);
  kh_del(str, hash->table, k);
  array_free(array);
}

void
hash_foreach_remove(hash_t *hash, hash_foreach_rm_fn fn, void *data) {
  array_t *array;
  const char *key;

  kh_foreach(
      hash->table,
      key,
      array,
      if(fn(key, array, data))
      hash_internal_remove(hash, key, array)
      );
}

void
hash_foreach(hash_t *hash, hash_foreach_fn fn, void *data) {
  array_t *array;
  const char *key;

  kh_foreach(hash->table, key, array, fn(key, array, data));
}

size_t
hash_size(hash_t *hash) {
  return kh_size(hash->table);
}

void
hash_print(hash_t *hash) {
  printf("{\n");
  hash_foreach(hash, hash_print_pair, NULL);
  printf("}\n");
}
