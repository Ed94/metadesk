#ifdef INTELLISENSE_DIRECTIVES
#	include "command_line.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ NOTE(rjf): Command Line Option Parsing

MD_CmdLineOpt**
md_cmd_line_slot_from_string(MD_CmdLine* cmd_line, MD_String8 string) {
	MD_CmdLineOpt** slot = 0;
	if (cmd_line->option_table_size != 0)
	{
		MD_U64 hash   = md_cmd_line_hash_from_string(string);
		MD_U64 bucket = hash % cmd_line->option_table_size;
		slot = &cmd_line->option_table[bucket];
	}
	return slot;
}

MD_CmdLineOpt*
md_cmd_line_opt_from_slot(MD_CmdLineOpt** slot, MD_String8 string) {
	MD_CmdLineOpt* result = 0;
	for (MD_CmdLineOpt* var = *slot; var; var = var->hash_next)
	{
		if (md_str8_match(string, var->string, 0)) {
			result = var;
			break;
		}
	}
	return result;
}

MD_CmdLineOpt*
md_cmd_line_insert_opt__ainfo(MD_AllocatorInfo ainfo, MD_CmdLine* cmd_line, MD_String8 string, MD_String8List values)
{
	MD_CmdLineOpt*  var          = 0;
	MD_CmdLineOpt** slot         = md_cmd_line_slot_from_string(cmd_line, string);
	MD_CmdLineOpt*  existing_var = md_cmd_line_opt_from_slot(slot, string);
	if(existing_var != 0)
	{
		var = existing_var;
	}
	else
	{
		var = md_alloc_array(ainfo, MD_CmdLineOpt, 1);
		var->hash_next    = *slot;
		var->hash         = md_cmd_line_hash_from_string(string);
		var->string       = md_str8_copy(ainfo, string);
		var->value_strings = values;

		MD_StringJoin join = {0};
		join.pre  = md_str8_lit("");
		join.sep  = md_str8_lit(",");
		join.post = md_str8_lit("");

		var->value_string = md_str8_list_join(ainfo, &var->value_strings, &join);
		*slot = var;
		md_cmd_line_push_opt(&cmd_line->options, var);
	}
	return var;
}

MD_CmdLine
md_cmd_line_from_string_list__ainfo(MD_AllocatorInfo ainfo, MD_String8List command_line)
{
	MD_CmdLine parsed = {0};
	parsed.exe_name = command_line.first->string;
	
	// NOTE(rjf): Set up config option table.
	{
		parsed.option_table_size = 4096;
		parsed.option_table      = md_alloc_array(ainfo, MD_CmdLineOpt*, parsed.option_table_size);
	}
	
	// NOTE(rjf): Parse command line.
	MD_B32 after_passthrough_option = 0;
	MD_B32 first_passthrough        = 1;
	for (MD_String8Node* node = command_line.first->next, *next = 0; node != 0; node = next)
	{
		        next        = node->next;
		MD_String8 option_name = node->string;
		
		// NOTE(rjf): Look at -- or - at the start of an argument to determine if it's
		// a flag option. All arguments after a single "--" (with no trailing string
		// on the command line will be considered as input files.
		MD_B32 is_option = 1;
		if (after_passthrough_option == 0)
		{
			if (md_str8_match(node->string, md_str8_lit("--"), 0)) {
				after_passthrough_option = 1;
				is_option = 0;
			}
			else if (md_str8_match(md_str8_prefix(node->string, 2), md_str8_lit("--"), 0)) {
				option_name = md_str8_skip(option_name, 2);
			}
			else if (md_str8_match(md_str8_prefix(node->string, 1), md_str8_lit("-"), 0)) {
				option_name = md_str8_skip(option_name, 1);
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
		if (is_option)
		{
			MD_B32     has_arguments           = 0;
			MD_U64     arg_signifier_position1 = md_str8_find_needle(option_name, 0, md_str8_lit(":"), 0);
			MD_U64     arg_signifier_position2 = md_str8_find_needle(option_name, 0, md_str8_lit("="), 0);
			MD_U64     arg_signifier_position  = md_min(arg_signifier_position1, arg_signifier_position2);
			MD_String8 arg_portion_this_string = md_str8_skip(option_name, arg_signifier_position+1);
			if (arg_signifier_position < option_name.size) {
				has_arguments = 1;
			}
			option_name = md_str8_prefix(option_name, arg_signifier_position);
			
			MD_String8List arguments = {0};
			
			// NOTE(rjf): Parse arguments.
			if (has_arguments)
			{
				for (MD_String8Node* n = node; n; n = n->next)
				{
					next = n->next;
					
					MD_String8 string = n->string;
					if (n == node) {
						string = arg_portion_this_string;
					}
					
					MD_U8          splits[] = { ',' };
					MD_String8List args_in_this_string = md_str8_split(ainfo, string, splits, md_array_count(splits), 0);
					for (MD_String8Node* sub_arg = args_in_this_string.first; sub_arg; sub_arg = sub_arg->next) {
						md_str8_list_push(ainfo, &arguments, sub_arg->string);
					}
					if ( !md_str8_match(md_str8_postfix(n->string, 1), md_str8_lit(","), 0) && (n != node || arg_portion_this_string.size != 0)) {
						break;
					}
				}
			}
			
			// NOTE(rjf): Register config variable.
			md_cmd_line_insert_opt(ainfo, &parsed, option_name, arguments);
		}
		
		// NOTE(rjf): Default path, treat as a passthrough config option to be
		// handled by tool-specific code.
		else if ( !md_str8_match(node->string, md_str8_lit("--"), 0) || !first_passthrough) {
			md_str8_list_push(ainfo, &parsed.inputs, node->string);
			after_passthrough_option = 1;
			first_passthrough        = 0;
		}
	}
	
	return parsed;
}
