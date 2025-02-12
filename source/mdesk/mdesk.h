#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "os/os.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Messages

typedef enum MD_MsgKind
{
	MD_MsgKind_Null,
	MD_MsgKind_Note,
	MD_MsgKind_Warning,
	MD_MsgKind_Error,
	MD_MsgKind_FatalError,
}
MD_MsgKind;

typedef struct MD_Node MD_Node;
typedef struct MD_Msg  MD_Msg;
struct MD_Msg
{
	MD_Msg*    next;
	MD_Node*   node;
	MD_MsgKind kind;
	MD_String8 string;
};

typedef struct MD_MsgList MD_MsgList;
struct MD_MsgList
{
	MD_Msg*    first;
	MD_Msg*    last;
	MD_U64     count;
	MD_MsgKind worst_message_kind;
};

////////////////////////////////
//~ rjf: MD_Token Types

typedef MD_U32 MD_TokenFlags;
enum
{
	// TODO(Ed): Track type of comment, and opening/closing main delimiter. 
	// (The parser needs that info later and just forces md_str_matches that werent necessary)

	// rjf: base kind info
	MD_TokenFlag_Identifier          = (1 << 0),
	MD_TokenFlag_Numeric             = (1 << 1),
	MD_TokenFlag_StringLiteral       = (1 << 2),
	MD_TokenFlag_Symbol              = (1 << 3),
	MD_TokenFlag_Reserved            = (1 << 4),
	MD_TokenFlag_Comment             = (1 << 5),
	MD_TokenFlag_Whitespace          = (1 << 6),
	MD_TokenFlag_Newline             = (1 << 7),

	// rjf: decoration info
	MD_TokenFlag_StringSingleQuote   = (1 << 8),
	MD_TokenFlag_StringDoubleQuote   = (1 << 9),
	MD_TokenFlag_StringTick          = (1 << 10),
	MD_TokenFlag_StringTriplet       = (1 << 11),
	
	// rjf: error info
	MD_TokenFlag_BrokenComment       = (1 << 12),
	MD_TokenFlag_BrokenStringLiteral = (1 << 13),
	MD_TokenFlag_BadCharacter        = (1 << 14),
};

typedef MD_U32 MD_TokenFlagGroups;
enum
{
	MD_TokenFlagGroup_Comment    = MD_TokenFlag_Comment,
	MD_TokenFlagGroup_Whitespace = (MD_TokenFlag_Whitespace| MD_TokenFlag_Newline),
	MD_TokenFlagGroup_Irregular  = (MD_TokenFlagGroup_Comment  | MD_TokenFlagGroup_Whitespace),
	MD_TokenFlagGroup_Regular    = ~MD_TokenFlagGroup_Irregular,
	MD_TokenFlagGroup_Label      = (MD_TokenFlag_Identifier | MD_TokenFlag_Numeric | MD_TokenFlag_StringLiteral | MD_TokenFlag_Symbol),
	MD_TokenFlagGroup_Error      = (MD_TokenFlag_BrokenComment | MD_TokenFlag_BrokenStringLiteral | MD_TokenFlag_BadCharacter),
};

typedef struct MD_Token MD_Token;
struct MD_Token
{
	MD_Rng1U64    range;
	MD_TokenFlags flags;
};

typedef struct MD_TokenChunkNode MD_TokenChunkNode;
struct MD_TokenChunkNode
{
	MD_TokenChunkNode* next;
	MD_Token*          v;
	MD_U64             count;
	MD_U64             cap;
};

typedef struct MD_TokenChunkList MD_TokenChunkList;
struct MD_TokenChunkList
{
	MD_TokenChunkNode* first;
	MD_TokenChunkNode* last;
	MD_U64 chunk_count;
	MD_U64 total_token_count;
};

typedef struct MD_TokenArray MD_TokenArray;
struct MD_TokenArray
{
	MD_Token* v;
	MD_U64    count;
};

////////////////////////////////
//~ rjf: MD_Node Types

typedef enum MD_NodeKind MD_NodeKind;
enum MD_NodeKind
{
	MD_NodeKind_Nil,
	MD_NodeKind_File,
	MD_NodeKind_ErrorMarker,
	MD_NodeKind_Main,
	MD_NodeKind_Tag,
	MD_NodeKind_List,
	MD_NodeKind_Reference,
	MD_NodeKind_COUNT
};

