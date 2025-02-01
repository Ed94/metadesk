#if MD_INTELLISENSE_DIRECTIVES
#pragma once
#include "base_types.h"
#endif

typedef struct String8 String8;
struct String8
{
  U8 *str;
  U64 size;
};

