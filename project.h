#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "database.h"
#include "conjunction.h"
#include "classifier.h"
#include "hash.h"
#include "output.h"

typedef struct project_t {
  unsigned char *name;
  unsigned char *task;
  database_t *d0;
  array_t *conjunctions;
  hash_t *blocks;
  classifier_t *classifier;
  output_t *output;
  unsigned char *blockslist;
  float blocks_mean_size;
  array_t *works;
} project_t;


project_t *project_new();
void project_print(project_t *);
void project_free(project_t *);
void project_parse(project_t *, char *);
int project_get_field_id(project_t *, char*);



#endif
