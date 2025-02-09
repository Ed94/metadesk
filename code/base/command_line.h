#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "strings.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Parsed Command Line Types

typedef struct CmdLineOpt CmdLineOpt;
struct CmdLineOpt
{
	CmdLineOpt* next;
	CmdLineOpt* hash_next;
	U64         hash;
	String8     string;
	String8List value_strings;
	String8     value_string;
};

typedef struct CmdLineOptList CmdLineOptList;
struct CmdLineOptList
{
	U64         count;
	CmdLineOpt* first;
	CmdLineOpt* last;
};

typedef struct CmdLine CmdLine;
struct CmdLine
{
	String8        exe_name;
	CmdLineOptList options;
	String8List    inputs;
	U64            option_table_size;
	CmdLineOpt**   option_table;
};

////////////////////////////////
//~ NOTE(rjf): Command Line Option Parsing

       U64          cmd_line_hash_from_string(String8 string);
MD_API CmdLineOpt** cmd_line_slot_from_string(CmdLine*        cmd_line, String8 string);
MD_API CmdLineOpt*  cmd_line_opt_from_slot   (CmdLineOpt**    slot,     String8 string);
       void         cmd_line_push_opt        (CmdLineOptList* list,     CmdLineOpt* var);

CmdLineOpt*  cmd_line_opt_from_string (CmdLine* cmd_line, String8 name);
String8List  cmd_line_strings         (CmdLine* cmd_line, String8 name);
String8      cmd_line_string          (CmdLine* cmd_line, String8 name);
B32          cmd_line_has_flag        (CmdLine* cmd_line, String8 name);
B32          cmd_line_has_argument    (CmdLine* cmd_line, String8 name);

inline CmdLineOpt*  cmd_line_insert_opt_push       (Arena*        arena, CmdLine* cmd_line, String8 string, String8List values);
MD_API CmdLineOpt*  cmd_line_insert_opt_alloc      (AllocatorInfo ainfo, CmdLine* cmd_line, String8 string, String8List values);
inline CmdLine      cmd_line_from_string_list_push (Arena*        arena, String8List arguments);
MD_API CmdLine      cmd_line_from_string_list_alloc(AllocatorInfo ainfo, String8List arguments);

#define cmd_line_insert_opt(allocator, cmd_line, string, values) \
_Generic(allocator,                                              \
	Arena*:        cmd_line_insert_opt_push,                     \
	AllocatorInfo: cmd_line_insert_opt_alloc,                    \
	default:       METADESK_NO_RESOLVED_GENERIC_SELECTION        \
) MD_RESOLVED_FUNCTION_CALL(allocator, cmd_line, string, values)

#define cmd_line_from_string_list(allocator, arguments)   \
_Generic(allocator,                                       \
	Arena*:        cmd_line_from_string_list_push,        \
	AllocatorInfo: cmd_line_from_string_list_alloc,       \
	default:       METADESK_NO_RESOLVED_GENERIC_SELECTION \
) MD_RESOLVED_FUNCTION_CALL(allocator, arguments)

inline U64
cmd_line_hash_from_string(String8 string) { 
	U64 result = 5381;
	for(U64 i = 0; i < string.size; i += 1) {
		result = ((result << 5) + result) + string.str[i];
	}
	return result;
}

force_inline CmdLineOpt* cmd_line_insert_opt_push      (Arena* arena, CmdLine* cmd_line, String8 string, String8List values) { return cmd_line_insert_opt_alloc      (arena_allocator(arena), cmd_line, string, values); }
force_inline CmdLine     cmd_line_from_string_list_push(Arena* arena, String8List command_line)                              { return cmd_line_from_string_list_alloc(arena_allocator(arena), command_line); }

inline CmdLineOpt* cmd_line_opt_from_string(CmdLine *cmd_line, String8 name) { return cmd_line_opt_from_slot(cmd_line_slot_from_string(cmd_line, name), name); }
inline B32         cmd_line_has_flag       (CmdLine *cmd_line, String8 name) { CmdLineOpt *var = cmd_line_opt_from_string(cmd_line, name); return(var != 0); }
inline B32         cmd_line_has_argument   (CmdLine *cmd_line, String8 name) { CmdLineOpt *var = cmd_line_opt_from_string(cmd_line, name); return(var != 0 && var->value_strings.node_count > 0); }

inline String8List 
cmd_line_strings(CmdLine *cmd_line, String8 name) {
	String8List result = {0};
	CmdLineOpt* var    = cmd_line_opt_from_string(cmd_line, name);
	if (var != 0) { result = var->value_strings; }
	return result;
}

inline String8     
cmd_line_string(CmdLine *cmd_line, String8 name) {
	String8     result = {0};
	CmdLineOpt* var    = cmd_line_opt_from_string(cmd_line, name);
	if (var != 0) { result = var->value_string; }
	return result;
}

inline void cmd_line_push_opt(CmdLineOptList* list, CmdLineOpt* var) { sll_queue_push(list->first, list->last, var); list->count += 1; }
