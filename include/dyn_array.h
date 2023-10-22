#ifndef __DYN_ARRAY_H__
#define __DYN_ARRAY_H__

#include <stddef.h>

typedef struct {
  void**  ptr;
  size_t  length;
  size_t  capacity;
} DynArray;

void dyn_array_reserve(DynArray* arr, size_t capacity);
void dyn_array_push(DynArray* arr, void* data);

#endif