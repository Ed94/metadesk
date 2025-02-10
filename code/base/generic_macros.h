#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
// NOTE(Ed): For C++ generation, this file will not be injected, any macros that are used will either be injected as transparent defines
// or have their usage removed during the library generation pass.
#endif

#pragma region _Generic Macros
//   ____                       _        ______                _   _                ____                  _                 __ _
//  / ___}                     (_)      |  ____}              | | (_)              / __ \                | |               | |(_)
// | | ___  ___ _ __   ___ _ __ _  ___  | |__ _   _ _ __   ___| |_ _  ___  _ __   | |  | |_   _____ _ __ | | ___   __ _  __| | _ _ __   __ _
// | |{__ |/ _ \ '_ \ / _ \ '__} |/ __| |  __} | | | '_ \ / __} __} |/ _ \| '_ \  | |  | \ \ / / _ \ '_ }| |/ _ \ / _` |/ _` || | '_ \ / _` |
// | |__j |  __/ | | |  __/ |  | | (__  | |  | |_| | | | | (__| l_| | (_) | | | | | l__| |\ V /  __/ |   | | (_) | (_| | (_| || | | | | (_| |
//  \____/ \___}_l l_l\___}_l  l_l\___| l_l   \__,_l_l l_l\___}\__}_l\___/l_l l_l  \____/  \_/ \___}_l   l_l\___/ \__,_l\__,_l|_|_| |_|\__, |
// This implemnents macros for utilizing "The Naive Extendible _Generic Macro" explained in:                                            __| |
// https://github.com/JacksonAllan/CC/blob/main/articles/Better_C_Generics_Part_1_The_Extendible_Generic.md                            {___/
// It was choosen over the more novel implementations to keep the macros as easy to understand and unobfuscated as possible.
// --------------------------------------------------------------------------------------------------------------------------------------------
// For explanation of intended usage with staged metaprogramming see: https://github.com/Ed94/gencpp/tree/main/gen_c_library#macro-usage
// Techncially this can be used for more than just implementing function overloading.
// Additional info: https://www.chiark.greenend.org.uk/~sgtatham/quasiblog/c11-generic/
// --------------------------------------------------------------------------------------------------------------------------------------------

#ifndef COMMA_OPERATOR
#define COMMA_OPERATOR , // The comma operator is used by preprocessor macros to delimit arguments, so we have to represent it via a macro to prevent parsing incorrectly.
#endif

// Helper macros for argument selection
#ifndef select_arg_1
#define select_arg_1( _1, ... ) _1 // <-- Of all th args passed pick _1.
#define select_arg_2( _1, _2, ... ) _2 // <-- Of all the args passed pick _2.
#define select_arg_3( _1, _2, _3, ... ) _3 // etc..

#define generic_sel_entry_type             select_arg_1 // Use the arg expansion macro to select arg 1 which should have the type.
#define generic_sel_entry_function         select_arg_2 // Use the arg expansion macro to select arg 2 which should have the function.
#define generic_sel_entry_comma_delimiter  select_arg_3 // Use the arg expansion macro to select arg 3 which should have the comma delimiter ','.
#endif

#ifndef resolved_function_call
#define resolved_function_call // Just used to indicate where the call "occurs"
#endif

// ----------------------------------------------------------------------------------------------------------------------------------
#ifndef if_generic_selector_defined_include_slot
// if_generic_selector_defined_include_slot( macro ) includes a _Generic slot only if the specified macro is defined (as type, function_name).
// It takes advantage of the fact that if the macro is defined, then the expanded text will contain a comma.
// Expands to ',' if it can find (type): (function) <comma_operator: ',' >
// Where generic_sel_entry_comma_delimiter is specifically looking for that <comma> ,
#define if_generic_selector_defined_include_slot( slot_exp ) generic_sel_entry_comma_delimiter( slot_exp, generic_sel_entry_type( slot_exp, ): generic_sel_entry_function( slot_exp, ) COMMA_OPERATOR, , )
//                                                            ^ Selects the comma                          ^ is the type                        ^ is the function                       ^ Insert a comma
// The slot won't exist if that comma is not found.
#endif

// For the occastion where an expression didn't resolve to a selection option the "default: <value>" will be set to:
typedef struct UNRESOLVED_GENERIC_SELECTION UNRESOLVED_GENERIC_SELECTION;
struct UNRESOLVED_GENERIC_SELECTION {
	void* _THE_VOID_SLOT_;
};
UNRESOLVED_GENERIC_SELECTION const assert_generic_selection_fail = {0};
// Which will provide the message:  error: called object type 'struct NO_RESOLVED_GENERIC_SELECTION' is not a function or function pointer
// ----------------------------------------------------------------------------------------------------------------------------------

