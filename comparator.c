#include "comparator.h"

work_t *
work_new(array_t *array, int start, int step) {
  work_t *work;

  work = malloc(sizeof(work_t));

  work->array = array;
  work->start = start;
  work->step = step;

  return work;
}

void
work_free(work_t *work) {
  free(work);
}

double
compare(comparator_t *comparator, record_t *r, record_t *s, int field) {
  int match;
  double score, freqf1, freqf2;
  array_t *array;
  char *f1, *f2;

  f1 = record_get_field(r, field);
  f2 = record_get_field(s, field);

  if(!f1 || !f2) {
    return 0;
  }

  if(comparator->exact) {
    match = !strcmp(f1, f2);
  } else {
    match = winkler(f1, f2) >= comparator->min_value_to_be_match;
  }

  if(match) {
    if(comparator->use_weight_table) {
      /*
       * Implementar tabela de peso
       * score = inverse_freq_f1 | inverse_freq_f2 | default_weight
       */
      array = (array_t *) hash_get(comparator->frequency_table, f1);
      freqf1 = *(double *)array_get(array, 0);

      array = (array_t *) hash_get(comparator->frequency_table, f2);
      freqf2 = *(double *)array_get(array, 0);

      /* We need to get the smalest inverse frequency, since it means
       * this element is more frequent.
       */
      if(!freqf1 && !freqf2) {
        score = comparator->default_weight;
      } else {
        score = MIN(freqf1, freqf2);
      }

    } else {
      score = comparator->log2_m_u;
    }
  } else {
    score = comparator->log2_1m_1u;
  }

  return score;
}

double
compare_all(
    classifier_t *classifier,
    record_t *r,
    record_t *s,
    double *scores) {
  size_t i;
  double score;

  score = 0;

  for(i = 0; i < array_size(classifier->comparators); i++) {
    scores[i] = compare(array_get(classifier->comparators, i), r, s, i);
    score += scores[i];
  }

  return score;
}

void
compare_block(work_t *work, project_t *project, int id) {
  size_t i, j, size, classes;
  record_t *r1, *r2;
  double *scores;
  double score;
  char status;

  size = array_size(work->array);
  classes = array_size(project->classifier->comparators);
  scores = malloc(sizeof(double) * classes);

  for(i = work->start; i < size - 1; i += work->step) {
    r1 = array_get(work->array, i);
    for(j = i + 1; j < size; j++) {
      r2 = array_get(work->array, j);
      score = compare_all(project->classifier, r1, r2, scores);

      if(score < project->output->min) {
        status = 'N';
      } else if(score > project->output->max) {
        status = 'Y';
      } else {
        status = '?';
      }
      if(between(score, project->output->min, project->output->max)) {
        output_write(
            record_get_field(r1, 0),
            record_get_field(r2, 0),
            status,
            score,
            scores,
            classes,
            output_get_file(project->output, id));
      }
    }
  }
  free(scores);
  work_free(work);
}

void *
compare_block_void(void *data) {
  unsigned long int i, size;
  comparator_pthread_params_t *par;

  par = data;

  size = (unsigned long int) array_size(par->project->works);

  for(i = par->id; i < size; i += par->num_threads) {
    if(!(i % 10000)) {
      printf(
          "Blocos processados: %lu de %lu (%2.2f%%)\n",
          i,
          size,
          100.0 * i / size);
    }
    compare_block(
        array_get(par->project->works, i),
        par->project,
        par->id);
  }

  return NULL;
}

void
comparator_get_block(gpointer key, gpointer ary, gpointer proj) {
  work_t *work;
  project_t *project;
  float prop;
  int i;
  size_t size;
  array_t *array;
  (void) key;

  array = (array_t *) ary;
  project = (project_t *) proj;
  size = array_size(array);

  prop = size / project->blocks_mean_size;
  prop = prop > 2 ? prop : 1;

  for(i = 0; i < prop; i++) {
    work = work_new(array, i, prop);
    array_append(project->works, work);
  }
}

int
comparator_calc_sum(gpointer key, gpointer ary, gpointer ac) {
  array_t *array;
  size_t size;
  float *acc;
  (void) key;

  array = (array_t *) ary;
  acc = (float *) ac;
  size = array_size(array);

  if(size == 1) {
    return 1;
  }

  *acc += size;

  return 0;
}

pthread_t **
comparator_run_async(project_t *project) {
  pthread_t **threads;
  comparator_pthread_params_t *param;
  int i, size;
  float acc = 0;

  threads = malloc(sizeof(pthread_t *) * project->args->max_threads);

  size = hash_size(project->blocks);
  printf("Calculando trabalho médio e removendo blocos únicos\n");
  printf("Quantidade de blocos: %d\n", size);

  hash_foreach_remove(project->blocks, comparator_calc_sum, &acc);
  size = hash_size(project->blocks);

  project->blocks_mean_size = acc / size;

  printf("Trabalho médio: %.0f registros\n", project->blocks_mean_size);
  printf("Novos blocos: %d\n", size);

  project->works = array_new(size);

  printf("Dividindo e compartilhando blocos para a comparação\n");
  hash_foreach(project->blocks, comparator_get_block, project);
  printf("Todos os blocos já foram alocados\n");
  printf("Começando comparação em si\n");

  output_open_files(project->output, project->args->max_threads);

  for(i = 0; i < project->args->max_threads; i++) {
    threads[i] = malloc(sizeof(pthread_t));
    param = malloc(sizeof(comparator_pthread_params_t));
    param->project = project;
    param->id = i;
    param->num_threads = project->args->max_threads;
    pthread_create(threads[i], NULL, compare_block_void, param);
  }

  return threads;
}
