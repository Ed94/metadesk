#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#endif

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

#ifndef MD_COMMA_OPERATOR
#define MD_COMMA_OPERATOR , // The comma operator is used by preprocessor macros to delimit arguments, so we have to represent it via a macro to prevent parsing incorrectly.
#endif

// Helper macros for argument selection
#ifndef md_select_arg_1
#define md_select_arg_1( _1, ... ) _1 // <-- Of all th args passed pick _1.
#define md_select_arg_2( _1, _2, ... ) _2 // <-- Of all the args passed pick _2.
#define md_select_arg_3( _1, _2, _3, ... ) _3 // etc..

#define md_generic_sel_entry_type             md_select_arg_1 // Use the arg expansion macro to select arg 1 which should have the type.
#define md_generic_sel_entry_function         md_select_arg_2 // Use the arg expansion macro to select arg 2 which should have the function.
#define md_generic_sel_entry_comma_delimiter  md_select_arg_3 // Use the arg expansion macro to select arg 3 which should have the comma delimiter ','.
#endif

#ifndef md_generic_call
#define md_generic_call // Just used to indicate where the call "occurs"
#endif

// ----------------------------------------------------------------------------------------------------------------------------------
#ifndef md_if_generic_selector_defined_include_slot
// md_if_generic_selector_defined_include_slot( macro ) includes a _Generic slot only if the specified macro is defined (as type, function_name).
// It takes advantage of the fact that if the macro is defined, then the expanded text will contain a comma.
// Expands to ',' if it can find (type): (function) <comma_operator: ',' >
// Where md_generic_sel_entry_comma_delimiter is specifically looking for that <comma> ,
#define md_if_generic_selector_defined_include_slot( slot_exp ) md_generic_sel_entry_comma_delimiter( slot_exp, md_generic_sel_entry_type( slot_exp, ): md_generic_sel_entry_function( slot_exp, ) MD_COMMA_OPERATOR, , )
//                                                                   ^ Selects the comma                          ^ is the type                            ^ is the function                         ^ Insert a comma
// The slot won't exist if that comma is not found.
#endif

// For the occastion where an expression didn't resolve to a selection option the "default: <value>" will be set to:
typedef struct MD_UNRESOLVED_GENERIC_SELECTION MD_UNRESOLVED_GENERIC_SELECTION;
struct MD_UNRESOLVED_GENERIC_SELECTION {
	void* _THE_VOID_SLOT_;
};
MD_UNRESOLVED_GENERIC_SELECTION const md_assert_generic_sel_fail = {0};
// Which will provide the message:  error: called object type 'struct NO_RESOLVED_GENERIC_SELECTION' is not a function or function pointer
// ----------------------------------------------------------------------------------------------------------------------------------

// Below are generated on demand for an overlaod depdendent on a type:
// ----------------------------------------------------------------------------------------------------------------------------------
#define function_generic_example( selector_arg ) _Generic(                         \
(selector_arg), /* Select Via Expression*/                                         \
  /* Extendibility slots: */                                                       \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_1__function_sig ) \
	md_if_generic_selector_defined_include_slot( GENERIC_SLOT_2__function_sig )    \
	default: md_assert_generic_sel_fail                                            \
) md_generic_call( selector_arg )
// ----------------------------------------------------------------------------------------------------------------------------------

// Then each definiton of a function has an associated define:
// #define GENERIC_SLOT_<#>_<generic identifier> <typename>, <function_to_resolve>

// Then somehwere later on
// <etc> <return_type> <function_id> ( <arguments> ) { <implementation> }

// Concrete example:

// To add support for long:
#define MD_GENERIC_SLOT_1__example_hash long, md_generic_example_hash__P_long
size_t md_generic_example_hash__P_long( long val ) { return val * 2654435761ull; }

// To add support for long long:
#define MD_GENERIC_SLOT_2__example_hash  long long, md_generic_example_hash__P_long_long
size_t md_generic_example_hash__P_long_long( long long val ) { return val * 2654435761ull; }

// If using an Editor with support for syntax hightlighting macros:
// MD_GENERIC_SLOT_1__example_hash and GENERIC_SLOT_2__example_hash should show color highlighting indicating the slot is enabled,
// or, "defined" for usage during the compilation pass that handles the _Generic instrinsic.
#define generic_example_hash( function_arguments ) _Generic(                       \
(function_arguments), /* Select Via Expression*/                                   \
  /* Extendibility slots: */                                                       \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_1__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_2__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_3__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_4__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_5__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_6__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_7__example_hash ) \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_8__example_hash ) \
	default: md_assert_generic_sel_fail                                            \
) md_generic_call( function_arguments )

// Additional Variations:

// If the function takes more than one argument the following is used:
#define md_function_generic_example_varadic( selector_arg, ... ) _Generic(         \
(selector_arg),                                                                    \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_1__function_sig ) \
	md_if_generic_selector_defined_include_slot( GENERIC_SLOT_2__function_sig )    \
	/* ... */                                                                      \
	md_if_generic_selector_defined_include_slot(GENERIC_SLOT_N__function_sig )     \
	default: md_assert_generic_sel_fail                                            \
) md_generic_call( selector_arg, __VA_ARG__ )

// If the function does not take the arugment as a parameter:
#define md_function_generic_example_direct_type( selector_arg ) _Generic(          \
( md_type_to_expression(selector_arg) ),                                           \
	md_if_generic_selector_defined_include_slot( MD_GENERIC_SLOT_1__function_sig ) \
	md_if_generic_selector_defined_include_slot( GENERIC_SLOT_2__function_sig )    \
	/* ... */                                                                      \
	md_if_generic_selector_defined_include_slot(GENERIC_SLOT_N__function_sig )     \
	default: md_assert_generic_sel_fail                                            \
) md_generic_call(selector_arg)


#ifndef md_type_to_expression
// Used to keep the _Generic keyword happy as bare types are not considered "expressions"
#define md_type_to_expression(type) (* (type*)NULL)
// Instead of using this macro, it should be directly expanded by code generation.
#endif

// _Generic also suffers from type compability flatting distinctions between typedef by resolving the selection to the underling type and qualifier.
// To resolve this these distinctions must be enforce by keeping "compatible" types in separate _Generic expressions:

#define example_with__l2(expr) _Generic( \
(expr),                                  \
    MD_S64    : example_with_s64,        \
    default: md_assert_generic_sel_fail  \
)
#define example_with(expr)                          \
_Generic((expr),                                    \
    MD_SSIZE   : example_with_SSIZEZ                \
    default : example_with_not_SSIZE(allocator, in) \
) md_generic_call(allocator, in)

// This can be made more concise with he following "layer" indicating _Generic macros

#define _Generic_L2(expr, ...) default: _Generic(expr, __VA_ARGS__)
#define _Generic_L3(expr, ...) default: _Generic(expr, __VA_ARGS__)

#define example_with_generic_layers(expr)   \
_Generic(   (expr),                         \
	MD_S64 : example_with_s64,              \
_Generic_L2((expr),                         \
	MD_SSIZE: example_with_SSIZZE           \
	default: md_assert_generic_sel_fail     \
),                                          \
) md_generic_call(expr)

// Removing example definitions
#undef example_with
#undef example_with__l2
#undef example_with_generic_layers
#undef function_generic_example
#undef MD_GENERIC_SLOT_1__example_hash
#undef MD_GENERIC_SLOT_2__example_hash
#undef generic_example_hash
#undef md_function_generic_example_varadic
#undef md_function_generic_example_direct_type
#undef generic_example_do_something_with
