#ifndef _RECORD_H_
#define _RECORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record {
  int id;
  int num_fields;
  char **fields;
  int _current_fields;
} record;

record *record_new(int id, int num_fields);
void record_add_field(record *, char *);
void record_free(record *);
void record_print(record *);

#endif
