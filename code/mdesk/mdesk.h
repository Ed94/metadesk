#if INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base/base.h"
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
	// rjf: base kind info
	TokenFlag_Identifier          = (1<<0),
	TokenFlag_Numeric             = (1<<1),
	TokenFlag_StringLiteral       = (1<<2),
	TokenFlag_Symbol              = (1<<3),
	TokenFlag_Reserved            = (1<<4),
	TokenFlag_Comment             = (1<<5),
	TokenFlag_Whitespace          = (1<<6),
	TokenFlag_Newline             = (1<<7),
	
	// rjf: decoration info
	TokenFlag_StringSingleQuote   = (1<<8),
	TokenFlag_StringDoubleQuote   = (1<<9),
	TokenFlag_StringTick          = (1<<10),
	TokenFlag_StringTriplet       = (1<<11),
	
	// rjf: error info
	TokenFlag_BrokenComment       = (1<<12),
	TokenFlag_BrokenStringLiteral = (1<<13),
	TokenFlag_BadCharacter        = (1<<14),
};

typedef U32 TokenGroups;
enum
{
	TokenGroup_Comment    = TokenFlag_Comment,
	TokenGroup_Whitespace = (TokenFlag_Whitespace| TokenFlag_Newline),
	TokenGroup_Irregular  = (TokenGroup_Comment  | TokenGroup_Whitespace),
	TokenGroup_Regular    = ~TokenGroup_Irregular,
	TokenGroup_Label      = (TokenFlag_Identifier | TokenFlag_Numeric | TokenFlag_StringLiteral | TokenFlag_Symbol),
	TokenGroup_Error      = (TokenFlag_BrokenComment | TokenFlag_BrokenStringLiteral | TokenFlag_BadCharacter),
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

typedef enum NodeKind
{
	NodeKind_Nil,
	NodeKind_File,
	NodeKind_ErrorMarker,
	NodeKind_Main,
	NodeKind_Tag,
	NodeKind_List,
	NodeKind_Reference,
	NodeKind_COUNT
}
NodeKind;

typedef U32 NodeFlags;
enum
{
	NodeFlag_MaskSetDelimiters          = (0x3F<<0),
	NodeFlag_HasParenLeft               = (1<<0),
	NodeFlag_HasParenRight              = (1<<1),
	NodeFlag_HasBracketLeft             = (1<<2),
	NodeFlag_HasBracketRight            = (1<<3),
	NodeFlag_HasBraceLeft               = (1<<4),
	NodeFlag_HasBraceRight              = (1<<5),
	
	NodeFlag_MaskSeparators             = (0xF<<6),
	NodeFlag_IsBeforeSemicolon          = (1<<6),
	NodeFlag_IsAfterSemicolon           = (1<<7),
	NodeFlag_IsBeforeComma              = (1<<8),
	NodeFlag_IsAfterComma               = (1<<9),
	
	NodeFlag_MaskStringDelimiters       = (0xF<<10),
	NodeFlag_StringSingleQuote          = (1<<10),
	NodeFlag_StringDoubleQuote          = (1<<11),
	NodeFlag_StringTick                 = (1<<12),
	NodeFlag_StringTriplet              = (1<<13),
	
