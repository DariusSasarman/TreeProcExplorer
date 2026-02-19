CC = gcc

CFLAGS = -Wall -O2 -g -Igraphics/raylib_files
LDFLAGS =
LDLIBS = graphics/raylib_files/libraylib.a -lm -lpthread -ldl -lrt -lX11

SRC = main.c \
      process_monitor/list_processes.c \
      graphics/visual_representation.c \
      translation_between_forms/translator.c

OBJ = $(SRC:.c=.o)

TreeProcExplorer: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) TreeProcExplorer
