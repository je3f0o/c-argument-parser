/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
* File Name   : dyn_array.c
* Created at  : 2023-10-22
* Updated at  : 2023-10-22
* Author      : jeefo
* Purpose     :
* Description :
.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.*/
#include "dyn_array.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 8

void dyn_array_reserve(DynArray* arr, size_t capacity) {
  if (arr->capacity >= capacity) return;

  size_t size   = capacity * sizeof(void*);
  void* new_ptr = malloc(size);
  memset(new_ptr, 0, size);
  if (arr->length > 0) {
    memcpy(new_ptr, arr->ptr, arr->length * sizeof(void*));
    free(arr->ptr);
  }
  arr->ptr = new_ptr;
}

void dyn_array_push(DynArray* arr, void* data) {
  if (arr->capacity == 0) {
    dyn_array_reserve(arr, INITIAL_CAP);
  } else if (arr->length >= arr->capacity) {
    dyn_array_reserve(arr, arr->capacity * 2);
  }

  arr->ptr[arr->length++] = data;
}