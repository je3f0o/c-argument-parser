#ifndef __COMMAND_MANAGER_H__
#define __COMMAND_MANAGER_H__

#include <stddef.h>

typedef struct {
  char*  name;
  char*  desciption;
  char** aliases;
  size_t alias_length;
} Command;

typedef struct {
  Command** commands;
  size_t    length;
  size_t    capacity;
} CommandManager;

void command_print(const char* program, Command* command);

void command_manager_register(
  CommandManager* command_manager,
  Command*        command
);

#endif