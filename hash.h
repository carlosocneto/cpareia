#ifndef _HASH_H_
#define _HASH_H_

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct hash {
  GHashTable *table;
  int last_id;
} hash;

hash * hash_new();
void hash_free(hash *);
int hash_get_id(hash *, char *);
void hash_print(hash *);

#endif