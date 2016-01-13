#include "stats.h"

stats_t *
stats_new(int y, int n, int d, int total) {
  stats_t *stats;

  stats = malloc(sizeof(stats_t));

  stats->y = y;
  stats->n = n;
  stats->d = d;
  stats->total = total;
  return stats;
}

void
stats_print(stats_t *stats) {
  printf("  True Pairs: %d\n", stats->y);
  printf("  Non Pairs: %d\n", stats->n);
  printf("  Doubt Pairs: %d\n", stats->d);
  printf("  Total Pairs: %d\n", stats->total);
}

void
stats_free(stats_t *stats) {

  free(stats);
}
