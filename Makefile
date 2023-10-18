CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c11
HEADERS = -I ./include

all:
	$(CC) $(CFLAGS) $(HEADERS) src/main.c src/command_manager.c -o arg_parser

clean:
	rm arg_parser