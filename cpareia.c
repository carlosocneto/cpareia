#include "cpareia.h"

int
delta(struct timeval a, struct timeval b) {
  return (int) (b.tv_sec - a.tv_sec);
}

int
main(int argc, char *argv[]) {
  int i;
  project_t *project;
  pthread_t *read_thread;
  pthread_t **blocking_threads;
  pthread_t **comparator_threads;
  long max_threads;

  if(argc != 3)
    handle_error("Usage: cpareia XML -b(Start with a file of Blocking) or -f(To do the full work)\n");

  max_threads = sysconf(_SC_NPROCESSORS_ONLN);

  project = project_new();

  project_parse(project, argv[1]);

  printf("Começando leitura e blocagem\n");
  read_thread = database_read_async(project->d0);

  if ( argv[3] == "-f") {
    blocking_threads = blocking_async(project, max_threads - 1);

    pthread_join(*read_thread, NULL);
    printf("Continuando blocagem\n");

    for(i = 0; i < max_threads - 1; i++) {
      pthread_join(*blocking_threads[i], NULL);
      free(blocking_threads[i]);
    }
    free(blocking_threads);
  }

  if ( argv[3] == "-b") {
    read_blocks(project, )
  }

  free(read_thread);
  printf("Blocagem pronta\n\nComeçando comparação e escrita\n");
  comparator_threads = comparator_run_async(project, max_threads);

  for(i = 0; i < max_threads - 1; i++) {
    pthread_join(*comparator_threads[i], NULL);
    free(comparator_threads[i]);
  }

  free(comparator_threads);

  printf("Comparação pronta\n");

  project_free(project);
  return 0;
}
