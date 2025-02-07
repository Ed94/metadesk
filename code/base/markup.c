#ifdef INTELLISENSE_DIRECTIVES
#	include "markup.h"
#	include "os/os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

void
set_thread_name(String8 string) {
	ProfThreadName("%.*s", str8_varg(string));
	os_set_thread_name(string);
}