typedef MD_U32 NodeFlags;
enum
{
	MD_NodeFlag_MaskSetDelimiters          = (0x3F << 0),
	MD_NodeFlag_HasParenLeft               = (1    << 0),
	MD_NodeFlag_HasParenRight              = (1    << 1),
	MD_NodeFlag_HasBracketLeft             = (1    << 2),
	MD_NodeFlag_HasBracketRight            = (1    << 3),
	MD_NodeFlag_HasBraceLeft               = (1    << 4),
	MD_NodeFlag_HasBraceRight              = (1    << 5),
	
	MD_NodeFlag_MaskSeparators             = (0xF << 6),
	MD_NodeFlag_IsBeforeSemicolon          = (1   << 6),
	MD_NodeFlag_IsAfterSemicolon           = (1   << 7),
	MD_NodeFlag_IsBeforeComma              = (1   << 8),
	MD_NodeFlag_IsAfterComma               = (1   << 9),
	
	MD_NodeFlag_MaskStringDelimiters       = (0xF << 10),
	MD_NodeFlag_StringSingleQuote          = (1   << 10),
	MD_NodeFlag_StringDoubleQuote          = (1   << 11),
	MD_NodeFlag_StringTick                 = (1   << 12),
	MD_NodeFlag_StringTriplet              = (1   << 13),
	
	MD_NodeFlag_MaskLabelKind              = (0xF << 14),
	MD_NodeFlag_Numeric                    = (1   << 14),
	MD_NodeFlag_Identifier                 = (1   << 15),
	MD_NodeFlag_StringLiteral              = (1   << 16),
	MD_NodeFlag_Symbol                     = (1   << 17),
};
#define MD_NodeFlag_AfterFromBefore(f) ((f) << 1)

typedef struct MD_Node MD_Node;
struct MD_Node
{
	// rjf: tree links
	MD_Node* next;
	MD_Node* prev;
	MD_Node* parent;
	MD_Node* first;
	MD_Node* last;
	
	// rjf: tag links
	MD_Node* first_tag;
	MD_Node* last_tag;
	
	// rjf: node info
	MD_NodeKind  kind;
	NodeFlags flags;
	MD_String8   string;
	MD_String8   raw_string;
	
	// rjf: source code info
	MD_U64 src_offset;
	
	// rjf: user-controlled generation number
	//
	// (unused by mdesk layer, but can be used by usage code to use MD_Node trees
	// in a "retained mode" way, where stable generational handles can be formed
	// to nodes)
	MD_U64 user_gen;
	
	// rjf: extra padding to 128 bytes
	MD_U64 _unused_[2];
};

typedef struct MD_NodeRec MD_NodeRec;
struct MD_NodeRec
{
	MD_Node* next;
	MD_S32   md_push_count;
	MD_S32   pop_count;
};

////////////////////////////////
//~ rjf: Text -> Tokens Types

typedef struct MD_TokenizeResult MD_TokenizeResult;
struct MD_TokenizeResult
{
  MD_TokenArray tokens;
  MD_MsgList    msgs;
};

////////////////////////////////
//~ rjf: Tokens -> Tree Types

typedef struct MD_ParseResult MD_ParseResult;
struct MD_ParseResult
{
	MD_Node*   root;
	MD_MsgList msgs;
};

////////////////////////////////
// MD_Context

typedef struct MD_Context MD_Context;
struct MD_Context
{
	// Currently, this is only relevant if the user is utilizing this library via bindings
	// or they are not utilizing metadesk's hosted `md_entry_point` runtime

	// Its recommended that the user hooks up 
	// their own arena allocators to the thread context
	// which will be utilized for thread local scratch arena
	MD_TCTX thread_ctx;

	// Just as with MD_TCTX its recommended the user setup the arena allocators
	// for te os context
	MD_OS_Context os_ctx;

	// This skips the md_os_init process but that means the user is expected to setup
	// the thread context's arenas for the process
	MD_B32 dont_init_os;
};

////////////////////////////////
//~ rjf: Globals

