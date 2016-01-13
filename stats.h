#ifndef _STATS_H_
#define _STATS_H_

#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include "record.h"
#include "errors.h"

typedef struct stats_t {
  int y;
  int n;
  int d;
  int total;
} stats_t;

stats_t *stats_new(int, int,int,int);
void stats_free(stats_t *);
void stats_print(stats_t *);

#endif
