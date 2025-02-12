#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#	include "time.h"
#endif

////////////////////////////////
//~ allen: Files

typedef MD_U32 MD_FilePropertyFlags;
enum
{
	MD_FilePropertyFlag_IsFolder = (1 << 0),
};

typedef struct MD_FileProperties MD_FileProperties;
struct MD_FileProperties
{
	MD_U64               size;
	MD_DenseTime         modified;
	MD_DenseTime         created;
	MD_FilePropertyFlags flags;
};
