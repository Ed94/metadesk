#if INTELLISENSE_DIRECTIVES
#	pragma once
#	include "os/os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Messages

typedef enum MsgKind
{
	MsgKind_Null,
	MsgKind_Note,
	MsgKind_Warning,
	MsgKind_Error,
	MsgKind_FatalError,
}
MsgKind;

typedef struct Node Node;
typedef struct Msg  Msg;
struct Msg
{
	Msg*    next;
	Node*   node;
	MsgKind kind;
	String8 string;
};

typedef struct MsgList MsgList;
struct MsgList
{
	Msg*    first;
	Msg*    last;
	U64     count;
	MsgKind worst_message_kind;
};

////////////////////////////////
//~ rjf: Token Types

typedef U32 TokenFlags;
enum
{
	// TODO(Ed): Track type of comment, and opening/closing main delimiter. 
	// (The parser needs that info later and just forces str_matches that werent necessary)

	// rjf: base kind info
	TokenFlag_Identifier          = (1 << 0),
	TokenFlag_Numeric             = (1 << 1),
	TokenFlag_StringLiteral       = (1 << 2),
	TokenFlag_Symbol              = (1 << 3),
	TokenFlag_Reserved            = (1 << 4),
	TokenFlag_Comment             = (1 << 5),
	TokenFlag_Whitespace          = (1 << 6),
	TokenFlag_Newline             = (1 << 7),

	// rjf: decoration info
	TokenFlag_StringSingleQuote   = (1 << 8),
	TokenFlag_StringDoubleQuote   = (1 << 9),
	TokenFlag_StringTick          = (1 << 10),
	TokenFlag_StringTriplet       = (1 << 11),
	
	// rjf: error info
	TokenFlag_BrokenComment       = (1 << 12),
	TokenFlag_BrokenStringLiteral = (1 << 13),
	TokenFlag_BadCharacter        = (1 << 14),
};

typedef U32 TokenFlagGroups;
enum
{
	TokenFlagGroup_Comment    = TokenFlag_Comment,
	TokenFlagGroup_Whitespace = (TokenFlag_Whitespace| TokenFlag_Newline),
	TokenFlagGroup_Irregular  = (TokenFlagGroup_Comment  | TokenFlagGroup_Whitespace),
	TokenFlagGroup_Regular    = ~TokenFlagGroup_Irregular,
	TokenFlagGroup_Label      = (TokenFlag_Identifier | TokenFlag_Numeric | TokenFlag_StringLiteral | TokenFlag_Symbol),
	TokenFlagGroup_Error      = (TokenFlag_BrokenComment | TokenFlag_BrokenStringLiteral | TokenFlag_BadCharacter),
};

typedef struct Token Token;
struct Token
{
	Rng1U64    range;
	TokenFlags flags;
};

typedef struct TokenChunkNode TokenChunkNode;
struct TokenChunkNode
{
	TokenChunkNode* next;
	Token*          v;
	U64             count;
	U64             cap;
};

typedef struct TokenChunkList TokenChunkList;
struct TokenChunkList
{
	TokenChunkNode* first;
	TokenChunkNode* last;
	U64 chunk_count;
	U64 total_token_count;
};

typedef struct TokenArray TokenArray;
struct TokenArray
{
	Token* v;
	U64    count;
};

////////////////////////////////
//~ rjf: Node Types

typedef enum NodeKind NodeKind;
enum NodeKind
{
	NodeKind_Nil,
	NodeKind_File,
	NodeKind_ErrorMarker,
	NodeKind_Main,
	NodeKind_Tag,
	NodeKind_List,
	NodeKind_Reference,
	NodeKind_COUNT
};

typedef U32 NodeFlags;
enum
{
	NodeFlag_MaskSetDelimiters          = (0x3F << 0),
	NodeFlag_HasParenLeft               = (1    << 0),
	NodeFlag_HasParenRight              = (1    << 1),
	NodeFlag_HasBracketLeft             = (1    << 2),
	NodeFlag_HasBracketRight            = (1    << 3),
	NodeFlag_HasBraceLeft               = (1    << 4),
	NodeFlag_HasBraceRight              = (1    << 5),
	
