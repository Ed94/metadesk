#ifdef INTELLISENSE_DIRECTIVES
#	include "platform.h"
#endif

////////////////////////////////
//~ rjf: Sorts

#ifndef md_quick_sort
#define md_quick_sort(ptr, count, element_size, cmp_function) qsort((ptr), (count), (element_size), (int (*)(const void *, const void *))(cmp_function))
#endif
