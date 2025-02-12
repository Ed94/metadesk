#if INTELLISENSE_DIRECTIVES
#	pragma once
#endif

#define GEN_DONT_ENFORCE_GEN_TIME_GUARD
#define GEN_DEFINE_LIBRARY_CODE_CONSTANTS
#define GEN_ENFORCE_STRONG_CODE_TYPES
#define GEN_IMPLEMENTATION
#include "third_party/gencpp_c11/gencpp_c11.h"
#include "third_party/gencpp_c11/misc.h"

#include "third_party/gencpp_c11/push_ignores.inline.h"

#define path_root            "./"
#define path_bin             path_root    "bin/"
#define path_build           path_root    "build/"
#define path_code            path_root    "code/"
#define path_gen_c11         path_root    "gen_c11/"
#define path_gen             path_gen_c11 "gen/"
#define path_base            path_code    "base/"
#define path_mdesk           path_code    "mdesk/"
#define path_os              path_code    "os/"
#define path_os_win32        path_os      "win32/"
#define path_os_linux        path_os      "linux/"
#define path_third_party     path_root    "third_party/"
#define path_scratch_file    path_gen     "scratch.h"
#define path_format_style    path_bin     ".clang-format "

#define gen_iterator( Type, container, iter )   \
	        gen_begin_ ## Type(container);      \
	iter != gen_end_   ## Type(container);      \
	iter  = gen_next_  ## Type(container, iter)

// Codegen DSL

#define lit     gen_txt
#define nullptr gen_nullptr
// #define args    

#define str_fmt(fmt, ...)           gen_strbuilder_to_str(gen_strbuilder_fmt_buf(gen_get_context()->Allocator_Temp, fmt, __VA_ARGS__))
#define print(code)                 gen_builder_print(builder, code);
#define print_fmt(fmt, ...)         gen_builder_print_fmt(builder, fmt, __VA_ARGS__)
#define new_line()                  gen_builder_print(builder, gen_fmt_newline)
#define pragma_region(label)        print(gen_def_pragma(str_fmt("region %S",    lit(label))))
#define pragma_endregion(label)     print(gen_def_pragma(str_fmt("endregion %S", lit(label))))
#define define(id, type, ...)       print(gen_def__define(id, type, &(gen_Opts_def_define){ __VA_ARGS__ }))
#define comment(content)            print(gen_def_comment(lit(content)))
#define include(content, ...)       print(gen_def__include(content, &(gen_Opts_def_include){ __VA_ARGS__}))
#define preprocess_cond(type, expr) gen_def_preprocess_cond(type, expr)
#define preprocess_if(expr)         print(preprocess_cond(PreprocessCond_If,       lit(expr))); new_line()
#define preprocess_ifndef(expr)     print(preprocess_cond(PreprocessCond_IfNotDef, lit(expr))); new_line()
#define preprocess_elif(expr)       print(preprocess_cond(PreprocessCond_ElIf,     lit(expr))); new_line()
#define preprocess_endif()		    print(gen_preprocess_endif)

#ifndef PRINT_SECTION_REGION_PRAGMAS
#define PRINT_SECTION_REGION_PRAGMAS 1
#endif

#define print_section(code, label) print__section(builder, code, label)
void print__section(gen_Builder* builder, gen_Code code, gen_Str label) {
	if (PRINT_SECTION_REGION_PRAGMAS) gen_builder_print(builder, gen_def_pragma(str_fmt("region %S", label)) );
	gen_builder_print(builder, code);
	if (PRINT_SECTION_REGION_PRAGMAS) gen_builder_print(builder, gen_def_pragma(str_fmt("endregion %S", label)) );
	gen_builder_print(builder, gen_fmt_newline);
}

