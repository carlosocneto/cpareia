#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include "record.h"
#include "errors.h"
#include "stats.h"

typedef struct output_t {
  double min, max;
  char *filename;
  int num_files;
  gzFile *files;
  int no_print; 
  stats_t *stats;
} output_t;

output_t *output_new(char *, double, double,int,stats_t *);
void output_open_files(output_t *, int);
void output_free(output_t *);
void output_print(output_t *);
gzFile output_get_file(output_t *, int);
void output_write(output_t *, char *, char *, char, double, double *, int, int);

#endif