// Below are generated on demand for an overlaod depdendent on a type:
// ----------------------------------------------------------------------------------------------------------------------------------
#define function_generic_example( selector_arg ) _Generic(                   \
(selector_arg), /* Select Via Expression*/                                   \
  /* Extendibility slots: */                                                 \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_1__function_sig ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_2__function_sig ) \
	default: assert_generic_selection_fail                                   \
) resolved_function_call( selector_arg )
// ----------------------------------------------------------------------------------------------------------------------------------

// Then each definiton of a function has an associated define:
#// #define GENERIC_SLOT_<#>_<generic identifier> <typename>, <function_to_resolve>

// Then somehwere later on
// <etc> <return_type> <function_id> ( <arguments> ) { <implementation> }

// Concrete example:

// To add support for long:
#define GENERIC_SLOT_1__example_hash long, example_hash__P_long
size_t example_hash__P_long( long val ) { return val * 2654435761ull; }

// To add support for long long:
#define GENERIC_SLOT_2__example_hash  long long, example_hash__P_long_long
size_t example_hash__P_long_long( long long val ) { return val * 2654435761ull; }

// If using an Editor with support for syntax hightlighting macros:
// GENERIC_SLOT_1__example_hash and GENERIC_SLOT_2__example_hash should show color highlighting indicating the slot is enabled,
// or, "defined" for usage during the compilation pass that handles the _Generic instrinsic.
#define hash_example( function_arguments ) _Generic(                         \
(function_arguments), /* Select Via Expression*/                             \
  /* Extendibility slots: */                                                 \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_1__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_2__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_3__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_4__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_5__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_6__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_7__example_hash ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_8__example_hash ) \
	default: assert_generic_selection_fail                                   \
) resolved_function_call( function_arguments )

// Additional Variations:

// If the function takes more than one argument the following is used:
#define function_generic_example_varadic( selector_arg, ... ) _Generic(       \
(selector_arg),                                                               \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_1__function_sig )  \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_2__function_sig )  \
	/* ... */                                                                 \
	if_generic_selector_defined_include_slot(GENERIC_SLOT_N__function_sig )   \
	default: assert_generic_selection_fail                                    \
) resolved_function_call( selector_arg, __VA_ARG__ )

// If the function does not take the arugment as a parameter:
#define function_generic_example_direct_type( selector_arg ) _Generic(       \
( type_to_expression(selector_arg) ),                                        \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_1__function_sig ) \
	if_generic_selector_defined_include_slot( GENERIC_SLOT_2__function_sig ) \
	/* ... */                                                                \
	if_generic_selector_defined_include_slot(GENERIC_SLOT_N__function_sig )  \
	default: assert_generic_selection_fail                                   \
) resolved_function_call()


#ifndef type_to_expression
// Used to keep the _Generic keyword happy as bare types are not considered "expressions"
#define type_to_expression(type) (* (type*)NULL)
#endif
// Instead of using this macro, it should be directly expanded by code generation.

// _Generic also suffers from type compability flatting distinctions between typedef by resolving the selection to the underling type and qualifier.
// To resolve this these distinctions must be enforced using structs that enforce the typedef

#ifndef distinct_register_selector
// Will define the metadata struct for generic selection usage with the distinct_lookup function macro
// Generally does not need to be used unless getting an error similar to: "_Generic association compatible with previous association type"
#define distinct_register_selector(type) typedef struct { type* UNUSED__; } Distinct_Typedef_ ## type
#endif

#ifndef distinct_lookup
// This is used by a generic selector to lookup a unique struct typeid of a typedef if distinct_register_selector function macro was utilized
// Only necessary to use with _Generic if getting an error similar to: "_Generic association compatible with previous association type"
#define distinct_lookup(type) Distinct_Typedef_ ## type
#endif

// Example: -------------------------------------------------------------------------------------------------------------------------
// Now this can work so long as typedef_generic_selector was used on the S64 && SSIZE typedefs
#define do_something_with(selector_arg, in) _Generic(                                        \
(selector_arg),                                                                              \
	distinct_lookup(S64)  : do_something_with_s64,   default: assert_generic_selection_fail, \
	distinct_lookup(SSIZE): do_something_with_ssize, default: assert_generic_selection_fail, \
	default               : assert_generic_selection_fail                                    \
) resolved_function_call(selector_arg)
// ----------------------------------------------------------------------------------------------------------------------------------

#ifndef distinct
// Can inlay with typedef and a distinct_register_selector. Usage: typedef distinct(int, SomeType);
// Generally does not need to be used unless getting an error similar to: "_Generic association compatible with previous association type"
// Use distinct_lookup, to resolve the distinct type for the generic selection entry.
#define distinct(underlying_type, type) underlying_type type; distinct_register_selector(type)
#endif

// Removing example definitions
#undef function_generic_example
#undef GENERIC_SLOT_1__example_hash
#undef GENERIC_SLOT_2__example_hash
#undef hash_example
#undef function_generic_example_varadic
#undef function_generic_example_direct_type
#undef do_something_with
#pragma endregion _Generic Macros
