CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11 -ggdb
HEADERS = -I ./include

all:
	$(CC) $(CFLAGS) $(HEADERS) \
		src/main.c \
		src/colors.c \
		src/dyn_array.c \
		src/command_manager.c \
		-o arg_parser

clean:
	rm arg_parser