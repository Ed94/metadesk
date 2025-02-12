#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Zero Settings

#if !defined(MD_PROFILE_TELEMETRY)
#	define MD_PROFILE_TELEMETRY 0
#endif

#if !defined(MD_MARKUP_LAYER_COLOR)
#	define MD_MARKUP_LAYER_COLOR 1.00f, 0.00f, 1.00f
#endif

////////////////////////////////
//~ rjf: Third Party Includes

#if MD_PROFILE_TELEMETRY
#	include "rad_tm.h"
#	if MD_OS_WINDOWS
#		pragma comment(lib, "rad_tm_win64.lib")
#	endif
#endif

////////////////////////////////
//~ rjf: Telemetry Profile Defines

#if MD_PROFILE_TELEMETRY
#	define md_prof_begin(...)           tmEnter(0, 0, __VA_ARGS__)
#	define md_prof_begin_dynamic(...)   (TM_API_PTR ? TM_API_PTR->_tmEnterZoneV_Core(0, 0, __FILE__, &g_telemetry_filename_id, __LINE__, __VA_ARGS__) : (void)0)
#	define md_prof_end(...)             (TM_API_PTR ? TM_API_PTR->_tmLeaveZone(0) : (void)0)
#	define md_prof_tick(...)            tmTick(0)
#	define md_prof_is_capturing(...)    tmRunning()
#	define md_prof_begin_capture(...)   tmOpen(0, __VA_ARGS__, __DATE__, "localhost", TMCT_TCP, TELEMETRY_DEFAULT_PORT, TMOF_INIT_NETWORKING|TMOF_CAPTURE_CONTEXT_SWITCHES, 100)
#	define md_prof_end_capture(...)     tmClose(0)
#	define md_prof_thread_name(...)     (TM_API_PTR ? TM_API_PTR->_tmThreadName(0, 0, __VA_ARGS__) : (void)0)
#	define md_prof_msg(...)             (TM_API_PTR ? TM_API_PTR->_tmMessageV_Core(0, TMMF_ICON_NOTE, __FILE__, &g_telemetry_filename_id, __LINE__, __VA_ARGS__) : (void)0)
#	define md_prof_begin_lock_wait(...) tmStartWaitForLock(0, 0, __VA_ARGS__)
#	define md_prof_end_lock_wait(...)   tmEndWaitForLock(0)
#	define md_prof_lock_take(...)       tmAcquiredLock(0, 0, __VA_ARGS__)
#	define md_prof_lock_drop(...)       tmReleasedLock(0, __VA_ARGS__)
#	define md_prof_color(color)         tmZoneColorSticky(color)
#endif

// TODO(Ed): Support spall?
// TODO(Ed): Support tracey?

////////////////////////////////
//~ rjf: Zeroify Undefined Defines

#if !defined(md_prof_begin)
#	define md_prof_begin(...)         (0)
#	define md_prof_begin_dynamic(...) (0)
#	define md_prof_end(...)           (0)
#	define md_prof_tick(...)          (0)
#	define md_prof_is_capturing(...)  (0)
#	define md_prof_begin_capture(...) (0)
#	define md_prof_end_capture(...)   (0)
#	define md_prof_thread_name(...)   (0)
#	define md_prof_msg(...)           (0)
#	define md_prof_end_lock_wait(...) (0)
#	define md_prof_end_lock_wait(...) (0)
#	define md_prof_lock_take(...)     (0)
#	define md_prof_lock_drop(...)     (0)
#	define md_prof_color(...)         (0)
#endif

////////////////////////////////
//~ rjf: Helper Wrappers

#define md_prof_begin_function(...) md_prof_begin(md_this_function_name)
#define md_prof_scope(...)          md_defer_loop(md_prof_begin_dynamic(__VA_ARGS__), ProfEnd())