	NodeFlag_MaskSeparators             = (0xF << 6),
	NodeFlag_IsBeforeSemicolon          = (1   << 6),
	NodeFlag_IsAfterSemicolon           = (1   << 7),
	NodeFlag_IsBeforeComma              = (1   << 8),
	NodeFlag_IsAfterComma               = (1   << 9),
	
	NodeFlag_MaskStringDelimiters       = (0xF << 10),
	NodeFlag_StringSingleQuote          = (1   << 10),
	NodeFlag_StringDoubleQuote          = (1   << 11),
	NodeFlag_StringTick                 = (1   << 12),
	NodeFlag_StringTriplet              = (1   << 13),
	
	NodeFlag_MaskLabelKind              = (0xF << 14),
	NodeFlag_Numeric                    = (1   << 14),
	NodeFlag_Identifier                 = (1   << 15),
	NodeFlag_StringLiteral              = (1   << 16),
	NodeFlag_Symbol                     = (1   << 17),
};
#define NodeFlag_AfterFromBefore(f) ((f) << 1)

typedef struct Node Node;
struct Node
{
	// rjf: tree links
	Node* next;
	Node* prev;
	Node* parent;
	Node* first;
	Node* last;
	
	// rjf: tag links
	Node* first_tag;
	Node* last_tag;
	
	// rjf: node info
	NodeKind  kind;
	NodeFlags flags;
	String8   string;
	String8   raw_string;
	
	// rjf: source code info
	U64 src_offset;
	
	// rjf: user-controlled generation number
	//
	// (unused by mdesk layer, but can be used by usage code to use Node trees
	// in a "retained mode" way, where stable generational handles can be formed
	// to nodes)
	U64 user_gen;
	
	// rjf: extra padding to 128 bytes
	U64 _unused_[2];
};

typedef struct NodeRec NodeRec;
struct NodeRec
{
	Node* next;
	S32   push_count;
	S32   pop_count;
};

////////////////////////////////
//~ rjf: Text -> Tokens Types

typedef struct TokenizeResult TokenizeResult;
struct TokenizeResult
{
  TokenArray tokens;
  MsgList    msgs;
};

////////////////////////////////
//~ rjf: Tokens -> Tree Types

typedef struct ParseResult ParseResult;
struct ParseResult
{
	Node*   root;
	MsgList msgs;
};

////////////////////////////////
// Context

typedef struct Context Context;
struct Context
{
	// Currently, this is only relevant if the user is utilizing this library via bindings
	// or they are not utilizing metadesk's hosted `entry_point` runtime

	// Its recommended that the user hooks up 
	// their own arena allocators to the thread context
	// which will be utilized for thread local scratch arena
	TCTX thread_ctx;

	// Just as with TCTX its recommended the user setup the arena allocators
	// for te os context
	OS_Context os_ctx;

	// This skips the os_init process but that means the user is expected to setup
	// the thread context's arenas for the process
	B32 dont_init_os;
};

////////////////////////////////
//~ rjf: Globals

inline Node*
nil_node()
{
	read_only local_persist 
	Node nil =
	{
		&nil,
		&nil,
		&nil,
		&nil,
		&nil,
		&nil,
		&nil,
	};
	return &nil;
}

////////////////////////////////
// Context lifetime Functios

// NOTE(Ed): The are see comments in Context struct, this is only necessary when not utilizing 
// the metadesk os runtime provided entry_point interface

MD_API void init(Context* ctx);
MD_API void deinit(Context* ctx); // Does nothing for now.

////////////////////////////////
//~ rjf: Message Type Functions

       void msg_list_push__arena (Arena*        arena, MsgList* msgs, Node* node, MsgKind kind, String8 string);
MD_API void msg_list_push__ainfo (AllocatorInfo ainfo, MsgList* msgs, Node* node, MsgKind kind, String8 string);
       void msg_list_pushf__arena(Arena*        arena, MsgList* msgs, Node* node, MsgKind kind, char *fmt, ...);
       void msg_list_pushf__ainfo(AllocatorInfo ainfo, MsgList* msgs, Node* node, MsgKind kind, char *fmt, ...);