inline MD_Node*
md_nil_node()
{
	md_read_only md_local_persist 
	MD_Node nil =
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
// MD_Context lifetime Functios

// NOTE(Ed): The are see comments in MD_Context struct, this is only necessary when not utilizing 
// the metadesk os runtime provided md_entry_point interface

MD_API void md_init(MD_Context* ctx);
MD_API void md_deinit(MD_Context* ctx); // Does nothing for now.

////////////////////////////////
//~ rjf: Message Type Functions

       void md_msg_list_push__arena (MD_Arena*        arena, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, MD_String8 string);
MD_API void md_msg_list_push__ainfo (MD_AllocatorInfo ainfo, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, MD_String8 string);
       void md_msg_list_pushf__arena(MD_Arena*        arena, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, char *fmt, ...);
       void md_msg_list_pushf__ainfo(MD_AllocatorInfo ainfo, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, char *fmt, ...);

#define md_msg_list_push(allocator, msgs, node, kind, string)            _Generic(allocator, MD_Arena*: md_msg_list_push__arena,  MD_AllocatorInfo: md_msg_list_push__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, msgs, node, kind, string)
#define md_msg_list_pushf(allocator, msgs, node, kind, string, fmt, ...) _Generic(allocator, MD_Arena*: md_msg_list_pushf__arena, MD_AllocatorInfo: md_msg_list_pushf__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, msgs, node, kind, fmt, __VA_ARGS__)

MD_API void md_msg_list_concat_in_place(MD_MsgList* dst, MD_MsgList* to_push);

md_force_inline void md_msg_list_push__arena(MD_Arena* arena, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, MD_String8 string) { md_msg_list_push__ainfo(md_arena_allocator(arena), msgs, node, kind, string); }

inline void
md_msg_list_pushf__arena(MD_Arena* arena, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8 string = md_str8fv(arena, fmt, args);
	md_msg_list_push(arena, msgs, node, kind, string);
	va_end(args);
}

inline void
md_msg_list_pushf__ainfo(MD_AllocatorInfo ainfo, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	MD_String8 string = md_str8fv(ainfo, fmt, args);
	md_msg_list_push(ainfo, msgs, node, kind, string);
	va_end(args);
}

////////////////////////////////
//~ rjf: MD_Token Type Functions

MD_Token md_token_make (MD_Rng1U64    range, MD_TokenFlags flags);
MD_B32   md_token_match(MD_Token      a,     MD_Token      b);

MD_API MD_String8 content_string_from_token_flags_str8(MD_TokenFlags flags, MD_String8 string);

       MD_String8List md_string_list_from_token_flags__arena(MD_Arena*        arena, MD_TokenFlags flags);
MD_API MD_String8List md_string_list_from_token_flags__ainfo(MD_AllocatorInfo ainfo, MD_TokenFlags flags);
MD_API void        md_token_chunk_list_push__arena       (MD_Arena*        arena, MD_TokenChunkList* list, MD_U64 cap, MD_Token token);
MD_API void        md_token_chunk_list_push__ainfo       (MD_AllocatorInfo ainfo, MD_TokenChunkList* list, MD_U64 cap, MD_Token token);
MD_API MD_TokenArray  md_token_array_from_chunk_list__arena (MD_Arena*        arena, MD_TokenChunkList* chunks);
MD_API MD_TokenArray  md_token_array_from_chunk_list__ainfo (MD_AllocatorInfo ainfo, MD_TokenChunkList* chunks);

#define md_string_list_from_token_flags(allocator, flags)     _Generic(allocator, MD_Arena*: md_string_list_from_token_flags__arena, MD_AllocatorInfo: md_string_list_from_token_flags__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, flags)
#define md_token_chunk_list_push(allocator, list, cap, token) _Generic(allocator, MD_Arena*: md_token_chunk_list_push__arena,        MD_AllocatorInfo: md_token_chunk_list_push__ainfo,        default: md_assert_generic_sel_fail) md_generic_call(allocator, list, cap, token)
#define md_token_array_from_chunk_list(allocator, chunks)     _Generic(allocator, MD_Arena*: md_token_array_from_chunk_list__arena,  MD_AllocatorInfo: md_token_array_from_chunk_list__ainfo,  default: md_assert_generic_sel_fail) md_generic_call(allocator, chunks)

md_force_inline MD_String8List md_string_list_from_token_flags__arena(MD_Arena* arena, MD_TokenFlags flags)                           { return md_string_list_from_token_flags__ainfo(md_arena_allocator(arena), flags); }
md_force_inline void        md_token_chunk_list_push__arena       (MD_Arena* arena, MD_TokenChunkList* list, MD_U64 cap, MD_Token token) { md_token_chunk_list_push__ainfo(md_arena_allocator(arena), list, cap, token); }
md_force_inline MD_TokenArray  md_token_array_from_chunk_list__arena (MD_Arena* arena, MD_TokenChunkList* chunks)                     { return md_token_array_from_chunk_list__ainfo(md_arena_allocator(arena), chunks); }

inline MD_Token
md_token_make(MD_Rng1U64 range, MD_TokenFlags flags) {
  MD_Token  token = { range, flags };
  return token;
}

inline MD_B32
md_token_match(MD_Token a, MD_Token b) {
	return (a.range.md_min == b.range.md_min &&
			a.range.md_max == b.range.md_max &&
			a.flags     == b.flags       );
}

////////////////////////////////
//~ rjf: MD_Node Type Functions

//- rjf: flag conversions

inline NodeFlags
md_node_flags_from_token_flags(MD_TokenFlags flags)
{
	NodeFlags result = 0;
	result |= MD_NodeFlag_Identifier        *!! (flags & MD_TokenFlag_Identifier       );
	result |= MD_NodeFlag_Numeric           *!! (flags & MD_TokenFlag_Numeric          );
	result |= MD_NodeFlag_StringLiteral     *!! (flags & MD_TokenFlag_StringLiteral    );
	result |= MD_NodeFlag_Symbol            *!! (flags & MD_TokenFlag_Symbol           );
	result |= MD_NodeFlag_StringSingleQuote *!! (flags & MD_TokenFlag_StringSingleQuote);
	result |= MD_NodeFlag_StringDoubleQuote *!! (flags & MD_TokenFlag_StringDoubleQuote);
	result |= MD_NodeFlag_StringTick        *!! (flags & MD_TokenFlag_StringTick       );
	result |= MD_NodeFlag_StringTriplet     *!! (flags & MD_TokenFlag_StringTriplet    );
	return result;
}

//- rjf: nil

MD_B32 md_node_is_nil(MD_Node* node) { return (node == 0 || node == md_nil_node() || node->kind == MD_NodeKind_Nil); }

//- rjf: iteration

#define md_each_node(it, first) (MD_Node* it = first; !md_node_is_nil(it); it = it->next)

MD_API MD_NodeRec md_node_rec_depth_first(MD_Node* node, MD_Node* subtree_root, MD_U64 child_off, MD_U64 sib_off);

#define md_node_rec_depth_first_pre(node, subtree_root)     md_node_rec_depth_first((node), (subtree_root), md_offset_of(MD_Node, first), md_offset_of(MD_Node, next))
#define md_node_rec_depth_first_pre_rev(node, subtree_root) md_node_rec_depth_first((node), (subtree_root), md_offset_of(MD_Node, last),  md_offset_of(MD_Node, prev))

//- rjf: tree building

MD_Node* md_push_node__arena(MD_Arena*        arena, MD_NodeKind kind, NodeFlags flags, MD_String8 string, MD_String8 raw_string, MD_U64 src_offset);
MD_Node* md_push_node__ainfo(MD_AllocatorInfo ainfo, MD_NodeKind kind, NodeFlags flags, MD_String8 string, MD_String8 raw_string, MD_U64 src_offset);

#define md_push_node(allocator, kind, flags, string, raw_string, src_offset) _Generic(allocator, MD_Arena*: md_push_node__arena, MD_AllocatorInfo: md_push_node__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, kind, flags, string, raw_string, src_offset)

void  md_node_insert_tag  (MD_Node* parent, MD_Node* prev_child, MD_Node* node);
void  md_node_insert_child(MD_Node* parent, MD_Node* prev_child, MD_Node* node);
void  md_node_push_child  (MD_Node* parent, MD_Node* node);
void  md_node_push_tag    (MD_Node* parent, MD_Node* node);
void  md_unhook           (MD_Node* node);

inline MD_Node* md_push_node__arena(MD_Arena* arena, MD_NodeKind kind, NodeFlags flags, MD_String8 string, MD_String8 raw_string, MD_U64 src_offset) { md_push_node__ainfo(md_arena_allocator(arena), kind, flags, string, raw_string, src_offset); }

inline MD_Node*
md_push_node__ainfo(MD_AllocatorInfo ainfo, MD_NodeKind kind, NodeFlags flags, MD_String8 string, MD_String8 raw_string, MD_U64 src_offset) {
	MD_Node* node = md_alloc_array(ainfo, MD_Node, 1);
	node->first      = node->last = node->parent = node->next = node->prev = node->first_tag = node->last_tag = md_nil_node();
	node->kind       = kind;
	node->flags      = flags;
	node->string     = string;
	node->raw_string = raw_string;
	node->src_offset = src_offset;
	return node;
}

inline void
md_node_insert_child(MD_Node* parent, MD_Node* prev_child, MD_Node* node) {
	node->parent = parent;
	md_dll_insert_npz(md_nil_node(), parent->first, parent->last, prev_child, node, next, prev);
}

inline void
md_node_insert_tag(MD_Node* parent, MD_Node* prev_child, MD_Node* node) {
	node->kind   = MD_NodeKind_Tag;
	node->parent = parent;
	md_dll_insert_npz(md_nil_node(), parent->first_tag, parent->last_tag, prev_child, node, next, prev);
}

inline void
md_node_push_child(MD_Node* parent, MD_Node* node) {
  node->parent = parent;
  md_dll_push_back_npz(md_nil_node(), parent->first, parent->last, node, next, prev);
}

inline void
md_node_push_tag(MD_Node* parent, MD_Node* node) {
  node->kind   = MD_NodeKind_Tag;
  node->parent = parent;
  md_dll_push_back_npz(md_nil_node(), parent->first_tag, parent->last_tag, node, next, prev);
}

//- rjf: tree introspection

MD_Node* md_node_from_chain_string(MD_Node* first, MD_Node* opl, MD_String8 string, MD_StringMatchFlags flags);
MD_Node* md_node_from_chain_index (MD_Node* first, MD_Node* opl, MD_U64 index);
MD_Node* md_node_from_chain_flags (MD_Node* first, MD_Node* opl, NodeFlags flags);
MD_U64   md_index_from_node       (MD_Node* node);
MD_Node* md_root_from_node        (MD_Node* node);
MD_Node* md_child_from_string     (MD_Node* node, MD_String8 child_string, MD_StringMatchFlags flags);
MD_Node* md_tag_from_string       (MD_Node* node, MD_String8 tag_string, MD_StringMatchFlags flags);
MD_Node* md_child_from_index      (MD_Node* node, MD_U64 index);
MD_Node* md_tag_from_index        (MD_Node* node, MD_U64 index);
MD_Node* md_tag_arg_from_index    (MD_Node* node, MD_String8 tag_string, MD_StringMatchFlags flags, MD_U64 index);
MD_Node* md_tag_arg_from_string   (MD_Node* node, MD_String8 tag_string, MD_StringMatchFlags tag_str_flags, MD_String8 arg_string, MD_StringMatchFlags arg_str_flags);
MD_B32   md_node_has_child        (MD_Node* node, MD_String8 string, MD_StringMatchFlags flags);
MD_B32   md_node_has_tag          (MD_Node* node, MD_String8 string, MD_StringMatchFlags flags);
MD_U64   md_child_count_from_node (MD_Node* node);
MD_U64   md_tag_count_from_node   (MD_Node* node);

MD_API MD_String8 md_string_from_children__arena(MD_Arena*        arena, MD_Node* root);
       MD_String8 md_string_from_children__ainfo(MD_AllocatorInfo ainfo, MD_Node* root);

#define md_string_from_children(allocator, root) _Generic(allocator, MD_Arena*: md_string_from_children__arena, MD_AllocatorInfo: md_string_from_children__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, root)

md_force_inline MD_String8 md_string_from_children__arena(MD_Arena* arena, MD_Node* root) { return md_string_from_children__ainfo(md_arena_allocator(arena), root); }

inline MD_Node*
md_node_from_chain_string(MD_Node* first, MD_Node* opl, MD_String8 string, MD_StringMatchFlags flags)
{
	MD_Node* result = md_nil_node();
	for (MD_Node* n = first; !md_node_is_nil(n) && n != opl; n = n->next)
	{
		if (md_str8_match(n->string, string, flags)) {
			result = n;
			break;
		}
	}
	return result;
}

inline MD_Node*
md_node_from_chain_index(MD_Node* first, MD_Node* opl, MD_U64 index) {
	MD_Node* result = md_nil_node();
	MD_S64   idx    = 0;
	for (MD_Node* n = first; !md_node_is_nil(n) && n != opl; n = n->next, idx += 1)
	{
		if (index == idx) {
			result = n;
			break;
		}
	}
	return result;
}

inline MD_Node*
md_node_from_chain_flags(MD_Node* first, MD_Node* opl, NodeFlags flags) {
	MD_Node* result = md_nil_node();
	for (MD_Node* n = first; !md_node_is_nil(n) && n != opl; n = n->next)
	{
		if (n->flags & flags) {
			result = n;
			break;
		}
	}
	return result;
}

inline MD_U64
md_index_from_node(MD_Node* node) {
	MD_U64 index = 0;
	for (MD_Node* n = node->prev; !md_node_is_nil(n); n = n->prev) {
		index += 1;
	}
	return index;
}

inline MD_Node*
md_root_from_node(MD_Node* node) {
	MD_Node* result = node;
	for (MD_Node* p = node->parent; (p->kind == MD_NodeKind_Main || p->kind == MD_NodeKind_Tag) && !md_node_is_nil(p); p = p->parent) {
		result = p;
	}
	return result;
}

inline MD_Node* md_child_from_string(MD_Node* node, MD_String8 child_string, MD_StringMatchFlags flags) { return md_node_from_chain_string(node->first,     md_nil_node(), child_string, flags); }
inline MD_Node* md_tag_from_string  (MD_Node* node, MD_String8 tag_string,   MD_StringMatchFlags flags) { return md_node_from_chain_string(node->first_tag, md_nil_node(), tag_string,   flags); }
inline MD_Node* md_child_from_index (MD_Node* node, MD_U64 index)                                    { return md_node_from_chain_index (node->first,     md_nil_node(), index); }
inline MD_Node* md_tag_from_index   (MD_Node* node, MD_U64 index)                                    { return md_node_from_chain_index (node->first_tag, md_nil_node(), index); }

inline MD_Node*
md_tag_arg_from_index(MD_Node* node, MD_String8 tag_string, MD_StringMatchFlags flags, MD_U64 index) {
	MD_Node* tag = md_tag_from_string(node, tag_string, flags);
	return md_child_from_index(tag, index);
}

inline MD_Node*
md_tag_arg_from_string(MD_Node* node, MD_String8 tag_string, MD_StringMatchFlags tag_str_flags, MD_String8 arg_string, MD_StringMatchFlags arg_str_flags) {
	MD_Node* tag = md_tag_from_string  (node, tag_string, tag_str_flags);
	MD_Node* arg = md_child_from_string(tag,  arg_string, arg_str_flags);
	return arg;
}

inline MD_B32 md_node_has_child(MD_Node* node, MD_String8 string, MD_StringMatchFlags flags) { return !md_node_is_nil(md_child_from_string(node, string, flags)); }
inline MD_B32 md_node_has_tag  (MD_Node* node, MD_String8 string, MD_StringMatchFlags flags) { return !md_node_is_nil(md_tag_from_string  (node, string, flags)); }

inline MD_U64
md_child_count_from_node(MD_Node *node) {
	MD_U64 result = 0;
	for (MD_Node* child = node->first; !md_node_is_nil(child); child = child->next) {
		result += 1;
	}
	return result;
}

inline MD_U64
md_tag_count_from_node(MD_Node* node) {
	MD_U64 result = 0;
	for (MD_Node* child = node->first_tag; !md_node_is_nil(child); child = child->next) {
		result += 1;
	}
	return result;
}

//- rjf: tree comparison

MD_API MD_B32 tree_match(MD_Node* a, MD_Node* b, MD_StringMatchFlags flags);
MD_API MD_B32 md_node_match(MD_Node* a, MD_Node* b, MD_StringMatchFlags flags);

//- rjf: tree duplication

MD_API MD_Node* md_tree_copy__arena(MD_Arena*        arena, MD_Node* src_root);
MD_API MD_Node* md_tree_copy__ainfo(MD_AllocatorInfo ainfo, MD_Node* src_root);

#define md_treecopy(allocator, src_root) _Generic(allocator, MD_Arena*: md_tree_copy__arena, MD_AllocatorInfo: md_tree_copy__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, src_root)

md_force_inline MD_Node* md_tree_copy__arena(MD_Arena* arena, MD_Node* src_root) { return md_tree_copy__ainfo(md_arena_allocator(arena), src_root); }

////////////////////////////////
//~ rjf: Text -> Tokens Functions

MD_API MD_TokenizeResult md_tokenize_from_text__arena(MD_Arena*        arena, MD_String8 text);
MD_API MD_TokenizeResult md_tokenize_from_text__ainfo(MD_AllocatorInfo ainfo, MD_String8 text);

#define md_tokenize_from_text(allocator, text) _Generic(allocator, MD_Arena*: md_tokenize_from_text__arena, MD_AllocatorInfo: md_tokenize_from_text__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, text)

md_force_inline MD_TokenizeResult md_tokenize_from_text__arena(MD_Arena* arena, MD_String8 text) {  return md_tokenize_from_text__ainfo(md_arena_allocator(arena), text); }

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

MD_API MD_ParseResult md_parse_from_text_tokens__arena(MD_Arena*        arena, MD_String8 filename, MD_String8 text, MD_TokenArray tokens);
MD_API MD_ParseResult md_parse_from_text_tokens__ainfo(MD_AllocatorInfo ainfo, MD_String8 filename, MD_String8 text, MD_TokenArray tokens);

#define md_parse_from_text_tokens(allocator, filename, text, tokens) _Generic(allocator, MD_Arena*: md_parse_from_text_tokens__arena, MD_AllocatorInfo: md_parse_from_text_tokens__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, filename, text, tokens)

md_force_inline MD_ParseResult md_parse_from_text_tokens__arena(MD_Arena* arena, MD_String8 filename, MD_String8 text, MD_TokenArray tokens) { return md_parse_from_text_tokens__ainfo(md_arena_allocator(arena), filename, text, tokens); }


////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

MD_API MD_ParseResult md_parse_from_text__arena(MD_Arena*        arena, MD_String8 filename, MD_String8 text);
MD_API MD_ParseResult md_parse_from_text__ainfo(MD_AllocatorInfo ainfo, MD_String8 filename, MD_String8 text);

#define md_parse_from_text(allocator, filename, text) _Generic(allocator, MD_Arena*: md_parse_from_text__arena, MD_AllocatorInfo: md_parse_from_text__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, filename, text)
#define tree_from_string(allocator, string)       (md_parse_from_text((allocator), md_str8_zero(), (string)).root)

md_force_inline MD_ParseResult md_parse_from_text__arena(MD_Arena* arena, MD_String8 filename, MD_String8 text) { return md_parse_from_text__ainfo(md_arena_allocator(arena), filename, text); }

////////////////////////////////
//~ rjf: Tree -> Text Functions

MD_API MD_String8List md_debug_string_list_from_tree__arena(MD_Arena*        arena, MD_Node* root);
MD_API MD_String8List md_debug_string_list_from_tree__ainfo(MD_AllocatorInfo ainfo, MD_Node* root);

#define md_debug_string_list_from_tree(allocator, string) _Generic(allocator, MD_Arena*: md_debug_string_list_from_tree__arena, MD_AllocatorInfo: md_debug_string_list_from_tree__ainfo, default: md_assert_generic_sel_fail) md_generic_call(allocator, string)

md_force_inline MD_String8List md_debug_string_list_from_tree__arena(MD_Arena* arena, MD_Node* root) { return md_debug_string_list_from_tree__ainfo(md_arena_allocator(arena), root); }