void register_library_macros() 
{
	// third_party/stb/stb_sprintf.h
	gen_register_macros( args(
		((gen_Macro) { lit("STB_SPRINTF_H_INCLUDE"),   MT_Statement,  }),
		((gen_Macro) { lit("STBSP__ASAN"),             MT_Statement,                  MF_Allow_As_Attribute }),
		((gen_Macro) { lit("STBSP__PUBLICDEC"),        MT_Expression,                 MF_Allow_As_Attribute }),
		((gen_Macro) { lit("STBSP__ATTRIBUTE_FORMAT"), MT_Expression, MF_Functional | MF_Allow_As_Attribute }),
		((gen_Macro) { lit("STBSP__NOTUSED"),          MT_Expression, MF_Functional | MF_Allow_As_Attribute }),
		((gen_Macro) { lit("STBSP__NOTUSED"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("STB_SPRINTF_MIN"),         MT_Expression,                 MF_Allow_As_Identifier }),
		((gen_Macro) { lit("STB_SPRINTF_DECORATE"),    MT_Expression, MF_Functional | MF_Allow_As_Identifier | MF_Identifier_Decorator }),
		((gen_Macro) { lit("STB_SPRINTF_MSVC_MODE"),   MT_Expression,  }),
		((gen_Macro) { lit("STBSP__PUBLICDEF"),        MT_Expression,                 MF_Allow_As_Attribute }),
		((gen_Macro) { lit("STBSP__UNALIGNED"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("STBSP__SPECIAL"),          MT_Expression,  }),
		((gen_Macro) { lit("STBSP__LEFTJUST"),         MT_Expression,  }),
		((gen_Macro) { lit("STBSP__LEADINGPLUS"),      MT_Expression,  }),
		((gen_Macro) { lit("STBSP__LEADINGSPACE"),     MT_Expression,  }),
		((gen_Macro) { lit("STBSP__LEADING_0X"),       MT_Expression,  }),
		((gen_Macro) { lit("STBSP__LEADINGZERO"),      MT_Expression,  }),
		((gen_Macro) { lit("STBSP__INTMAX"),           MT_Expression,  }),
		((gen_Macro) { lit("STBSP__TRIPLET_COMMA"),    MT_Expression,  }),
		((gen_Macro) { lit("STBSP__NEGATIVE"),         MT_Expression,  }),
		((gen_Macro) { lit("STBSP__METRIC_SUFFIX"),    MT_Expression,  }),
		((gen_Macro) { lit("STBSP__HALFWIDTH"),        MT_Expression,  }),
		((gen_Macro) { lit("STBSP__METRIC_NOSPACE"),   MT_Expression,  }),
		((gen_Macro) { lit("STBSP__METRIC_1024"),      MT_Expression,  }),
		((gen_Macro) { lit("STBSP__METRIC_JEDEC"),     MT_Expression,  }),
		((gen_Macro) { lit("stbsp__chk_cb_bufL"),      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__chk_cb_buf"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__flush_cb"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__cb_buf_clamp"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("STBSP__NUMSZ"),            MT_Expression, }),
		((gen_Macro) { lit("STBSP__COPYFP"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__tento19th"),        MT_Expression,  }),
		((gen_Macro) { lit("stbsp__ddmulthi"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__ddrenorm"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__ddmultlo"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__ddmultlos"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("stbsp__ddtoS64"),          MT_Expression, MF_Functional })
	));

	// These in the same order as metadesk.h

	// base/context_cracking.h
	gen_register_macros( args(
		((gen_Macro) { lit("BUILD_DEBUG"),                          MT_Expression,  }),
		((gen_Macro) { lit("BUILD_STATIC"),                         MT_Expression,  }),
		((gen_Macro) { lit("BUILD_DYNAMIC"),                        MT_Expression,  }),
		((gen_Macro) { lit("BUILD_API_EXPORT"),                     MT_Expression,  }),
		((gen_Macro) { lit("BUILD_ENTRY_DEFINING_UNIT"),            MT_Expression,  }),
		((gen_Macro) { lit("BUILD_CONSOLE_INTERFACE"),              MT_Expression,  }),
		((gen_Macro) { lit("BUILD_VERSION_MAJOR"),                  MT_Expression,  }),
		((gen_Macro) { lit("BUILD_VERSION_MINOR"),                  MT_Expression,  }),
		((gen_Macro) { lit("BUILD_VERSION_PATCH"),                  MT_Expression,  }),
		((gen_Macro) { lit("BUILD_VERSION_STRING_LITERAL"),         MT_Expression,  }),
		((gen_Macro) { lit("BUILD_MODE_STRING_LITERAL_APPEND"),     MT_Expression,  }),
		((gen_Macro) { lit("BUILD_GIT_HASH_STRING_LITERAL_APPEND"), MT_Expression,  }),
		((gen_Macro) { lit("BUILD_TITLE"),                          MT_Expression,  }),
		((gen_Macro) { lit("BUILD_RELEASE_PHASE_STRING_LITERAL"),   MT_Expression,  }),
		((gen_Macro) { lit("BUILD_ISSUES_LINK_STRING_LITERAL"),     MT_Expression,  }),
		((gen_Macro) { lit("BUILD_TITLE_STRING_LITERAL"),           MT_Expression,  }),
		((gen_Macro) { lit("COMPILER_MSVC"),                        MT_Expression,  }),
		((gen_Macro) { lit("COMPILER_CLANG"),                       MT_Expression,  }),
		((gen_Macro) { lit("COMPILER_GCC"),                         MT_Expression,  }),
		((gen_Macro) { lit("LANG_C"),                               MT_Expression,  }),
		((gen_Macro) { lit("LANG_CPP"),                             MT_Expression,  }),
		((gen_Macro) { lit("ARCH_X64"),                             MT_Expression,  }),
		((gen_Macro) { lit("ARCH_X86"),                             MT_Expression,  }),
		((gen_Macro) { lit("ARCH_ARM64"),                           MT_Expression,  }),
		((gen_Macro) { lit("ARCH_ARM32"),                           MT_Expression,  }),
		((gen_Macro) { lit("ARCH_LITTLE_ENDIAN"),                   MT_Expression,  }),
		((gen_Macro) { lit("OS_WINDOWS"),                           MT_Expression,  }),
		((gen_Macro) { lit("OS_OSX"),                               MT_Expression,  }),
		((gen_Macro) { lit("OS_ANDROID"),                           MT_Expression,  }),
		((gen_Macro) { lit("OS_LINUX"),                             MT_Expression,  }),
		((gen_Macro) { lit("OS_FREEBSD"),                           MT_Expression,  }),
		((gen_Macro) { lit("OS_OPENBSD"),                           MT_Expression,  }),
		((gen_Macro) { lit("OS_EMSCRIPTEN"),                        MT_Expression,  }),
		((gen_Macro) { lit("OS_CYGWIN"),                            MT_Expression,  }),
		((gen_Macro) { lit("MD_API"),                               MT_Expression,  MF_Allow_As_Attribute }),
		((gen_Macro) { lit("MD_API_C"),                             MT_Expression,  MF_Allow_As_Attribute }),
		((gen_Macro) { lit("MD_API_C_BEGIN"),                       MT_Statement,                         }),
		((gen_Macro) { lit("MD_API_C_END"),                         MT_Statement,                         })
	));
	// base/macros.h
	gen_register_macros(args(
		((gen_Macro) { lit("expect"),                                   MT_Expression, MF_Functional | MF_Allow_As_Attribute }),
		((gen_Macro) { lit("likely"),                                   MT_Expression, MF_Functional | MF_Allow_As_Attribute }),
		((gen_Macro) { lit("unlikely"),                                 MT_Expression, MF_Functional | MF_Allow_As_Attribute }),
		((gen_Macro) { lit("ccast"),                                    MT_Expression, MF_Functional                         }),
		((gen_Macro) { lit("rcast"),                                    MT_Expression, MF_Functional                         }),
		((gen_Macro) { lit("pcast"),                                    MT_Expression, MF_Functional                         }),
		((gen_Macro) { lit("scast"),                                    MT_Expression, MF_Functional                         }),
		((gen_Macro) { lit("typeof"),                                   MT_Expression,  }),
		((gen_Macro) { lit("nullptr"),                                  MT_Expression,  }),
		((gen_Macro) { lit("defer_loop"),                               MT_Statement,  MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("defer_loop_checked"),                       MT_Statement,  MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("each_enum_val"),                            MT_Statement,  MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("each_non_zero_enum_val"),                   MT_Statement,  MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("stringify"),                                MT_Expression, MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("glue"),                                     MT_Expression, MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("src_line_str"),                             MT_Expression,  }),
		((gen_Macro) { lit("do_once"),                                  MT_Statement,  MF_Functional | MF_Expects_Body }),
		((gen_Macro) { lit("do_once_start"),                            MT_Statement,  }),
		((gen_Macro) { lit("do_once_end"),                              MT_Statement,  }),
		((gen_Macro) { lit("do_once_end"),                              MT_Statement,  })
	));
	// base/generic_macros.h
	gen_register_macros(args(
		((gen_Macro) { lit("COMMA_OPERATOR"),                           MT_Expression, }),
		((gen_Macro) { lit("select_arg_1"),                             MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("select_arg_2"),                             MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("select_arg_3"),                             MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("generic_sel_entry_type"),                   MT_Expression,                }),
		((gen_Macro) { lit("generic_sel_entry_function"),               MT_Expression,                }),
		((gen_Macro) { lit("generic_sel_entry_comma_delimiter"),        MT_Expression,                }),
		((gen_Macro) { lit("generic_call"),                   MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("if_generic_selector_defined_include_slot"), MT_Expression,                }),
		((gen_Macro) { lit("function_generic_example"),                 MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("GENERIC_SLOT_1__example_hash"),             MT_Expression,                }),
		((gen_Macro) { lit("GENERIC_SLOT_2__example_hash"),             MT_Expression,                }),
		((gen_Macro) { lit("generic_example_hash"),                     MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("function_generic_example_varadic"),         MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("function_generic_example_direct_type"),     MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("type_to_expression"),                       MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("_Generic_L2"),                              MT_Expression, MF_Functional  }),
		((gen_Macro) { lit("_Generic_L3"),                              MT_Expression, MF_Functional  })
	));
	// base/profiling.h
	gen_register_macros(args(
		((gen_Macro) { lit("PROFILE_TELEMETRY"),    MT_Expression,               }),
		((gen_Macro) { lit("MARKUP_LAYER_COLOR"),   MT_Expression,               }),
		((gen_Macro) { lit("prof_begin"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_begin_dynamic"),   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_end"),             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_tick"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_is_capturing"),    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_begin_capture"),   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_end_capture"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_thread_name"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_msg"),             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_begin_lock_wait"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_end_lock_wait"),   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_lock_take"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_lock_drop"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_color"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_msg"),             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_begin_function"),  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_scope"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("prof_scope"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("size_of"),              MT_Expression, MF_Functional })
	));
	// base/base_types.h
	gen_register_macros(args(
		((gen_Macro) { lit("SIGN32"),            MT_Expression,  }),
		((gen_Macro) { lit("EXPONENT32"),        MT_Expression,  }),
		((gen_Macro) { lit("MANTISSA32"),        MT_Expression,  }),
		((gen_Macro) { lit("BIG_GOLDEN32"),      MT_Expression,  }),
		((gen_Macro) { lit("SMALL_GOLDEN32"),    MT_Expression,  }),
		((gen_Macro) { lit("PI32"),              MT_Expression,  }),
		((gen_Macro) { lit("MACHINE_EPSILON64"), MT_Expression,  }),
		((gen_Macro) { lit("MIN_U8"),            MT_Expression,  }),
		((gen_Macro) { lit("MAX_U8"),            MT_Expression,  }),
		((gen_Macro) { lit("MIN_S8"),            MT_Expression,  }),
		((gen_Macro) { lit("MAX_S8"),            MT_Expression,  }),
		((gen_Macro) { lit("MIN_U16"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_U16"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_S16"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_S16"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_U32"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_U32"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_S32"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_U64"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_U64"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_S64"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_S64"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_USIZE"),         MT_Expression,  }),
		((gen_Macro) { lit("MAX_USIZE"),         MT_Expression,  }),
		((gen_Macro) { lit("MIN_SSIZE"),         MT_Expression,  }),
		((gen_Macro) { lit("MAX_SSIZE"),         MT_Expression,  }),
		((gen_Macro) { lit("MIN_F32"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_F32"),           MT_Expression,  }),
		((gen_Macro) { lit("MIN_F64"),           MT_Expression,  }),
		((gen_Macro) { lit("MAX_F64"),           MT_Expression,  }),
		((gen_Macro) { lit("BITMASK1"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK2"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK3"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK4"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK5"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK6"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK7"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK8"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK9"),          MT_Expression,  }),
		((gen_Macro) { lit("BITMASK10"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK11"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK12"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK13"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK14"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK15"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK16"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK17"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK18"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK19"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK20"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK21"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK22"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK23"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK24"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK25"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK26"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK27"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK28"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK29"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK30"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK31"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK32"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK33"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK34"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK35"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK36"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK37"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK38"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK39"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK40"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK41"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK42"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK43"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK44"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK45"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK46"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK47"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK48"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK49"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK50"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK51"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK52"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK53"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK54"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK55"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK56"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK57"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK58"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK59"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK60"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK61"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK62"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK63"),         MT_Expression,  }),
		((gen_Macro) { lit("BITMASK64"),         MT_Expression,  })
	));
	gen_register_macros(args(
		((gen_Macro) { lit("BIT1"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT2"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT3"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT4"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT5"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT6"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT7"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT8"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT9"),  MT_Expression,  }),
		((gen_Macro) { lit("BIT10"), MT_Expression,  }),
		((gen_Macro) { lit("BIT11"), MT_Expression,  }),
		((gen_Macro) { lit("BIT12"), MT_Expression,  }),
		((gen_Macro) { lit("BIT13"), MT_Expression,  }),
		((gen_Macro) { lit("BIT14"), MT_Expression,  }),
		((gen_Macro) { lit("BIT15"), MT_Expression,  }),
		((gen_Macro) { lit("BIT16"), MT_Expression,  }),
		((gen_Macro) { lit("BIT17"), MT_Expression,  }),
		((gen_Macro) { lit("BIT18"), MT_Expression,  }),
		((gen_Macro) { lit("BIT19"), MT_Expression,  }),
		((gen_Macro) { lit("BIT20"), MT_Expression,  }),
		((gen_Macro) { lit("BIT21"), MT_Expression,  }),
		((gen_Macro) { lit("BIT22"), MT_Expression,  }),
		((gen_Macro) { lit("BIT23"), MT_Expression,  }),
		((gen_Macro) { lit("BIT24"), MT_Expression,  }),
		((gen_Macro) { lit("BIT25"), MT_Expression,  }),
		((gen_Macro) { lit("BIT26"), MT_Expression,  }),
		((gen_Macro) { lit("BIT27"), MT_Expression,  }),
		((gen_Macro) { lit("BIT28"), MT_Expression,  }),
		((gen_Macro) { lit("BIT29"), MT_Expression,  }),
		((gen_Macro) { lit("BIT30"), MT_Expression,  }),
		((gen_Macro) { lit("BIT31"), MT_Expression,  }),
		((gen_Macro) { lit("BIT32"), MT_Expression,  }),
		((gen_Macro) { lit("BIT33"), MT_Expression,  }),
		((gen_Macro) { lit("BIT34"), MT_Expression,  }),
		((gen_Macro) { lit("BIT35"), MT_Expression,  }),
		((gen_Macro) { lit("BIT36"), MT_Expression,  }),
		((gen_Macro) { lit("BIT37"), MT_Expression,  }),
		((gen_Macro) { lit("BIT38"), MT_Expression,  }),
		((gen_Macro) { lit("BIT39"), MT_Expression,  }),
		((gen_Macro) { lit("BIT40"), MT_Expression,  }),
		((gen_Macro) { lit("BIT41"), MT_Expression,  }),
		((gen_Macro) { lit("BIT42"), MT_Expression,  }),
		((gen_Macro) { lit("BIT43"), MT_Expression,  }),
		((gen_Macro) { lit("BIT44"), MT_Expression,  }),
		((gen_Macro) { lit("BIT45"), MT_Expression,  }),
		((gen_Macro) { lit("BIT46"), MT_Expression,  }),
		((gen_Macro) { lit("BIT47"), MT_Expression,  }),
		((gen_Macro) { lit("BIT48"), MT_Expression,  }),
		((gen_Macro) { lit("BIT49"), MT_Expression,  }),
		((gen_Macro) { lit("BIT50"), MT_Expression,  }),
		((gen_Macro) { lit("BIT51"), MT_Expression,  }),
		((gen_Macro) { lit("BIT52"), MT_Expression,  }),
		((gen_Macro) { lit("BIT53"), MT_Expression,  }),
		((gen_Macro) { lit("BIT54"), MT_Expression,  }),
		((gen_Macro) { lit("BIT55"), MT_Expression,  }),
		((gen_Macro) { lit("BIT56"), MT_Expression,  }),
		((gen_Macro) { lit("BIT57"), MT_Expression,  }),
		((gen_Macro) { lit("BIT58"), MT_Expression,  }),
		((gen_Macro) { lit("BIT59"), MT_Expression,  }),
		((gen_Macro) { lit("BIT60"), MT_Expression,  }),
		((gen_Macro) { lit("BIT61"), MT_Expression,  }),
		((gen_Macro) { lit("BIT62"), MT_Expression,  }),
		((gen_Macro) { lit("BIT63"), MT_Expression,  }),
		((gen_Macro) { lit("BIT64"), MT_Expression,  })
	));
	// base/ring.h
	gen_register_macros(args(
		((gen_Macro) { lit("ring_write_struct"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ring_read_struct"),  MT_Expression, MF_Functional })
	));
	// base/debug.h
	gen_register_macros(args(
		((gen_Macro) { lit("trap"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("assert_msg"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("assert_always"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("assert"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("invalid_path"),      MT_Expression, }),
		((gen_Macro) { lit("not_implemented"),   MT_Expression, }),
		((gen_Macro) { lit("no_op"),             MT_Expression, }),
		((gen_Macro) { lit("md_static_assert"),  MT_Expression, MF_Functional })
	));
	// base/memory.h
	gen_register_macros(args(
		((gen_Macro) { lit("KILOBYTES"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("MEGABYTES"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("GIGABYTES"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("TERABYTES"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("KB"),                               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("MB"),                               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("GB"),                               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("TB"),                               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("thousand"),                         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("million"),                          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("billion"),                          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("align_of"),                         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("member"),                           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("offset_of"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("member_from_offset"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("cast_from_member"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("cast_from_member"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_copy"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_set"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_compare"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_str_len"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_copy_struct"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_copy_array"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_copy_type"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_zero"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_zero_struct"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_zero_array"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_zero_type"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_match"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_match_struct"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_match_array"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_read"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("memory_consume"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u64_eval"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u64_inc_eval"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u64_dec_eval"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u64_eval_assign"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u64_add_eval"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u64_eval_cond_assign"),  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u32_eval"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u32_eval_assign"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ins_atomic_u32_eval_cond_assign"),  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("check_nil"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("set_nil"),                          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("MD_LINKED_LIST_PURE_MACRO"),        MT_Expression,  }),
		((gen_Macro) { lit("dll_insert_npz"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_push_back_npz"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_push_front_npz"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_remove_npz"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_push_nz"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_push_front_nz"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_pop_nz"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_stack_push_n"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_stack_pop_n"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_insert_np"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_push_back_np"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_push_front_np"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_remove_np"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_insert"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_push_back"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_push_front"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dll_remove"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_push_n"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_push_front_n"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_pop_n"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_push"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_push_front"),             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_queue_pop"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_stack_push"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sll_stack_pop"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("NO_ASAN"),                          MT_Expression,  }),
		((gen_Macro) { lit("MD_ASAN_ENABLED"),                  MT_Expression,  }),
		((gen_Macro) { lit("asan_poison_memory_region"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("asan_unpoison_memory_region"),      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("array_count"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ceil_integer_div"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("swap"),                             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("int_from_ptr"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ptr_from_int"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("compose_64bit"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("align_pow2"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("align_down_pow2"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("align_pad_pow2"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("is_pow2"),                          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("is_pow2_or_zero"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("extract_bit"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("zero_struct"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("this_function_name"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("read_only"),                        MT_Expression, MF_Functional | MF_Allow_As_Specifier }),
		((gen_Macro) { lit("thread_static"),                    MT_Expression, MF_Allow_As_Specifier }),
		((gen_Macro) { lit("from_be_u16"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("from_be_u32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("from_be_u64"),                      MT_Expression, MF_Functional })
	));
	// base/memory_substrate.h
	gen_register_macros(args(
		((gen_Macro) { lit("MD__ONES"),                    MT_Expression,  }),
		((gen_Macro) { lit("MD__HIGHS"),                   MT_Expression,  }),
		((gen_Macro) { lit("MD__HAS_ZERO"),                MT_Expression,  }),
		((gen_Macro) { lit("MD_DEFAULT_MEMORY_ALIGNMENT"), MT_Expression,  }),
		((gen_Macro) { lit("MD_HEAP_ANALYSIS"),            MT_Expression,  }),
		((gen_Macro) { lit("heap"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("md_malloc"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("md_free"),                     MT_Expression,  }),
		((gen_Macro) { lit("VARENA_DEFUALT_RESERVE"),      MT_Expression,  }),
		((gen_Macro) { lit("VARENA_DEFUALT_COMMIT"),       MT_Expression,  }),
		((gen_Macro) { lit("varena_alloc"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("varena_allocator"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("farena_from_byteslice"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("farena_from_memory"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("farena_allocator"),            MT_Expression, MF_Functional })
	));
	// base/arena.h
	gen_register_macros(args(
		((gen_Macro) { lit("ARENA_DEFAULT_BLOCK_SIZE"),      MT_Expression,  }),
		((gen_Macro) { lit("arena_alloc"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("push_array_no_zero_aligned"),    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("push_array_aligned"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("push_array_no_zero"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("push_array"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("MD_OVERRIDE_DEFAULT_ALLOCATOR"), MT_Expression,  })
	));
	// base/math.h
	gen_register_macros(args(
		((gen_Macro) { lit("md_min"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("md_max"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("clamp_top"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("clamp_bot"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("clamp"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("abs_s64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sqrt_f32"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mod_f32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("pow_f32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ceil_f32"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("floor_f32"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("round_f32"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("abs_f32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("radians_from_turns_f32"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("turns_from_radians_f32"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("degrees_from_turns_f32"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("turns_from_degrees_f32"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("degrees_from_radians_f32"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("radians_from_degrees_f32"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sin_f32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("cos_f32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("tan_f32"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sqrt_f64"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mod_f64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("pow_f64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("ceil_f64"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("floor_f64"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("round_f64"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("abs_f64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("radians_from_turns_f64"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("turns_from_radians_f64"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("degrees_from_turns_f64"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("turns_from_degrees_f64"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("degrees_from_radians_f64"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("radians_from_degrees_f64"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sin_f64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("cos_f64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("tan_f64"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v2f32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v2s64"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v2s32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v2s16"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("vec2"),                         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("add_vec2"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sub_vec2"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mul_vec2"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("div_vec2"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("scale_vec2"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dot_vec2"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("length_squared_vec2"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("length_vec2"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("normalize_vec2"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mix_vec2"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v3f32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v3s32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("vec3"),                         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("add_vec3"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sub_vec3"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mul_vec3"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("div_vec3"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("scale_vec3"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dot_vec3"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("length_squared_vec3"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("length_vec3"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("normalize_vec3"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mix_vec3"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("cross_vec3"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v4f32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("v4s32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("vec4"),                         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("add_vec4"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("sub_vec4"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mul_vec4"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("div_vec4"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("scale_vec4"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("dot_vec4"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("length_squared_vec4"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("length_vec4"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("normalize_vec4"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("mix_vec4"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r1u32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r1s32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r1u64"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r1s64"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r1f32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r2s16"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r2s16p"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r2s32"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r2s32p"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r2s64"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("r2s64p"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("rgba_from_u32_lit_comp"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("rng1s64_list_push"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("rng1s64_array_from_list_push"), MT_Expression, MF_Functional })
	));
	// base/sort.h
	gen_register_macro((gen_Macro) { lit("quick_sort"), MT_Expression, MF_Functional });
	// base/strings.h
	gen_register_macros(args(
		((gen_Macro) { lit("cstring_length"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_lit"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_lit_comp"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_varg"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_array"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_array_fixed"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_struct"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str_range"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str_cstring"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_cat"),                        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_copy"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8fv"),                          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8f"),                           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("upper_from_str8"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("lower_from_str8"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("backslashed_from_str8"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_match_lit"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_match_cstr"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_ends_with_lit"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_memory_size"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_u64"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_s64"),                   MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_bits_u32"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_bits_u64"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_first"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_aligner"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_copy"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_push"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_push_front"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_pushf"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_push_frontf"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_split"),                      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_split_by_string_chars"),      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_split_by_string_chars"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_list_join"),                  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_array_from_list"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_array_reserve"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_split_path"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_path_list_join_by_style"),    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_str16"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str16_from_str8"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from_str32"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str32_from_str8"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("date_time_string"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("file_name_date_time_string"),      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("string_from_elapsed_time"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("string_from_guid"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("indented_from_string"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("escaped_from_raw_str8"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("raw_from_escaped_str8"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("wrapped_lines_from_string"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("hex_string_from_rgba_4f32"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("fuzzy_match_find"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("fuzzy_match_range_list_copy"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_array"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_struct"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_begin"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_end"),                 MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_align"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_size"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_data"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_data_list"),      MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_u64"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_u32"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_u16"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_u8"),             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_cstr"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_serial_push_string"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_deserial_read_array"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_deserial_read_struct"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str8_from"),                       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("str16_from"),                      MT_Expression, MF_Functional })
	));
	// base/thread_context.h
	gen_register_macros(args(
		((gen_Macro) { lit("tctx_write_this_srcloc"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("scratch_begin"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("scratch_end"),            MT_Expression, MF_Functional })
	));
	// base/command_line.h
	gen_register_macros(args(
		((gen_Macro) { lit("cmd_line_insert_opt"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("cmd_line_from_string_list"), MT_Expression, MF_Functional })
	));
	// base/markup.h
	gen_register_macros(args(
		((gen_Macro) { lit("thread_namef"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("thread_name"),  MT_Expression, MF_Functional })
	));
	// base/logger.h
	gen_register_macros(args(
		((gen_Macro) { lit("LOG_DEFAULT_ARENA_BLOCK_SIZE"), MT_Expression,  }),
		((gen_Macro) { lit("log_info"),                     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_infof"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_user_error"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_user_errorf"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_info_named_block"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_info_named_blockf"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_scope_begin"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("log_scope_end"),                MT_Expression, MF_Functional })
	));


	// os/os.h
	gen_register_macros(args(
		((gen_Macro) { lit("os_handle_ist_push"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_handle_array_from_list"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_string_list_from_argcv"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_full_path_from_path"),    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_file_iter_begin"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_data_from_file_path"),    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_string_from_file_range"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_string_from_guid"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_get_current_path"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_mutex_scope"),            MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_mutex_scope_r"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_mutex_scope_W"),          MT_Expression, MF_Functional }),
		((gen_Macro) { lit("os_mutex_scope_rw_promote"), MT_Expression, MF_Functional })
	));
	// os/os_resolve.h
	gen_register_macros(args(
		((gen_Macro) { lit("OS_FEATURE_GRAPHICAL"), MT_Expression,  }),
		((gen_Macro) { lit("OS_GFX_STUB"),          MT_Expression,  })
	));
	// os/win32/os_win32_includes.h
	gen_register_macros(args(
		((gen_Macro) { lit("NOMINMAX"),            MT_Expression,  }),
		((gen_Macro) { lit("WIN32_LEAN_AND_MEAN"), MT_Expression,  }),
		((gen_Macro) { lit("WIN32_MEAN_AND_LEAN"), MT_Expression,  }),
		((gen_Macro) { lit("VC_EXTRALEAN"),        MT_Expression,  })
	));
	// os/linux/os_linux_includes.h
	gen_register_macro((gen_Macro) { lit("_GNU_SOURCE"), MT_Expression, });

	// mdesk/mdesk.h
	gen_register_macros(args(
		((gen_Macro) { lit("NodeFlag_AfterFromBefore"),     MT_Expression,  }),
		((gen_Macro) { lit("msg_list_push"),                MT_Expression, MF_Functional }),
		((gen_Macro) { lit("msg_list_pushf"),               MT_Expression, MF_Functional }),
		((gen_Macro) { lit("string_list_from_token_flags"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("token_chunk_list_push"),        MT_Expression, MF_Functional }),
		((gen_Macro) { lit("token_array_from_chunk_list"),  MT_Expression, MF_Functional }),
		((gen_Macro) { lit("each_node"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("node_rec_depth_first_pre"),     MT_Expression, MF_Functional }),
		((gen_Macro) { lit("node_rec_depth_first_pre_rev"), MT_Expression, MF_Functional }),
		((gen_Macro) { lit("push_node"),                    MT_Expression, MF_Functional }),
		((gen_Macro) { lit("string_from_children"),         MT_Expression, MF_Functional }),
		((gen_Macro) { lit("tokenize_from_text"),           MT_Expression, MF_Functional }),
		((gen_Macro) { lit("parse_from_text_tokens"),       MT_Expression, MF_Functional }),
		((gen_Macro) { lit("parse_from_text"),              MT_Expression, MF_Functional }),
		((gen_Macro) { lit("tree_from_string"),             MT_Expression, MF_Functional }),
		((gen_Macro) { lit("debug_string_list_from_tree"),  MT_Expression, MF_Functional })
	));
}
