#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "markup.h"
#endif

inline void
md_set_thread_name(MD_String8 string) {
	md_prof_thread_name("%.*s", md_str8_varg(string));
	md_os_set_thread_name(string);
}
