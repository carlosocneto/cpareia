#include "blocking.h"


void
blocking_thread_params_free(blocking_thread_params_t *params) {
  free(params);
}

void
blocking_generate_keys(project_t *project, record_t *record) {
  size_t i, j;
  conjunction_t *conjunction;
  part_t *part;
  char buffer[5], key[1024];

  for(i = 0; i < array_size(project->conjunctions); i++) {
    conjunction = array_get(project->conjunctions, i);
    key[0] = '\0';

    for(j = 0; j < array_size(conjunction->parts); j++) {
      part = array_get(conjunction->parts, j);

      if(!part->transform) {
        strcat(key, record->fields[part->field]);
      } else if(!strcmp(part->transform, "brsoundex")) {
        brsoundex(record->fields[part->field], buffer, 5);
        strcat(key, buffer);
      }
      else {
        handle_error("Unknown transformation");
      }
    }
    if(strlen(key)) {
      hash_insert(project->blocks, key, record);
    }
  }
}

void *
blocking_generate_all_keys(void *data) {
  size_t i;
  int rank, total_ranks;
  project_t *project;
  blocking_thread_params_t *param;
  record_t *record;

  param = data;

  rank = param->rank;
  project = param->project;
  total_ranks = param->total_ranks;

  for(i = rank; i < array_total_size(project->d0->records); i += total_ranks) {
    while(!(record = array_get(project->d0->records, i))) {
      sleep(0.2);
    }
    blocking_generate_keys(project, record);
  }

  blocking_thread_params_free(param);

  return NULL;
}

pthread_t **
blocking_async(project_t *project, int num_threads) {
  pthread_t **threads;
  blocking_thread_params_t *param;
  int i;

  threads = malloc(sizeof(pthread_t *) * num_threads);

  for(i = 0; i < num_threads; i++) {
    threads[i] = malloc(sizeof(pthread_t));
    param = malloc(sizeof(blocking_thread_params_t));
    param->project = project;
    param->rank = i;
    param->total_ranks = num_threads;

    pthread_create(threads[i], NULL, blocking_generate_all_keys, param);
  }

  return threads;
}

void 
read_block(project_t *project) {

  char *path;
  path = project->blockslist;

  //project->blockslist;
  //open and get the file handle
  FILE * fh;
  fh = fopen(path, "r");

  //check if file exists
  if (fh == NULL){
    printf("File does not exists %s", path);
    return;
  }

  //read line by line
  const size_t line_size = 300;
  
  char* line = malloc(line_size);
  char *k;
  char *p;
  char *key;
    
  while (fgets(line, line_size, fh) != NULL)  {
    p = strtok(line, ":");
    key = p;

    p = strtok(NULL, ":");
    int total = atoi(p);

    p = strtok(NULL, ":");

    for (int i=0;i<total;i++) { 
      k = strtok(p, " ");
      while( k != NULL ) {
          int id = atoi(k+1);
          record_t *record = array_get(project->d0->records, id);
          hash_insert(project->blocks, key, record);
          k = strtok(NULL, " ");
      }
    }
  }

  free(line);   
}


void
blocking_print(project_t *project) {
  hash_print(project->blocks);
}