#define msg_list_push(allocator, msgs, node, kind, string)            _Generic(allocator, Arena*: msg_list_push__arena,  AllocatorInfo: msg_list_push__ainfo,  default: assert_generic_selection_fail) resolved_function_call(allocator, msgs, node, kind, string)
#define msg_list_pushf(allocator, msgs, node, kind, string, fmt, ...) _Generic(allocator, Arena*: msg_list_pushf__arena, AllocatorInfo: msg_list_pushf__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, msgs, node, kind, fmt, __VA_ARGS__)

MD_API void msg_list_concat_in_place(MsgList* dst, MsgList* to_push);

force_inline void msg_list_push__arena(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, String8 string) { msg_list_push__ainfo(arena_allocator(arena), msgs, node, kind, string); }

inline void
msg_list_pushf__arena(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8 string = str8fv(arena, fmt, args);
	msg_list_push(arena, msgs, node, kind, string);
	va_end(args);
}

inline void
msg_list_pushf__ainfo(AllocatorInfo ainfo, MsgList* msgs, Node* node, MsgKind kind, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8 string = str8fv(ainfo, fmt, args);
	msg_list_push(ainfo, msgs, node, kind, string);
	va_end(args);
}

////////////////////////////////
//~ rjf: Token Type Functions

Token token_make (Rng1U64    range, TokenFlags flags);
B32   token_match(Token      a,     Token      b);

MD_API String8 content_string_from_token_flags_str8(TokenFlags flags, String8 string);

       String8List string_list_from_token_flags__arena(Arena*        arena, TokenFlags flags);
MD_API String8List string_list_from_token_flags__ainfo(AllocatorInfo ainfo, TokenFlags flags);
MD_API void        token_chunk_list_push__arena       (Arena*        arena, TokenChunkList* list, U64 cap, Token token);
MD_API void        token_chunk_list_push__ainfo       (AllocatorInfo ainfo, TokenChunkList* list, U64 cap, Token token);
MD_API TokenArray  token_array_from_chunk_list__arena (Arena*        arena, TokenChunkList* chunks);
MD_API TokenArray  token_array_from_chunk_list__ainfo (AllocatorInfo ainfo, TokenChunkList* chunks);

#define string_list_from_token_flags(allocator, flags)     _Generic(allocator, Arena*: string_list_from_token_flags__arena, AllocatorInfo: string_list_from_token_flags__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, flags)
#define token_chunk_list_push(allocator, list, cap, token) _Generic(allocator, Arena*: token_chunk_list_push__arena,        AllocatorInfo: token_chunk_list_push__ainfo,        default: assert_generic_selection_fail) resolved_function_call(allocator, list, cap, token)
#define token_array_from_chunk_list(allocator, chunks)     _Generic(allocator, Arena*: token_array_from_chunk_list__arena,  AllocatorInfo: token_array_from_chunk_list__ainfo,  default: assert_generic_selection_fail) resolved_function_call(allocator, chunks)

force_inline String8List string_list_from_token_flags__arena(Arena* arena, TokenFlags flags)                           { return string_list_from_token_flags__ainfo(arena_allocator(arena), flags); }
force_inline void        token_chunk_list_push__arena       (Arena* arena, TokenChunkList* list, U64 cap, Token token) { token_chunk_list_push__ainfo(arena_allocator(arena), list, cap, token); }
force_inline TokenArray  token_array_from_chunk_list__arena (Arena* arena, TokenChunkList* chunks)                     { return token_array_from_chunk_list__ainfo(arena_allocator(arena), chunks); }

inline Token
token_make(Rng1U64 range, TokenFlags flags) {
  Token  token = { range, flags };
  return token;
}

inline B32
token_match(Token a, Token b) {
	return (a.range.min == b.range.min &&
			a.range.max == b.range.max &&
			a.flags     == b.flags       );
}

////////////////////////////////
//~ rjf: Node Type Functions

