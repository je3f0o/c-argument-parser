/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : colors.c
* Created at  : 2023-10-22
* Updated at  : 2023-10-22
* Author      : jeefo
* Purpose     :
* Description :
.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#include "colors.h"
#include <stdio.h>

UnixShellStyles_t UnixShellStyles = {
  .reset         = {0, 0},

  .bold          = {1, 22},
  .dim           = {2, 22},
  .italic        = {3, 23},
  .underline     = {4, 24},
  .inverse       = {7, 27},
  .hidden        = {8, 28},
  .strikethrough = {9, 29},

  .black         = {30, 39},
  .red           = {31, 39},
  .green         = {32, 39},
  .yellow        = {33, 39},
  .blue          = {34, 39},
  .magenta       = {35, 39},
  .cyan          = {36, 39},
  .white         = {37, 39},
  .gray          = {90, 39},

  .bg_black      = {40, 49},
  .bg_red        = {41, 49},
  .bg_green      = {42, 49},
  .bg_yellow     = {43, 49},
  .bg_blue       = {44, 49},
  .bg_magenta    = {45, 49},
  .bg_cyan       = {46, 49},
  .bg_white      = {47, 49},
};

const char* apply_style(const char* text, int a, int b) {
  static char buffer[256];
  snprintf(buffer, sizeof(buffer), "\x1b[%dm%s\x1b[%dm", a, text, b);
  return buffer;
}