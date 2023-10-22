#ifndef __COMMAND_MANAGER_H__
#define __COMMAND_MANAGER_H__

#include "dyn_array.h"
#include <stdbool.h>

typedef enum {
  COMMAND_OPTION_STRING,
  COMMAND_OPTION_INT,
  COMMAND_OPTION_UINT,
  COMMAND_OPTION_BOOL,
  COMMAND_OPTION_MAX
} CommandOptionType;

typedef struct {
  const char*       name;
  const char*       desciption;
  CommandOptionType type;
  void*             value;
  DynArray          aliases;
} CommandOption;

typedef struct {
  const char* name;
  const char* desciption;
  DynArray    aliases;
  DynArray    options;
} Command;

typedef struct {
  Command** commands;
  size_t    length;
  size_t    capacity;
} CommandManager;

// Public API
void command_print(const char* program,
                   Command*    command,
                   bool        has_color);
CommandOption create_command_option(const char* name, CommandOptionType type);

#endif