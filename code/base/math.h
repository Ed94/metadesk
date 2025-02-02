#if MD_INTELLISENSE_DIRECTIVES
#pragma once
#include "base_types.h"
#endif

typedef union Rng1U64 Rng1U64;
union Rng1U64
{
  struct
  {
    U64 min;
    U64 max;
  };
  U64 v[2];
};
