#ifdef INTELLISENSE_DIRECTIVES
#	include "command_line.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ NOTE(rjf): Command Line Option Parsing


CmdLineOpt**
cmd_line_slot_from_string(CmdLine* cmd_line, String8 string) {
	CmdLineOpt** slot = 0;
	if (cmd_line->option_table_size != 0)
	{
		U64 hash   = cmd_line_hash_from_string(string);
		U64 bucket = hash % cmd_line->option_table_size;
		slot = &cmd_line->option_table[bucket];
	}
	return slot;
}

CmdLineOpt*
cmd_line_opt_from_slot(CmdLineOpt** slot, String8 string) {
	CmdLineOpt* result = 0;
	for (CmdLineOpt* var = *slot; var; var = var->hash_next)
	{
		if (str8_match(string, var->string, 0)) {
			result = var;
			break;
		}
	}
	return result;
}

CmdLineOpt*
cmd_line_insert_opt(Arena* arena, CmdLine* cmd_line, String8 string, String8List values)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	CmdLineOpt *var = 0;
	CmdLineOpt **slot = cmd_line_slot_from_string(cmd_line, string);
	CmdLineOpt *existing_var = cmd_line_opt_from_slot(slot, string);
	if(existing_var != 0)
	{
		var = existing_var;
	}
	else
	{
		var = push_array(arena, CmdLineOpt, 1);
		var->hash_next    = *slot;
		var->hash         = cmd_line_hash_from_string(string);
		var->string       = push_str8_copy(arena, string);
		var->value_strings = values;

		StringJoin join = {0};
		join.pre = str8_lit("");
		join.sep = str8_lit(",");
		join.post = str8_lit("");
		var->value_string = str8_list_join(arena, &var->value_strings, &join);
		*slot = var;
		cmd_line_push_opt(&cmd_line->options, var);
	}
	return var;
#else
	return cmd_line_insert_opt_alloc(arena_allocator(arena), cmd_line, string, values);
#endif
}

CmdLineOpt*
cmd_line_insert_opt_alloc(AllocatorInfo ainfo, CmdLine* cmd_line, String8 string, String8List values)
{
	CmdLineOpt *var = 0;
	CmdLineOpt **slot = cmd_line_slot_from_string(cmd_line, string);
	CmdLineOpt *existing_var = cmd_line_opt_from_slot(slot, string);
	if(existing_var != 0)
	{
		var = existing_var;
	}
	else
	{
		var = alloc_array(ainfo, CmdLineOpt, 1);
		var->hash_next    = *slot;
		var->hash         = cmd_line_hash_from_string(string);
		var->string       = alloc_str8_copy(ainfo, string);
		var->value_strings = values;

		StringJoin join = {0};
		join.pre  = str8_lit("");
		join.sep  = str8_lit(",");
		join.post = str8_lit("");

		var->value_string = str8_list_join_alloc(ainfo, &var->value_strings, &join);
		*slot = var;
		cmd_line_push_opt(&cmd_line->options, var);
	}
	return var;
}

CmdLine
cmd_line_from_string_list(Arena* arena, String8List command_line)
{
#if MD_DONT_MAP_ARENA_TO_ALLOCATOR_IMPL
	CmdLine parsed = {0};
	parsed.exe_name = command_line.first->string;
	
	// NOTE(rjf): Set up config option table.
	{
		parsed.option_table_size = 4096;
		parsed.option_table      = push_array(arena, CmdLineOpt*, parsed.option_table_size);
	}
	
	// NOTE(rjf): Parse command line.
	B32 after_passthrough_option = 0;
	B32 first_passthrough        = 1;
	for (String8Node* node = command_line.first->next, *next = 0; node != 0; node = next)
	{
		        next        = node->next;
		String8 option_name = node->string;
		
		// NOTE(rjf): Look at -- or - at the start of an argument to determine if it's
		// a flag option. All arguments after a single "--" (with no trailing string
		// on the command line will be considered as input files.
		B32 is_option = 1;
		if(after_passthrough_option == 0)
		{
			if (str8_match(node->string, str8_lit("--"), 0)) {
				after_passthrough_option = 1;
				is_option = 0;
			}
			else if (str8_match(str8_prefix(node->string, 2), str8_lit("--"), 0)) {
				option_name = str8_skip(option_name, 2);
			}
			else if (str8_match(str8_prefix(node->string, 1), str8_lit("-"), 0)) {
				option_name = str8_skip(option_name, 1);
			}
			else {
				is_option = 0;
			}
		}
		else
		{
			is_option = 0;
		}
		
		// NOTE(rjf): This string is an option.
		if(is_option)
		{
			B32     has_arguments           = 0;
			U64     arg_signifier_position1 = str8_find_needle(option_name, 0, str8_lit(":"), 0);
			U64     arg_signifier_position2 = str8_find_needle(option_name, 0, str8_lit("="), 0);
			U64     arg_signifier_position  = min(arg_signifier_position1, arg_signifier_position2);
			String8 arg_portion_this_string = str8_skip(option_name, arg_signifier_position+1);
			if (arg_signifier_position < option_name.size) {
				has_arguments = 1;
			}
			option_name = str8_prefix(option_name, arg_signifier_position);
			
			String8List arguments = {0};
			
			// NOTE(rjf): Parse arguments.
			if (has_arguments)
			{
				for (String8Node* n = node; n; n = n->next)
				{
					next = n->next;
					
					String8 string = n->string;
					if (n == node) {
						string = arg_portion_this_string;
					}
					
					U8          splits[] = { ',' };
					String8List args_in_this_string = str8_split(arena, string, splits, ArrayCount(splits), 0);
					for (String8Node* sub_arg = args_in_this_string.first; sub_arg; sub_arg = sub_arg->next) {
						str8_list_push(arena, &arguments, sub_arg->string);
					}
					if ( !str8_match(str8_postfix(n->string, 1), str8_lit(","), 0) && (n != node || arg_portion_this_string.size != 0)) {
						break;
					}
				}
			}
			
			// NOTE(rjf): Register config variable.
			cmd_line_insert_opt(arena, &parsed, option_name, arguments);
		}
		
		// NOTE(rjf): Default path, treat as a passthrough config option to be
		// handled by tool-specific code.
		else if ( !str8_match(node->string, str8_lit("--"), 0) || !first_passthrough) {
			str8_list_push(arena, &parsed.inputs, node->string);
			after_passthrough_option = 1;
			first_passthrough        = 0;
		}
	}
	
	return parsed;
#else
	return cmd_line_from_string_list_alloc(arena_allocator(arena), command_line);
#endif
}

