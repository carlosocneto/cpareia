#include "classifier.h"

void
comparator_read_frequency_table(comparator_t *comparator) {
  char *p, *key, *name, line[200];
  double *field_value;
  FILE *fh;

  name = comparator->frequency_table_name;

  comparator->frequency_table = hash_new();

  if (name && strlen(name)) {
    check_file(name);

    fh = fopen(name, "r");

    while (fgets(line, sizeof(line), fh))  {
      p = strtok(line, " ");
      key = p;

      p = strtok(NULL, " ");
      field_value = malloc(sizeof(double));
      *field_value = atof(p);

      hash_insert(comparator->frequency_table, key, field_value);
    }
  }
}

comparator_t *
comparator_new(
    int exact,
    int use_weight_table,
    double m,
    double u,
    double missing,
    int field1,
    char *frequency_table_name,
    char *function,
    double min_value_to_be_match,
    double default_weight) {
  comparator_t *comparator;

  comparator = malloc(sizeof(comparator_t));

  comparator->exact = exact;
  comparator->use_weight_table = use_weight_table;
  comparator->m = m;
  comparator->u = u;
  comparator->missing = missing;
  comparator->field1 = field1;
  comparator->function = function;
  comparator->min_value_to_be_match = min_value_to_be_match;
  comparator->default_weight = default_weight;
  comparator->frequency_table_name = frequency_table_name;

  comparator->log2_m_u = log2(m / u);
  comparator->log2_1m_1u = log2(1 - m / 1 - u);

  comparator_read_frequency_table(comparator);

  return comparator;
}

void
comparator_print(comparator_t *comparator) {
  printf("  Exact: %d\n", comparator->exact);
  printf("  Use_weight_table: %d\n", comparator->use_weight_table);
  printf("  M: %f\n", comparator->m);
  printf("  U: %f\n", comparator->u);
  printf("  Missing: %f\n", comparator->missing);
  printf("  Field1: %d\n", comparator->field1);

  /*
   *How to Get a "Name" for the Hash. Is it needed?
   *printf("  Frequency_table: %s\n", comparator->frequency_table);
  */
  printf("  Function: %s\n", comparator->function);
  printf("  Min_value_to_be_match: %f\n", comparator->min_value_to_be_match);
  printf("  Default_weight: %f\n", comparator->default_weight);
  printf("  Log2(m, u): %f\n", comparator->log2_m_u);
  printf("  Log2(1 - m, 1 - u): %f\n", comparator->log2_1m_1u);
}

void
comparator_free(comparator_t *comparator) {
  free(comparator->function);
  free(comparator->frequency_table_name);
  hash_free(comparator->frequency_table);
  free(comparator);
}

classifier_t *
classifier_new(int match_min, int not_match_max) {
  classifier_t *classifier;
  classifier = malloc(sizeof(classifier_t));

  classifier->match_min = match_min;
  classifier->not_match_max = not_match_max;

  classifier->comparators = array_new(1);

  return classifier;
}

void
classifier_add_comparator(
    classifier_t *classifier,
    comparator_t *comparator) {
    array_append(classifier->comparators, comparator);
}

void
classifier_free(classifier_t *classifier) {
  size_t i;

  for(i = 0; i < array_size(classifier->comparators); i++) {
    comparator_free(array_get(classifier->comparators, i));
  }
  array_free(classifier->comparators);

  free(classifier);
}

void
classifier_print(classifier_t *classifier) {
  size_t i;

  printf("    match_min: %d\n", classifier->match_min);
  printf("    not_match_max: %d\n", classifier->not_match_max);

  printf("\nComparators:");
  for(i = 0; i < array_size(classifier->comparators); i++) {
    printf("\n  Comparator %d\n", (int) i);
    comparator_print(array_get(classifier->comparators, i));
  }
}
