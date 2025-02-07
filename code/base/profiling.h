#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "macros.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Zero Settings

#if !defined(PROFILE_TELEMETRY)
#	define PROFILE_TELEMETRY 0
#endif

#if !defined(MARKUP_LAYER_COLOR)
#	define MARKUP_LAYER_COLOR 1.00f, 0.00f, 1.00f
#endif

////////////////////////////////
//~ rjf: Third Party Includes

#if PROFILE_TELEMETRY
#	include "rad_tm.h"
#	if OS_WINDOWS
#		pragma comment(lib, "rad_tm_win64.lib")
#	endif
#endif

////////////////////////////////
//~ rjf: Telemetry Profile Defines

#if PROFILE_TELEMETRY
#	define prof_begin(...)           tmEnter(0, 0, __VA_ARGS__)
#	define prof_begin_dynamic(...)   (TM_API_PTR ? TM_API_PTR->_tmEnterZoneV_Core(0, 0, __FILE__, &g_telemetry_filename_id, __LINE__, __VA_ARGS__) : (void)0)
#	define prof_end(...)             (TM_API_PTR ? TM_API_PTR->_tmLeaveZone(0) : (void)0)
#	define prof_tick(...)            tmTick(0)
#	define prof_is_capturing(...)    tmRunning()
#	define prof_begin_capture(...)   tmOpen(0, __VA_ARGS__, __DATE__, "localhost", TMCT_TCP, TELEMETRY_DEFAULT_PORT, TMOF_INIT_NETWORKING|TMOF_CAPTURE_CONTEXT_SWITCHES, 100)
#	define prof_end_capture(...)     tmClose(0)
#	define prof_thread_name(...)     (TM_API_PTR ? TM_API_PTR->_tmThreadName(0, 0, __VA_ARGS__) : (void)0)
#	define prof_msg(...)             (TM_API_PTR ? TM_API_PTR->_tmMessageV_Core(0, TMMF_ICON_NOTE, __FILE__, &g_telemetry_filename_id, __LINE__, __VA_ARGS__) : (void)0)
#	define prof_begin_lock_wait(...) tmStartWaitForLock(0, 0, __VA_ARGS__)
#	define prof_end_lock_wait(...)   tmEndWaitForLock(0)
#	define prof_lock_take(...)       tmAcquiredLock(0, 0, __VA_ARGS__)
#	define prof_lock_drop(...)       tmReleasedLock(0, __VA_ARGS__)
#	define prof_color(color)         tmZoneColorSticky(color)
#endif

// TODO(Ed): Support spall?
// TODO(Ed): Support tracey?

////////////////////////////////
//~ rjf: Zeroify Undefined Defines

#if !defined(prof_begin)
#	define prof_begin(...)         (0)
#	define prof_begin_dynamic(...) (0)
#	define prof_end(...)           (0)
#	define prof_tick(...)          (0)
#	define prof_is_capturing(...)  (0)
#	define prof_begin_capture(...) (0)
#	define prof_end_capture(...)   (0)
#	define prof_thread_name(...)   (0)
#	define prof_msg(...)           (0)
#	define prof_end_lock_wait(...) (0)
#	define prof_end_lock_wait(...) (0)
#	define prof_lock_take(...)     (0)
#	define prof_lock_drop(...)     (0)
#	define prof_color(...)         (0)
#endif

////////////////////////////////
//~ rjf: Helper Wrappers

#define prof_begin_function(...) prof_begin(this_function_name)
#define prof_scope(...)          defer_loop(prof_begin_dynamic(__VA_ARGS__), ProfEnd())
