#include "array.h"

array_t *
array_new() {
  return array_new_prealloc(INITIAL_SIZE);
}

array_t *
array_new_prealloc(size_t size) {
  array_t *array;

  array = malloc(sizeof(array_t));

  array->_size = 0;
  array->_total_size = size;
  array->_data = malloc(sizeof(void *) * size);

  return array;
}

void
array_fini(array_t *array) {
  array->_data = realloc(array->_data, sizeof(void *) * array->_size);
  array->_total_size = array->_size;
}

void
array_free(array_t *array) {
  free(array->_data);
  free(array);
}

void *
array_remove_last(array_t *array) {
  return array->_data[--array->_size];
}

void
array_realloc(array_t *array) {
  array->_total_size *= 2;
  array->_data =
    realloc(array->_data, sizeof(void *) * array->_total_size);
}

void
array_add_at(array_t *array, void *el, size_t i) {
  if(array->_size == array->_total_size) {
    array_realloc(array);
  }
  array->_data[i] = el;
  array->_size++;
}

void
array_append(array_t *array, void *el) {
  array_add_at(array, el, array->_size);
}

inline void *
array_get(array_t *array, size_t i) {
  return i < array->_size ? array->_data[i] : NULL;
}

inline void *
array_get_last(array_t *array) {
  return array->_data[array->_size -1];
}

inline size_t
array_size(array_t *array) {
  return array->_size;
}

void
array_print(array_t *array, array_fn_print fn_print) {
  size_t i;

  for(i = 0; i < array_size(array); i++) {
    fn_print(array_get(array, i));
  }
}
