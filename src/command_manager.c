/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name   : command_manager.c
* Created at  : 2023-10-19
 * Updated at  : 2023-12-22
* Author      : jeefo
* Purpose     :
* Description :
.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#include "command_manager.h"
#include "colors.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#define INITIAL_CAP 8

static const char* command_type_to_string(CommandOptionType type) {
  switch (type) {
  case COMMAND_OPTION_STRING : return "String";
  case COMMAND_OPTION_INT    : return "Int";
  case COMMAND_OPTION_BOOL   : return "Boolean";
  default:
    assert(false && "INVALID OPTION TYPE");
  }
}

static void print_aliases(
  DynArray* aliases,
  size_t    indentation,
  bool      has_color
) {
  if (aliases->length > 0) {
    for (size_t i = 0; i < indentation; ++i) {
      putc(' ', stdout);
    }

    if (has_color) {
      printf("\x1b[%dm", UnixShellStyles.gray[0]);
    }

    printf("aliases: %s", (char*)aliases->ptr[0]);
    for (size_t i = 1; i < aliases->length; ++i) {
      printf(", %s", (char*)aliases->ptr[i]);
    }

    if (has_color) {
      printf("\x1b[%dm", UnixShellStyles.gray[1]);
    }

    puts("");
  }
}

static void print_option_default_value(CommandOption* option) {
  printf(" and (Default => ");
  switch (option->type) {
  case COMMAND_OPTION_STRING:
    printf("%s", (char*)option->default_value);
    break;
  case COMMAND_OPTION_INT: {
    int i = (int)(intptr_t)option->default_value;
    printf("%d", i);
  } break;
  case COMMAND_OPTION_BOOL: {
    bool v = (bool)option->default_value;
    printf("%s", v ? "true" : "false");
  } break;
  default:
    assert(false && "INVALID OPTION");
  }
  printf(")");
}

static void print_option(CommandOption* option, bool has_color) {
  if (has_color) printf("\x1b[%dm", UnixShellStyles.cyan[0]);
  printf("  --%s (%s)", option->name, command_type_to_string(option->type));

  if (option->has_default_value) {
    print_option_default_value(option);
  }

  if (has_color) printf("\x1b[%dm", UnixShellStyles.cyan[1]);
  puts("");

  if (option->desciption) {
    printf("    %s\n", option->desciption);
  }

  print_aliases(&option->aliases, 4, has_color);
}

static const char* cyan_text(const char* text) {
  return apply_style(text, UnixShellStyles.cyan[0], UnixShellStyles.cyan[1]);
}

static void print_command_with_colors(const char* program, Command* command) {
  printf("%s %s %s\n", program, command->name, cyan_text("<options...>"));
  if (command->desciption) {
    printf("  %s\n", command->desciption);
  }
  print_aliases(&command->aliases, 2, true);

  for (size_t i = 0; i < command->options.length; ++i) {
    print_option(command->options.ptr[i], true);
  }
}

void arg_shift(int* argc, char*** argv) {
  *argc -= 1;
  (*argv)++;
}

void command_print(const char* program, Command* command, bool has_color) {
  if (has_color) {
    print_command_with_colors(program, command);
    return;
  }

  printf("%s %s <options...>\n", program, command->name);
  if (command->desciption) {
    printf("  %s\n", command->desciption);
  }
  print_aliases(&command->aliases, 2, false);

  for (size_t i = 0; i < command->options.length; ++i) {
    print_option(command->options.ptr[i], false);
  }
}

CommandOption create_command_option(const char* name, CommandOptionType type) {
  return (CommandOption) {
    .name = name,
    .type = type,
  };
}

bool is_option_match(CommandOption* option, char const* arg) {
  static char buffer[64];
  snprintf(buffer, sizeof(buffer), "--%s", option->name);
  if (strcmp(buffer, arg) == 0) return true;

  for (size_t i = 0; i < option->aliases.length; ++i) {
    const char* alias = option->aliases.ptr[i];
    if (strcmp(alias, arg) == 0) return true;
  }

  return false;
}

void command_parse_options(Command* c, int* argc, char*** argv) {
  arg_shift(argc, argv);

  for (size_t i = 0; i < c->options.length && *argc > 0; ++i) {
    bool is_option_found = false;
    CommandOption* o = c->options.ptr[i];
    char* arg = *(argv[0]);

    if (is_option_match(o, arg)) {
      switch (o->type) {
      case COMMAND_OPTION_INT:
        if (*argc > 0) {
          arg_shift(argc, argv);
        } else {
          fprintf(stderr, "ERROR: argument value is required\n");
          exit(EXIT_FAILURE);
        }

        int num = atoi(*(argv[0]));
        if (errno != 0) {
          fprintf(stderr, "ERROR: option '--%s' is not a number.\n", o->name);
          exit(EXIT_FAILURE);
        }

        o->value        = (void*)(intptr_t)num;
        o->is_value_set = true;
        break;
      case COMMAND_OPTION_STRING:
        if (*argc > 0) {
          arg_shift(argc, argv);
        } else {
          fprintf(stderr, "ERROR: argument value is required\n");
          exit(EXIT_FAILURE);
        }

        o->value        = *(argv[0]);
        o->is_value_set = true;
        break;
      case COMMAND_OPTION_BOOL:
        o->value        = (void*)true;
        o->is_value_set = true;
        break;
      default:
        assert(false && "UNREACHABLE");
      }
      is_option_found = true;
    }

    if (is_option_found && *argc > 0) {
      arg_shift(argc, argv);
    }
  }
}

void command_parse(int argc, char** argv, char const* program, DynArray* cm) {
  while (argc > 0) {
    bool is_command_found = false;
    for (size_t i = 0; i < cm->length && !is_command_found; ++i) {
      Command* c = cm->ptr[i];
      if (strcmp(c->name, argv[0]) == 0) {
        command_parse_options(c, &argc, &argv);
        c->execute(program, cm, c);
        is_command_found = true;
        break;
      }
      for (size_t j = 0; j < c->aliases.length; ++j) {
        const char* alias_name = c->aliases.ptr[j];
        if (strcmp(alias_name, argv[0]) == 0) {
          command_parse_options(c, &argc, &argv);
          c->execute(program, cm, c);
          is_command_found = true;
          break;
        }
      }
    }

    if (!is_command_found && argc > 0) {
      fprintf(stderr, "ERROR: couldn't find '%s' command\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}