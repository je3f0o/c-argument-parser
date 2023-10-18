/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : command_manager.c
* Created at  : 2023-10-19
* Updated at  : 2023-10-19
* Author      : jeefo
* Purpose     :
* Description :
.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#include "command_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 8

static void reallocate(CommandManager* cm) {
  if (cm->capacity == 0) {
    cm->length   = 0;
    cm->capacity = INITIAL_CAP;
    cm->commands = malloc(sizeof(Command) * INITIAL_CAP);
    return;
  }

  cm->capacity  = cm->capacity * 2;
  size_t size   = cm->capacity * sizeof(Command);
  void* new_ptr = malloc(size);
  memset(new_ptr, 0, size);
  memcpy(new_ptr, cm->commands, cm->length * sizeof(Command));
}

void command_manager_register(
  CommandManager* cm,
  Command*        command
) {
  if (cm->length >= cm->capacity) {
    reallocate(cm);
  }

  Command* clone = malloc(sizeof(Command));
  memcpy(clone, command, sizeof(Command));
  cm->commands[cm->length++] = clone;
}

void command_print(const char* program, Command* command) {
  printf("%s %s <options...>\n", program, command->name);
  if (command->desciption) {
    printf("  %s\n", command->desciption);
  }
  if (command->alias_length > 0) {
    printf("  aliases: %s", command->aliases[0]);
    for (size_t i = 1; i < command->alias_length; ++i) {
      printf(", %s", command->aliases[i]);
    }
    puts("");
  }
}