//- rjf: flag conversions

inline NodeFlags
node_flags_from_token_flags(TokenFlags flags)
{
	NodeFlags result = 0;
	result |= NodeFlag_Identifier        *!! (flags & TokenFlag_Identifier       );
	result |= NodeFlag_Numeric           *!! (flags & TokenFlag_Numeric          );
	result |= NodeFlag_StringLiteral     *!! (flags & TokenFlag_StringLiteral    );
	result |= NodeFlag_Symbol            *!! (flags & TokenFlag_Symbol           );
	result |= NodeFlag_StringSingleQuote *!! (flags & TokenFlag_StringSingleQuote);
	result |= NodeFlag_StringDoubleQuote *!! (flags & TokenFlag_StringDoubleQuote);
	result |= NodeFlag_StringTick        *!! (flags & TokenFlag_StringTick       );
	result |= NodeFlag_StringTriplet     *!! (flags & TokenFlag_StringTriplet    );
	return result;
}

//- rjf: nil

B32 node_is_nil(Node* node) { return (node == 0 || node == nil_node() || node->kind == NodeKind_Nil); }

//- rjf: iteration

#define each_node(it, first) (Node* it = first; !node_is_nil(it); it = it->next)

MD_API NodeRec node_rec_depth_first(Node* node, Node* subtree_root, U64 child_off, U64 sib_off);

#define node_rec_depth_first_pre(node, subtree_root)     node_rec_depth_first((node), (subtree_root), offset_of(Node, first), offset_of(Node, next))
#define node_rec_depth_first_pre_rev(node, subtree_root) node_rec_depth_first((node), (subtree_root), offset_of(Node, last),  offset_of(Node, prev))

//- rjf: tree building

Node* push_node__arena(Arena*        arena, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset);
Node* push_node__ainfo(AllocatorInfo ainfo, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset);

#define push_node(allocator, kind, flags, string, raw_string, src_offset) _Generic(allocator, Arena*: push_node__arena, AllocatorInfo: push_node__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, kind, flags, string, raw_string, src_offset)

void  node_insert_tag  (Node* parent, Node* prev_child, Node* node);
void  node_insert_child(Node* parent, Node* prev_child, Node* node);
void  node_push_child  (Node* parent, Node* node);
void  node_push_tag    (Node* parent, Node* node);
void  unhook           (Node* node);

inline Node* push_node__arena(Arena* arena, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset) { push_node__ainfo(arena_allocator(arena), kind, flags, string, raw_string, src_offset); }

inline Node*
push_node__ainfo(AllocatorInfo ainfo, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset) {
	Node* node = alloc_array(ainfo, Node, 1);
	node->first      = node->last = node->parent = node->next = node->prev = node->first_tag = node->last_tag = nil_node();
	node->kind       = kind;
	node->flags      = flags;
	node->string     = string;
	node->raw_string = raw_string;
	node->src_offset = src_offset;
	return node;
}

inline void
node_insert_child(Node* parent, Node* prev_child, Node* node) {
	node->parent = parent;
	dll_insert_npz(nil_node(), parent->first, parent->last, prev_child, node, next, prev);
}

inline void
node_insert_tag(Node* parent, Node* prev_child, Node* node) {
	node->kind   = NodeKind_Tag;
	node->parent = parent;
	dll_insert_npz(nil_node(), parent->first_tag, parent->last_tag, prev_child, node, next, prev);
}

inline void
node_push_child(Node* parent, Node* node) {
  node->parent = parent;
  dll_push_back_npz(nil_node(), parent->first, parent->last, node, next, prev);
}

inline void
node_push_tag(Node* parent, Node* node) {
  node->kind   = NodeKind_Tag;
  node->parent = parent;
  dll_push_back_npz(nil_node(), parent->first_tag, parent->last_tag, node, next, prev);
}

//- rjf: tree introspection

