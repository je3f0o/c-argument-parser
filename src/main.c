/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name   : main.c
* Created at  : 2023-10-18
 * Updated at  : 2023-12-22
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

void print_specified_command(
  char const* program,
  DynArray*   cm,
  char const* command_name,
  bool        has_color
) {
  for (size_t i = 0; i < cm->length; ++i) {
    Command* command = cm->ptr[i];
    if (strcmp(command->name, command_name) == 0) {
      puts("");
      command_print(program, cm->ptr[i], has_color);
      return;
    }
  }

  fprintf(stderr,
    "ERROR: specified command option '%s' didn't found in 'help' command.\n",
    command_name
  );
  exit(EXIT_FAILURE);
}

void help(const char* program, DynArray* cm, Command* c) {
  printf("Usage: %s [command] <options...>\n", program);

  char* p = strrchr(program, '/');
  if (p) program = p+1;

  bool  has_color    = true;
  char* command_name = NULL;
  for (size_t i = 0; i < c->options.length; ++i) {
    CommandOption* o = c->options.ptr[i];
    if (strcmp(o->name, "no-color") == 0) {
      has_color = !((bool) (o->is_value_set ? o->value : o->default_value));
    } else if (strcmp(o->name, "command") == 0) {
      if (o->is_value_set) {
        command_name = o->value;
      }
    }
  }

  if (command_name != NULL) {
    print_specified_command(program, cm, command_name, has_color);
    return;
  }

  for (size_t i = 0; i < cm->length; ++i) {
    puts("");
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
  char const* program = argv[0];
  arg_shift(&argc, &argv);

  // Help command
  char* aliases[] = {
    "-h", "h", "--help", "?"
  };
  Command help_command = {
    .name         = "help",
    .desciption   = "Print commands and options description and exit",
    .execute      = help,
  };
  for (size_t i = 0; i < ARRAY_LEN(aliases); ++i) {
    dyn_array_push(&help_command.aliases, aliases[i]);
  }

  // Help -> command option
  char* option_aliases[] = {"-c"};
  {
    CommandOption option = create_command_option(
      "command", COMMAND_OPTION_STRING
    );
    option.desciption = "Show command options specified by `--command` option";
    dyn_array_push(&help_command.options, &option);
    for (size_t i = 0; i < ARRAY_LEN(option_aliases); ++i) {
      dyn_array_push(&option.aliases, option_aliases[i]);
    }
  }

  // Help -> number
  {
    CommandOption option = create_command_option(
      "log-level", COMMAND_OPTION_INT
    );
    option.desciption    = "Set verbosity of log level, 0 will be disable logs";
    option.default_value     = 0;
    option.has_default_value = true;
    dyn_array_push(&help_command.options, &option);
    static char* log_alias = "-l";
    dyn_array_push(&option.aliases, log_alias);
  }

  // Help -> 'no-color' option
  {
    CommandOption option = create_command_option(
      "no-color", COMMAND_OPTION_BOOL
    );
    option.desciption        = "Print outputs without color";
    option.default_value     = false;
    option.has_default_value = true;
    dyn_array_push(&help_command.options, &option);
  }

  // Foo command
  Command foo_command = {
    .name         = "foo",
    .desciption   = "Foo command description",
    .execute      = foo,
  };

  // Command manager
  DynArray command_manager = {0};
  dyn_array_push(&command_manager, &help_command);
  dyn_array_push(&command_manager, &foo_command);

  if (argc == 0) {
    help(program, &command_manager, &help_command);
    return 0;
  }

  command_parse(argc, argv, program, &command_manager);

  return EXIT_SUCCESS;
}