CmdLine
cmd_line_from_string_list_alloc(AllocatorInfo ainfo, String8List command_line)
{
	CmdLine parsed = {0};
	parsed.exe_name = command_line.first->string;
	
	// NOTE(rjf): Set up config option table.
	{
		parsed.option_table_size = 4096;
		parsed.option_table      = alloc_array(ainfo, CmdLineOpt*, parsed.option_table_size);
	}
	
	// NOTE(rjf): Parse command line.
	B32 after_passthrough_option = 0;
	B32 first_passthrough        = 1;
	for (String8Node* node = command_line.first->next, *next = 0; node != 0; node = next)
	{
		        next        = node->next;
		String8 option_name = node->string;
		
		// NOTE(rjf): Look at -- or - at the start of an argument to determine if it's
		// a flag option. All arguments after a single "--" (with no trailing string
		// on the command line will be considered as input files.
		B32 is_option = 1;
		if(after_passthrough_option == 0)
		{
			if (str8_match(node->string, str8_lit("--"), 0)) {
				after_passthrough_option = 1;
				is_option = 0;
			}
			else if (str8_match(str8_prefix(node->string, 2), str8_lit("--"), 0)) {
				option_name = str8_skip(option_name, 2);
			}
			else if (str8_match(str8_prefix(node->string, 1), str8_lit("-"), 0)) {
				option_name = str8_skip(option_name, 1);
			}
			else {
				is_option = 0;
			}
		}
		else
		{
			is_option = 0;
		}
		
		// NOTE(rjf): This string is an option.
		if(is_option)
		{
			B32     has_arguments           = 0;
			U64     arg_signifier_position1 = str8_find_needle(option_name, 0, str8_lit(":"), 0);
			U64     arg_signifier_position2 = str8_find_needle(option_name, 0, str8_lit("="), 0);
			U64     arg_signifier_position  = min(arg_signifier_position1, arg_signifier_position2);
			String8 arg_portion_this_string = str8_skip(option_name, arg_signifier_position+1);
			if (arg_signifier_position < option_name.size) {
				has_arguments = 1;
			}
			option_name = str8_prefix(option_name, arg_signifier_position);
			
			String8List arguments = {0};
			
			// NOTE(rjf): Parse arguments.
			if (has_arguments)
			{
				for (String8Node* n = node; n; n = n->next)
				{
					next = n->next;
					
					String8 string = n->string;
					if (n == node) {
						string = arg_portion_this_string;
					}
					
					U8          splits[] = { ',' };
					String8List args_in_this_string = str8_split_alloc(ainfo, string, splits, ArrayCount(splits), 0);
					for (String8Node* sub_arg = args_in_this_string.first; sub_arg; sub_arg = sub_arg->next) {
						str8_list_alloc(ainfo, &arguments, sub_arg->string);
					}
					if ( !str8_match(str8_postfix(n->string, 1), str8_lit(","), 0) && (n != node || arg_portion_this_string.size != 0)) {
						break;
					}
				}
			}
			
			// NOTE(rjf): Register config variable.
			cmd_line_insert_opt_alloc(ainfo, &parsed, option_name, arguments);
		}
		
		// NOTE(rjf): Default path, treat as a passthrough config option to be
		// handled by tool-specific code.
		else if ( !str8_match(node->string, str8_lit("--"), 0) || !first_passthrough) {
			str8_list_alloc(ainfo, &parsed.inputs, node->string);
			after_passthrough_option = 1;
			first_passthrough        = 0;
		}
	}
	
	return parsed;
}