Node* node_from_chain_string(Node* first, Node* opl, String8 string, StringMatchFlags flags);
Node* node_from_chain_index (Node* first, Node* opl, U64 index);
Node* node_from_chain_flags (Node* first, Node* opl, NodeFlags flags);
U64   index_from_node       (Node* node);
Node* root_from_node        (Node* node);
Node* child_from_string     (Node* node, String8 child_string, StringMatchFlags flags);
Node* tag_from_string       (Node* node, String8 tag_string, StringMatchFlags flags);
Node* child_from_index      (Node* node, U64 index);
Node* tag_from_index        (Node* node, U64 index);
Node* tag_arg_from_index    (Node* node, String8 tag_string, StringMatchFlags flags, U64 index);
Node* tag_arg_from_string   (Node* node, String8 tag_string, StringMatchFlags tag_str_flags, String8 arg_string, StringMatchFlags arg_str_flags);
B32   node_has_child        (Node* node, String8 string, StringMatchFlags flags);
B32   node_has_tag          (Node* node, String8 string, StringMatchFlags flags);
U64   child_count_from_node (Node* node);
U64   tag_count_from_node   (Node* node);

MD_API String8 string_from_children__arena(Arena*        arena, Node* root);
       String8 string_from_children__ainfo(AllocatorInfo ainfo, Node* root);

#define string_from_children(allocator, root) _Generic(allocator, Arena*: string_from_children__arena, AllocatorInfo: string_from_children__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, root)

force_inline String8 string_from_children__arena(Arena* arena, Node* root) { return string_from_children__ainfo(arena_allocator(arena), root); }

inline Node*
node_from_chain_string(Node* first, Node* opl, String8 string, StringMatchFlags flags)
{
	Node* result = nil_node();
	for (Node* n = first; !node_is_nil(n) && n != opl; n = n->next)
	{
		if (str8_match(n->string, string, flags)) {
			result = n;
			break;
		}
	}
	return result;
}

inline Node*
node_from_chain_index(Node* first, Node* opl, U64 index) {
	Node* result = nil_node();
	S64   idx    = 0;
	for (Node* n = first; !node_is_nil(n) && n != opl; n = n->next, idx += 1)
	{
		if (index == idx) {
			result = n;
			break;
		}
	}
	return result;
}

inline Node*
node_from_chain_flags(Node* first, Node* opl, NodeFlags flags) {
	Node* result = nil_node();
	for (Node* n = first; !node_is_nil(n) && n != opl; n = n->next)
	{
		if (n->flags & flags) {
			result = n;
			break;
		}
	}
	return result;
}

inline U64
index_from_node(Node* node) {
	U64 index = 0;
	for (Node* n = node->prev; !node_is_nil(n); n = n->prev) {
		index += 1;
	}
	return index;
}

inline Node*
root_from_node(Node* node) {
	Node* result = node;
	for (Node* p = node->parent; (p->kind == NodeKind_Main || p->kind == NodeKind_Tag) && !node_is_nil(p); p = p->parent) {
		result = p;
	}
	return result;
}

inline Node* child_from_string(Node* node, String8 child_string, StringMatchFlags flags) { return node_from_chain_string(node->first,     nil_node(), child_string, flags); }
inline Node* tag_from_string  (Node* node, String8 tag_string,   StringMatchFlags flags) { return node_from_chain_string(node->first_tag, nil_node(), tag_string,   flags); }
inline Node* child_from_index (Node* node, U64 index)                                    { return node_from_chain_index (node->first,     nil_node(), index); }
inline Node* tag_from_index   (Node* node, U64 index)                                    { return node_from_chain_index (node->first_tag, nil_node(), index); }

inline Node*
tag_arg_from_index(Node* node, String8 tag_string, StringMatchFlags flags, U64 index) {
	Node* tag = tag_from_string(node, tag_string, flags);
	return child_from_index(tag, index);
}

inline Node*
tag_arg_from_string(Node* node, String8 tag_string, StringMatchFlags tag_str_flags, String8 arg_string, StringMatchFlags arg_str_flags) {
	Node* tag = tag_from_string  (node, tag_string, tag_str_flags);
	Node* arg = child_from_string(tag,  arg_string, arg_str_flags);
	return arg;
}

