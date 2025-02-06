
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

#define MD_COMMA_OPERATOR , // The comma operator is used by preprocessor macros to delimit arguments, so we have to represent it via a macro to prevent parsing incorrectly.

// Helper macros for argument selection
#define MD_SELECT_ARG_1( _1, ... ) _1 // <-- Of all th args passed pick _1.
#define MD_SELECT_ARG_2( _1, _2, ... ) _2 // <-- Of all the args passed pick _2.
#define MD_SELECT_ARG_3( _1, _2, _3, ... ) _3 // etc..

#define MD_GENERIC_SEL_ENTRY_TYPE             MD_SELECT_ARG_1 // Use the arg expansion macro to select arg 1 which should have the type.
#define MD_GENERIC_SEL_ENTRY_FUNCTION         MD_SELECT_ARG_2 // Use the arg expansion macro to select arg 2 which should have the function.
#define MD_GENERIC_SEL_ENTRY_COMMA_DELIMITER  MD_SELECT_ARG_3 // Use the arg expansion macro to select arg 3 which should have the comma delimiter ','.

#define MD_RESOLVED_FUNCTION_CALL // Just used to indicate where the call "occurs"

// ----------------------------------------------------------------------------------------------------------------------------------
// MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( macro ) includes a _Generic slot only if the specified macro is defined (as type, function_name).
// It takes advantage of the fact that if the macro is defined, then the expanded text will contain a comma.
// Expands to ',' if it can find (type): (function) <comma_operator: ',' >
// Where MD_GENERIC_SEL_ENTRY_COMMA_DELIMITER is specifically looking for that <comma> ,
#define MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( slot_exp ) MD_GENERIC_SEL_ENTRY_COMMA_DELIMITER( slot_exp, MD_GENERIC_SEL_ENTRY_TYPE( slot_exp, ): MD_GENERIC_SEL_ENTRY_FUNCTION( slot_exp, ) MD_COMMA_OPERATOR, , )
//                                                          ^ Selects the comma                              ^ is the type                             ^ is the function                       ^ Insert a comma
// The slot won't exist if that comma is not found.

// For the occastion where an expression didn't resolve to a selection option the "default: <value>" will be set to:
typedef struct METADESK_NO_RESOLVED_GENERIC_SELECTION METADESK_NO_RESOLVED_GENERIC_SELECTION;
struct METADESK_NO_RESOLVED_GENERIC_SELECTION {
	void* _THE_VOID_SLOT_;
};
METADESK_NO_RESOLVED_GENERIC_SELECTION const MD_generic_selection_fail = {0};
// Which will provide the message:  error: called object type 'struct NO_RESOLVED_GENERIC_SELECTION' is not a function or function pointer
// ----------------------------------------------------------------------------------------------------------------------------------

// Below are generated on demand for an overlaod depdendent on a type:
// ----------------------------------------------------------------------------------------------------------------------------------
#define MD_FUNCTION_GENERIC_EXAMPLE( selector_arg ) _Generic(             \
(selector_arg), /* Select Via Expression*/                                 \
  /* Extendibility slots: */                                               \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1__function_sig ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2__function_sig ) \
	default: MD_generic_selection_fail                                    \
) MD_RESOLVED_FUNCTION_CALL( selector_arg )
// ----------------------------------------------------------------------------------------------------------------------------------

// Then each definiton of a function has an associated define:
#// #define GENERIC_SLOT_<#>_<generic identifier> <typename>, <function_to_resolve>

// Then somehwere later on
// <etc> <return_type> <function_id> ( <arguments> ) { <implementation> }

// Concrete example:

// To add support for long:
#define GENERIC_SLOT_1_MD_example_hash long, MD_example_hash__P_long
size_t MD_example_hash__P_long( long val ) { return val * 2654435761ull; }

// To add support for long long:
#define GENERIC_SLOT_2_MD_example_hash  long long, MD_example_hash__P_long_long
size_t MD_example_hash__P_long_long( long long val ) { return val * 2654435761ull; }

// If using an Editor with support for syntax hightlighting macros:
// GENERIC_SLOT_1_MD_example_hash and GENERIC_SLOT_2_MD_example_hash should show color highlighting indicating the slot is enabled,
// or, "defined" for usage during the compilation pass that handles the _Generic instrinsic.
#define MD_hash_example( function_arguments ) _Generic(                     \
(function_arguments), /* Select Via Expression*/                            \
  /* Extendibility slots: */                                                \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_3_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_4_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_5_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_6_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_7_MD_example_hash ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_8_MD_example_hash ) \
	default: MD_generic_selection_fail                                      \
) MD_RESOLVED_FUNCTION_CALL( function_arguments )

// Additional Variations:

// If the function takes more than one argument the following is used:
#define MD_FUNCTION_GENERIC_EXAMPLE_VARADIC( selector_arg, ... ) _Generic( \
(selector_arg),                                                            \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1__function_sig )  \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2__function_sig )  \
	/* ... */                                                              \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GENERIC_SLOT_N__function_sig )   \
	default: MD_generic_selection_fail                                     \
) MD_RESOLVED_FUNCTION_CALL( selector_arg, __VA_ARG__ )

// If the function does not take the arugment as a parameter:
#define MD_FUNCTION_GENERIC_EXAMPLE_DIRECT_TYPE( selector_arg ) _Generic( \
( MD_TYPE_TO_EXP(selector_arg) ),                                         \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_1__function_sig ) \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT( GENERIC_SLOT_2__function_sig ) \
	/* ... */                                                             \
	MD_IF_MACRO_DEFINED_INCLUDE_THIS_SLOT(GENERIC_SLOT_N__function_sig )  \
	default: MD_generic_selection_fail                                    \
) MD_RESOLVED_FUNCTION_CALL()

// Used to keep the _Generic keyword happy as bare types are not considered "expressions"
#define MD_TYPE_TO_EXP(type) (* (type*)NULL)
// Instead of using this macro, you'll see it directly expanded by the code generation.

// typedef void* MD_GenericExampleType;
// MD_FUNCTION_GENERIC_EXAMPLE_DIRECT_TYPE( MD_GenericExampleType );
#pragma endregion _Generic Macros
