/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : main.c
* Created at  : 2023-10-18
* Updated at  : 2023-10-19
* Author      : jeefo
* Purpose     :
* Description :
.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#include "command_manager.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))

void shift(int* argc, char*** argv) {
  *argc -= 1;
  (*argv)++;
}

void help(const char* program, CommandManager* cm) {
  printf("Usage: %s [command] <options...>\n\n", program);

  char* p = strrchr(program, '/');
  if (p) {
    program = p+1;
  }

  for (size_t i = 0; i < cm->length; ++i) {
    command_print(program, cm->commands[i]);
  }
}

int main(int argc, char** argv) {
  const char* program = argv[0];
  shift(&argc, &argv);

  char* aliases[] = {
    "-h", "h", "--help", "?"
  };
  Command command = {
    .name         = "help",
    .desciption   = "Print commands and options description and exit",
    .aliases      = malloc(ARRAY_LEN(aliases) * sizeof(char*)),
    .alias_length = ARRAY_LEN(aliases),
  };
  for (size_t i = 0; i < ARRAY_LEN(aliases); ++i) {
    command.aliases[i] = aliases[i];
  }

  CommandManager command_manager = {0};
  command_manager_register(&command_manager, &command);

  if (argc == 0) {
    help(program, &command_manager);
    return 0;
  }

  while (argc > 0) {
    if (strcmp(argv[0], "help") == 0) {
      help(program, &command_manager);
    }
    shift(&argc, &argv);
  }

  return 0;
}