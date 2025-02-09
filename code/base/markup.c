#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "markup.h"
#endif

inline void
set_thread_name(String8 string) {
	prof_thread_name("%.*s", str8_varg(string));
	os_set_thread_name(string);
}
