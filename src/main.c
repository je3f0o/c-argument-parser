/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : main.c
* Created at  : 2023-10-18
* Updated at  : 2023-10-22
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

void help(const char* program, DynArray* cm) {
  printf("Usage: %s [command] <options...>\n\n", program);

  char* p = strrchr(program, '/');
  if (p) {
    program = p+1;
  }

  for (size_t i = 0; i < cm->length; ++i) {
    command_print(program, cm->ptr[i], true);
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
  };
  for (size_t i = 0; i < ARRAY_LEN(aliases); ++i) {
    dyn_array_push(&command.aliases, aliases[i]);
  }

  // Command option
  {
    CommandOption option = create_command_option(
      "command", COMMAND_OPTION_STRING
    );
    option.desciption = "Show command options specified by `--command` option";
    dyn_array_push(&command.options, &option);
    char* option_aliases[] = {"-c"};
    for (size_t i = 0; i < ARRAY_LEN(option_aliases); ++i) {
      dyn_array_push(&option.aliases, option_aliases[i]);
    }
  }

  // Command option
  bool no_color = false;
  {
    CommandOption option = create_command_option(
      "no-color", COMMAND_OPTION_BOOL
    );
    option.value      = &no_color;
    option.desciption = "Print outputs without color";
    dyn_array_push(&command.options, &option);
  }

  // Command manager
  DynArray command_manager = {0};
  dyn_array_push(&command_manager, &command);

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