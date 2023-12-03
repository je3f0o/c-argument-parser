/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : command_manager.c
* Created at  : 2023-10-19
* Updated at  : 2023-12-03
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
#include <assert.h>

#define INITIAL_CAP 8

static const char* command_type_to_string(CommandOptionType type) {
  switch (type) {
  case COMMAND_OPTION_STRING : return "String";
  case COMMAND_OPTION_INT    : return "Int";
  case COMMAND_OPTION_UINT   : return "UInt";
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
    printf("%s", (char*)option->value);
    break;
  case COMMAND_OPTION_INT:
    printf("%d", *(int*)option->value);
    break;
  case COMMAND_OPTION_UINT:
    printf("%d", *(uint32_t*)option->value);
    break;
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

void command_parse_options(Command* c, int* argc, char*** argv) {
  static char buffer[64];
  arg_shift(argc, argv);

  while (*argc > 0) {
    bool is_option_found = false;
    for (size_t i = 0; i < c->options.length; ++i) {
      CommandOption* o = c->options.ptr[i];
      snprintf(buffer, sizeof(buffer), "--%s", o->name);

      if (strcmp(buffer, *(argv[0])) == 0) {
        switch (o->type) {
        case COMMAND_OPTION_STRING:
        case COMMAND_OPTION_INT:
        case COMMAND_OPTION_UINT:
        case COMMAND_OPTION_MAX:
          assert(false && "Unimplemented");
          break;
        case COMMAND_OPTION_BOOL:
          o->value        = (void*)true;
          o->is_value_set = true;
          is_option_found = true;
          break;
        }
      }
    }

    if (is_option_found) {
      if (*argc > 0) arg_shift(argc, argv);
    } else {
      break;
    }
  }
}