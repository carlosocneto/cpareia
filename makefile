NAME = cpareia
CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -pedantic -fdiagnostics-color=always `pkg-config --cflags libxml-2.0 glib-2.0`
#CFLAGS = -O3 -march=native -Wall -Wextra -pedantic -fdiagnostics-color=always `pkg-config --cflags libxml-2.0 glib-2.0`
LDFLAGS = `pkg-config --libs libxml-2.0 glib-2.0`
SRC = .
DEPS = $(wildcard $(SRC)/*.h)
CODE = $(wildcard $(SRC)/*.c)
OBJ = $(patsubst %.c,%.o,$(CODE))

.PHONY: install clean dist-gzip dist-bzip2 dist-xz dist
.SILENT: install clean dist-gzip dist-bzip2 dist-xz dist

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(NAME)
	./$(NAME) input/project.xml

val: $(NAME)
	valgrind --show-leak-kinds=all --leak-check=full ./$(NAME) input/project.xml

clean:
	rm -f $(SRC)/*.o *~ $(SRC)/*~ $(NAME)