	NodeFlag_MaskLabelKind              = (0xF<<14),
	NodeFlag_Numeric                    = (1<<14),
	NodeFlag_Identifier                 = (1<<15),
	NodeFlag_StringLiteral              = (1<<16),
	NodeFlag_Symbol                     = (1<<17),
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
//~ rjf: Globals

inline Node*
nil_node()
{
	read_only thread_local local_persist
	nil = {
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
//~ rjf: Message Type Functions

MD_API void msg_list_push (Arena* arena, MsgList* msgs, Node* node, MsgKind kind, String8 string);
       void msg_list_pushf(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, char *fmt, ...);

MD_API void msg_list_concat_in_place(MsgList* dst, MsgList* to_push);

inline void
msg_list_pushf(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	String8 string = push_str8fv(arena, fmt, args);
	msg_list_push(arena, msgs, node, kind, string);
	va_end(args);
}

////////////////////////////////
//~ rjf: Token Type Functions

Token token_make (Rng1U64    range, TokenFlags flags);
B32   token_match(Token      a,     Token      b);

MD_API String8 content_string_from_token_flags_str8(TokenFlags flags, String8 string);

MD_API String8List string_list_from_token_flags(Arena* arena, TokenFlags flags);
MD_API void        token_chunk_list_push       (Arena* arena, TokenChunkList* list, U64 cap, Token token);
MD_API TokenArray  token_array_from_chunk_list (Arena* arena, TokenChunkList* chunks);

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
	result |= NodeFlag_Identifier        *!!(flags & TokenFlag_Identifier       );
	result |= NodeFlag_Numeric           *!!(flags & TokenFlag_Numeric          );
	result |= NodeFlag_StringLiteral     *!!(flags & TokenFlag_StringLiteral    );
	result |= NodeFlag_Symbol            *!!(flags & TokenFlag_Symbol           );
	result |= NodeFlag_StringSingleQuote *!!(flags & TokenFlag_StringSingleQuote);
	result |= NodeFlag_StringDoubleQuote *!!(flags & TokenFlag_StringDoubleQuote);
	result |= NodeFlag_StringTick        *!!(flags & TokenFlag_StringTick       );
	result |= NodeFlag_StringTriplet     *!!(flags & TokenFlag_StringTriplet    );
	return result;
}

//- rjf: nil

B32 node_is_nil(Node* node) { return (node == 0 || node == nil_node() || node->kind == NodeKind_Nil); }

//- rjf: iteration

#define each_node(it, first) (Node *it = first; !node_is_nil(it); it = it->next)

MD_API NodeRec node_rec_depth_first(Node *node, Node *subtree_root, U64 child_off, U64 sib_off);

#define node_rec_depth_first_pre(node, subtree_root)     node_rec_depth_first((node), (subtree_root), offset_of(Node, first), offset_of(Node, next))
#define node_rec_depth_first_pre_rev(node, subtree_root) node_rec_depth_first((node), (subtree_root), offset_of(Node, last),  offset_of(Node, prev))

//- rjf: tree building

Node* push_node        (Arena *arena, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset);
void  node_insert_tag  (Node *parent, Node *prev_child, Node *node);
void  node_insert_child(Node *parent, Node *prev_child, Node *node);
void  node_push_child  (Node *parent, Node *node);
void  node_push_tag    (Node *parent, Node *node);
void  unhook           (Node *node);

inline Node*
push_node(Arena* arena, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset) {
	Node* node = push_array(arena, Node, 1);
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

//- rjf: tree introspectionhttps://github.com/Ed94/metadesk

internal Node*   node_from_chain_string(Node *first, Node *opl, String8 string, StringMatchFlags flags);
internal Node*   node_from_chain_index (Node *first, Node *opl, U64 index);
internal Node*   node_from_chain_flags (Node *first, Node *opl, NodeFlags flags);
internal U64     index_from_node       (Node *node);
internal Node*   root_from_node        (Node *node);
internal Node*   child_from_string     (Node *node, String8 child_string, StringMatchFlags flags);
internal Node*   tag_from_string       (Node *node, String8 tag_string, StringMatchFlags flags);
internal Node*   child_from_index      (Node *node, U64 index);
internal Node*   tag_from_index        (Node *node, U64 index);
internal Node*   tag_arg_from_index    (Node *node, String8 tag_string, StringMatchFlags flags, U64 index);
internal Node*   tag_arg_from_string   (Node *node, String8 tag_string, StringMatchFlags tag_str_flags, String8 arg_string, StringMatchFlags arg_str_flags);
internal B32     node_has_child        (Node *node, String8 string, StringMatchFlags flags);
internal B32     node_has_tag          (Node *node, String8 string, StringMatchFlags flags);
internal U64     child_count_from_node (Node *node);
internal U64     tag_count_from_node   (Node *node);

internal String8 string_from_children (Arena *arena, Node *root);

//- rjf: tree comparison

internal B32 tree_match(Node *a, Node *b, StringMatchFlags flags);
internal B32 node_match(Node *a, Node *b, StringMatchFlags flags);

//- rjf: tree duplication

internal Node* tree_copy(Arena *arena, Node *src_root);
// Node* tree_copy_arena(Node* src_root, Arena* arena);
// Node* tree_copy_ainfo(Node* src_root, AllocatorInfo info);

////////////////////////////////
//~ rjf: Text -> Tokens Functions

internal TokenizeResult tokenize_from_text(Arena *arena, String8 text);

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

internal ParseResult parse_from_text_tokens(Arena *arena, String8 filename, String8 text, TokenArray tokens);

////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

internal ParseResult parse_from_text(Arena *arena, String8 filename, String8 text);
#define tree_from_string(arena, string) (parse_from_text((arena), str8_zero(), (string)).root)

////////////////////////////////
//~ rjf: Tree -> Text Functions

internal String8List debug_string_list_from_tree(Arena *arena, Node *root);
