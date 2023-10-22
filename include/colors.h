#ifndef __UNIX_SHELL_COLORS_H__
#define __UNIX_SHELL_COLORS_H__

typedef int UnixShellStyle[2];

typedef struct {
  UnixShellStyle reset;

  UnixShellStyle bold;
  UnixShellStyle dim;
  UnixShellStyle italic;
  UnixShellStyle underline;
  UnixShellStyle inverse;
  UnixShellStyle hidden;
  UnixShellStyle strikethrough;

  UnixShellStyle black;
  UnixShellStyle red;
  UnixShellStyle green;
  UnixShellStyle yellow;
  UnixShellStyle blue;
  UnixShellStyle magenta;
  UnixShellStyle cyan;
  UnixShellStyle white;
  UnixShellStyle gray;

  UnixShellStyle bg_black;
  UnixShellStyle bg_red;
  UnixShellStyle bg_green;
  UnixShellStyle bg_yellow;
  UnixShellStyle bg_blue;
  UnixShellStyle bg_magenta;
  UnixShellStyle bg_white;
  UnixShellStyle bg_cyan;
} UnixShellStyles_t;

extern UnixShellStyles_t UnixShellStyles;

const char* apply_style(const char* text, int a, int b);

#endif