inline B32 node_has_child(Node* node, String8 string, StringMatchFlags flags) { return !node_is_nil(child_from_string(node, string, flags)); }
inline B32 node_has_tag  (Node* node, String8 string, StringMatchFlags flags) { return !node_is_nil(tag_from_string  (node, string, flags)); }

inline U64
child_count_from_node(Node *node) {
	U64 result = 0;
	for (Node* child = node->first; !node_is_nil(child); child = child->next) {
		result += 1;
	}
	return result;
}

inline U64
tag_count_from_node(Node* node) {
	U64 result = 0;
	for (Node* child = node->first_tag; !node_is_nil(child); child = child->next) {
		result += 1;
	}
	return result;
}

//- rjf: tree comparison

MD_API B32 tree_match(Node* a, Node* b, StringMatchFlags flags);
MD_API B32 node_match(Node* a, Node* b, StringMatchFlags flags);

//- rjf: tree duplication

MD_API Node* tree_copy__arena(Arena*        arena, Node* src_root);
MD_API Node* tree_copy__ainfo(AllocatorInfo ainfo, Node* src_root);

#define tree_copy(allocator, src_root) _Generic(allocator, Arena*: tree_copy__arena, AllocatorInfo: tree_copy__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, src_root)

force_inline Node* tree_copy__arena(Arena* arena, Node* src_root) { return tree_copy__ainfo(arena_allocator(arena), src_root); }

////////////////////////////////
//~ rjf: Text -> Tokens Functions

MD_API TokenizeResult tokenize_from_text__arena(Arena*        arena, String8 text);
MD_API TokenizeResult tokenize_from_text__ainfo(AllocatorInfo ainfo, String8 text);

#define tokenize_from_text(allocator, text) _Generic(allocator, Arena*: tokenize_from_text__arena, AllocatorInfo: tokenize_from_text__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, text)

force_inline TokenizeResult tokenize_from_text__arena(Arena* arena, String8 text) {  return tokenize_from_text__ainfo(arena_allocator(arena), text); }

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

MD_API ParseResult parse_from_text_tokens__arena(Arena*        arena, String8 filename, String8 text, TokenArray tokens);
MD_API ParseResult parse_from_text_tokens__ainfo(AllocatorInfo ainfo, String8 filename, String8 text, TokenArray tokens);

#define parse_from_text_tokens(allocator, filename, text, tokens) _Generic(allocator, Arena*: parse_from_text_tokens__arena, AllocatorInfo: parse_from_text_tokens__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, filename, text, tokens)

force_inline ParseResult parse_from_text_tokens__arena(Arena* arena, String8 filename, String8 text, TokenArray tokens) { return parse_from_text_tokens__ainfo(arena_allocator(arena), filename, text, tokens); }


////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

MD_API ParseResult parse_from_text__arena(Arena*        arena, String8 filename, String8 text);
MD_API ParseResult parse_from_text__ainfo(AllocatorInfo ainfo, String8 filename, String8 text);

#define parse_from_text(allocator, filename, text) _Generic(allocator, Arena*: parse_from_text__arena, AllocatorInfo: parse_from_text__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, filename, text)
#define tree_from_string(allocator, string)       (parse_from_text((allocator), str8_zero(), (string)).root)

force_inline ParseResult parse_from_text__arena(Arena* arena, String8 filename, String8 text) { return parse_from_text__ainfo(arena_allocator(arena), filename, text); }

////////////////////////////////
//~ rjf: Tree -> Text Functions

MD_API String8List debug_string_list_from_tree__arena(Arena*        arena, Node* root);
MD_API String8List debug_string_list_from_tree__ainfo(AllocatorInfo ainfo, Node* root);

#define debug_string_list_from_tree(allocator, string) _Generic(allocator, Arena*: debug_string_list_from_tree__arena, AllocatorInfo: debug_string_list_from_tree__ainfo, default: assert_generic_selection_fail) resolved_function_call(allocator, string)

force_inline String8List debug_string_list_from_tree__arena(Arena* arena, Node* root) { return debug_string_list_from_tree__ainfo(arena_allocator(arena), root); }
