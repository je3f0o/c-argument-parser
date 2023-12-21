#ifndef __COMMAND_MANAGER_H__
#define __COMMAND_MANAGER_H__

#include "dyn_array.h"
#include <stdbool.h>

typedef struct Command Command;

typedef void (*command_execute_t)(
  const char* program,
  DynArray*   cm,
  Command*    command
);

typedef enum {
  COMMAND_OPTION_STRING,
  COMMAND_OPTION_INT,
  COMMAND_OPTION_BOOL,
  COMMAND_OPTION_COUNT
} CommandOptionType;

typedef struct {
  const char*       name;
  const char*       desciption;
  CommandOptionType type;
  void*             value;
  void*             default_value;
  bool              is_value_set;
  bool              has_default_value;
  DynArray          aliases;
} CommandOption;

struct Command {
  const char*       name;
  const char*       desciption;
  DynArray          aliases;
  DynArray          options;
  command_execute_t execute;
};

typedef struct {
  Command** commands;
  size_t    length;
  size_t    capacity;
} CommandManager;

// Public API
void arg_shift(int* argc, char*** argv);
void command_print(const char* program,
                   Command*    command,
                   bool        has_color);
CommandOption create_command_option(const char* name, CommandOptionType type);

void command_parse_options(Command* c, int* argc, char*** argv);
void command_parse(int argc, char** argv, char const* program, DynArray* cm);

#endif