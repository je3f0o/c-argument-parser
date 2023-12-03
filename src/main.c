/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : main.c
* Created at  : 2023-10-18
* Updated at  : 2023-12-03
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

void help(const char* program, DynArray* cm, Command* c) {
  (void) c;
  printf("Usage: %s [command] <options...>\n\n", program);

  char* p = strrchr(program, '/');
  if (p) {
    program = p+1;
  }

  bool has_color = true;
  for (size_t i = 0; i < c->options.length; ++i) {
    CommandOption* o = c->options.ptr[i];
    if (strcmp(o->name, "no-color") == 0) {
      has_color = !((bool) (o->is_value_set ? o->value : o->default_value));
      break;
    }
  }

  for (size_t i = 0; i < cm->length; ++i) {
    command_print(program, cm->ptr[i], has_color);
  }
}

void foo(const char* program, DynArray* cm, Command* c) {
  (void) program;
  (void) cm;
  (void) c;
  puts("Foo command");
}

int main(int argc, char** argv) {
  const char* program = argv[0];
  arg_shift(&argc, &argv);

  // Help command
  char* aliases[] = {
    "-h", "h", "--help", "?"
  };
  Command command = {
    .name         = "help",
    .desciption   = "Print commands and options description and exit",
    .execute      = help,
  };
  for (size_t i = 0; i < ARRAY_LEN(aliases); ++i) {
    dyn_array_push(&command.aliases, aliases[i]);
  }

  // Help command's option
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

  // Help command's option
  {
    CommandOption option = create_command_option(
      "no-color", COMMAND_OPTION_BOOL
    );
    option.desciption        = "Print outputs without color";
    option.default_value     = false;
    option.has_default_value = true;
    dyn_array_push(&command.options, &option);
  }

  // Foo command
  Command foo_command = {
    .name         = "foo",
    .desciption   = "Bar",
    .execute      = foo,
  };

  // Command manager
  DynArray command_manager = {0};
  dyn_array_push(&command_manager, &command);
  dyn_array_push(&command_manager, &foo_command);

  if (argc == 0) {
    help(program, &command_manager, NULL);
    return 0;
  }

  while (argc > 0) {
    bool is_command_found = false;
    for (size_t i = 0; i < command_manager.length && !is_command_found; ++i) {
      Command* c = command_manager.ptr[i];
      if (strcmp(c->name, argv[0]) == 0) {
        command_parse_options(c, &argc, &argv);
        c->execute(program, &command_manager, c);
        is_command_found = true;
        break;
      }
      for (size_t j = 0; j < c->aliases.length; ++j) {
        const char* alias_name = c->aliases.ptr[j];
        if (strcmp(alias_name, argv[0]) == 0) {
          command_parse_options(c, &argc, &argv);
          c->execute(program, &command_manager, c);
          is_command_found = true;
          break;
        }
      }
    }

    if (!is_command_found) {
      fprintf(stderr, "ERROR: couldn't find '%s' command\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  return EXIT_SUCCESS;
}