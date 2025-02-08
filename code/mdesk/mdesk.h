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
typedef struct Msg Msg;
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

internal void md_msg_list_push (Arena* arena, MsgList* msgs, Node* node, MsgKind kind, String8 string);
internal void md_msg_list_pushf(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, char *fmt, ...);

internal void md_msg_list_concat_in_place(MsgList* dst, MsgList* to_push);

////////////////////////////////
//~ rjf: Token Type Functions

internal Token       md_token_make                          (Rng1U64 range, TokenFlags flags);
internal B32         md_token_match                         (Token a, Token b);
internal String8List md_string_list_from_token_flags        (Arena *arena, TokenFlags flags);
internal void        md_token_chunk_list_push               (Arena *arena, TokenChunkList *list, U64 cap, Token token);
internal TokenArray  md_token_array_from_chunk_list         (Arena *arena, TokenChunkList *chunks);
internal String8     md_content_string_from_token_flags_str8(TokenFlags flags, String8 string);

////////////////////////////////
//~ rjf: Node Type Functions

//- rjf: flag conversions
internal NodeFlags md_node_flags_from_token_flags(TokenFlags flags);

//- rjf: nil
internal B32 md_node_is_nil(Node *node);

//- rjf: iteration
#define MD_EachNode(it, first) (Node *it = first; !md_node_is_nil(it); it = it->next)

internal NodeRec node_rec_depth_first(Node *node, Node *subtree_root, U64 child_off, U64 sib_off);

#define node_rec_depth_first_pre(node, subtree_root)     node_rec_depth_first((node), (subtree_root), offset_of(Node, first), offset_of(Node, next))
#define node_rec_depth_first_pre_rev(node, subtree_root) node_rec_depth_first((node), (subtree_root), offset_of(Node, last),  offset_of(Node, prev))

//- rjf: tree building
internal Node* md_push_node        (Arena *arena, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset);
internal void  md_node_insert_tag  (Node *parent, Node *prev_child, Node *node);
internal void  md_node_insert_child(Node *parent, Node *prev_child, Node *node);
internal void  md_node_push_child  (Node *parent, Node *node);
internal void  md_node_push_tag    (Node *parent, Node *node);
internal void  md_unhook           (Node *node);

//- rjf: tree introspection
internal Node*   md_node_from_chain_string(Node *first, Node *opl, String8 string, StringMatchFlags flags);
internal Node*   md_node_from_chain_index (Node *first, Node *opl, U64 index);
internal Node*   md_node_from_chain_flags (Node *first, Node *opl, NodeFlags flags);
internal U64     md_index_from_node       (Node *node);
internal Node*   md_root_from_node        (Node *node);
internal Node*   md_child_from_string     (Node *node, String8 child_string, StringMatchFlags flags);
internal Node*   md_tag_from_string       (Node *node, String8 tag_string, StringMatchFlags flags);
internal Node*   md_child_from_index      (Node *node, U64 index);
internal Node*   md_tag_from_index        (Node *node, U64 index);
internal Node*   md_tag_arg_from_index    (Node *node, String8 tag_string, StringMatchFlags flags, U64 index);
internal Node*   md_tag_arg_from_string   (Node *node, String8 tag_string, StringMatchFlags tag_str_flags, String8 arg_string, StringMatchFlags arg_str_flags);
internal B32     md_node_has_child        (Node *node, String8 string, StringMatchFlags flags);
internal B32     md_node_has_tag          (Node *node, String8 string, StringMatchFlags flags);
internal U64     md_child_count_from_node (Node *node);
internal U64     md_tag_count_from_node   (Node *node);

internal String8 md_string_from_children (Arena *arena, Node *root);

//- rjf: tree comparison
internal B32 md_tree_match(Node *a, Node *b, StringMatchFlags flags);
internal B32 md_node_match(Node *a, Node *b, StringMatchFlags flags);

//- rjf: tree duplication
internal Node* md_tree_copy(Arena *arena, Node *src_root);
// Node* tree_copy_arena(Node* src_root, Arena* arena);
// Node* tree_copy_ainfo(Node* src_root, AllocatorInfo info);

////////////////////////////////
//~ rjf: Text -> Tokens Functions

internal TokenizeResult md_tokenize_from_text(Arena *arena, String8 text);

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

internal ParseResult md_parse_from_text_tokens(Arena *arena, String8 filename, String8 text, TokenArray tokens);

////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

internal ParseResult md_parse_from_text(Arena *arena, String8 filename, String8 text);
#define md_tree_from_string(arena, string) (md_parse_from_text((arena), str8_zero(), (string)).root)

////////////////////////////////
//~ rjf: Tree -> Text Functions

internal String8List md_debug_string_list_from_tree(Arena *arena, Node *root);
