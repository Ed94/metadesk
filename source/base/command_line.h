#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "strings.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Parsed Command Line Types

typedef struct MD_CmdLineOpt MD_CmdLineOpt;
struct MD_CmdLineOpt
{
	MD_CmdLineOpt* next;
	MD_CmdLineOpt* hash_next;
	MD_U64         hash;
	MD_String8     string;
	MD_String8List value_strings;
	MD_String8     value_string;
};

typedef struct MD_CmdLineOptList MD_CmdLineOptList;
struct MD_CmdLineOptList
{
	MD_U64         count;
	MD_CmdLineOpt* first;
	MD_CmdLineOpt* last;
};

typedef struct MD_CmdLine MD_CmdLine;
struct MD_CmdLine
{
	MD_String8        exe_name;
	MD_CmdLineOptList options;
	MD_String8List    inputs;
	MD_U64            option_table_size;
	MD_CmdLineOpt**   option_table;
};

////////////////////////////////
//~ NOTE(rjf): Command Line Option Parsing

       MD_U64          md_cmd_line_hash_from_string(MD_String8 string);
MD_API MD_CmdLineOpt** md_cmd_line_slot_from_string(MD_CmdLine*        cmd_line, MD_String8 string);
MD_API MD_CmdLineOpt*  md_cmd_line_opt_from_slot   (MD_CmdLineOpt**    slot,     MD_String8 string);
       void            md_cmd_line_push_opt        (MD_CmdLineOptList* list,     MD_CmdLineOpt* var);

MD_CmdLineOpt*  md_cmd_line_opt_from_string (MD_CmdLine* cmd_line, MD_String8 name);
MD_String8List  md_cmd_line_strings         (MD_CmdLine* cmd_line, MD_String8 name);
MD_String8      md_cmd_line_string          (MD_CmdLine* cmd_line, MD_String8 name);
MD_B32          md_cmd_line_has_flag        (MD_CmdLine* cmd_line, MD_String8 name);
MD_B32          md_cmd_line_has_argument    (MD_CmdLine* cmd_line, MD_String8 name);

inline MD_CmdLineOpt*  md_cmd_line_insert_opt__arena      (MD_Arena*        arena, MD_CmdLine* cmd_line, MD_String8 string, MD_String8List values);
MD_API MD_CmdLineOpt*  md_cmd_line_insert_opt__ainfo      (MD_AllocatorInfo ainfo, MD_CmdLine* cmd_line, MD_String8 string, MD_String8List values);
inline MD_CmdLine      md_cmd_line_from_string_list__arena(MD_Arena*        arena, MD_String8List arguments);
MD_API MD_CmdLine      md_cmd_line_from_string_list__ainfo(MD_AllocatorInfo ainfo, MD_String8List arguments);

#define md_cmd_line_insert_opt(allocator, cmd_line, string, values) \
_Generic(allocator,                                                 \
	MD_Arena*:        md_cmd_line_insert_opt__arena,                \
	MD_AllocatorInfo: md_cmd_line_insert_opt__ainfo,                \
	default:          md_assert_generic_sel_fail                    \
) md_generic_call(allocator, cmd_line, string, values)

#define md_cmd_line_from_string_list(allocator, arguments) \
_Generic(allocator,                                        \
	MD_Arena*:        md_cmd_line_from_string_list__arena, \
	MD_AllocatorInfo: md_cmd_line_from_string_list__ainfo, \
	default:       md_assert_generic_sel_fail              \
) md_generic_call(allocator, arguments)

inline MD_U64
md_cmd_line_hash_from_string(MD_String8 string) { 
	MD_U64 result = 5381;
	for(MD_U64 i = 0; i < string.size; i += 1) {
		result = ((result << 5) + result) + string.str[i];
	}
	return result;
}

md_force_inline MD_CmdLineOpt* md_cmd_line_insert_opt__arena      (MD_Arena* arena, MD_CmdLine* cmd_line, MD_String8 string, MD_String8List values) { return md_cmd_line_insert_opt__ainfo      (md_arena_allocator(arena), cmd_line, string, values); }
md_force_inline MD_CmdLine     md_cmd_line_from_string_list__arena(MD_Arena* arena, MD_String8List command_line)                                    { return md_cmd_line_from_string_list__ainfo(md_arena_allocator(arena), command_line); }

inline MD_CmdLineOpt* md_cmd_line_opt_from_string(MD_CmdLine *cmd_line, MD_String8 name) { return md_cmd_line_opt_from_slot(md_cmd_line_slot_from_string(cmd_line, name), name); }
inline MD_B32         md_cmd_line_has_flag       (MD_CmdLine *cmd_line, MD_String8 name) { MD_CmdLineOpt *var = md_cmd_line_opt_from_string(cmd_line, name); return(var != 0); }
inline MD_B32         md_cmd_line_has_argument   (MD_CmdLine *cmd_line, MD_String8 name) { MD_CmdLineOpt *var = md_cmd_line_opt_from_string(cmd_line, name); return(var != 0 && var->value_strings.node_count > 0); }

inline MD_String8List 
md_cmd_line_strings(MD_CmdLine *cmd_line, MD_String8 name) {
	MD_String8List result = {0};
	MD_CmdLineOpt* var    = md_cmd_line_opt_from_string(cmd_line, name);
	if (var != 0) { result = var->value_strings; }
	return result;
}

inline MD_String8     
md_cmd_line_string(MD_CmdLine *cmd_line, MD_String8 name) {
	MD_String8     result = {0};
	MD_CmdLineOpt* var    = md_cmd_line_opt_from_string(cmd_line, name);
	if (var != 0) { result = var->value_string; }
	return result;
}

inline void md_cmd_line_push_opt(MD_CmdLineOptList* list, MD_CmdLineOpt* var) { md_sll_queue_push(list->first, list->last, var); list->count += 1; }
