#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#	include "time.h"
#endif

////////////////////////////////
//~ allen: Files

typedef U32 FilePropertyFlags;
enum
{
  FilePropertyFlag_IsFolder = (1 << 0),
};

typedef struct FileProperties FileProperties;
struct FileProperties
{
  U64 size;
  DenseTime modified;
  DenseTime created;
  FilePropertyFlags flags;
};
