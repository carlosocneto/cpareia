#include "errors.h"

void handle_error(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
