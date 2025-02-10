#ifdef INTELLISENSE_DIRECTIVES
#	include "mdesk.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// Context

void init(Context* ctx)
{
	if ( ! ctx->dont_init_os) {
		os_init(& ctx->os_ctx, & ctx->thread_ctx);
	}

	
}

// void deinit(Context* ctx)
// {
// }

////////////////////////////////
//~ rjf: Message Type Functions

void
msg_list_push(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, String8 string)
{
#if MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL
	Msg*
	msg = push_array(arena, Msg, 1);
	msg->node   = node;
	msg->kind   = kind;
	msg->string = string;

	sll_queue_push(msgs->first, msgs->last, msg);

	msgs->count             += 1;
	msgs->worst_message_kind = md_max(kind, msgs->worst_message_kind);
#else
	msg_list_alloc(arena_allocator(arena), msgs, node, kind, string);
#endif
}

void
msg_list_alloc(AllocatorInfo ainfo, MsgList* msgs, Node* node, MsgKind kind, String8 string)
{
	Msg*
	msg = alloc_array(ainfo, Msg, 1);
	msg->node   = node;
	msg->kind   = kind;
	msg->string = string;

	sll_queue_push(msgs->first, msgs->last, msg);

	msgs->count             += 1;
	msgs->worst_message_kind = md_max(kind, msgs->worst_message_kind);
}

void
msg_list_concat_in_place(MsgList* dst, MsgList* to_push)
{
	if (to_push->first != 0)
	{
		if (dst->last) {
			dst->last->next         = to_push->first;
			dst->last               = to_push->last;
			dst->count             += to_push->count;
			dst->worst_message_kind = md_max(dst->worst_message_kind, to_push->worst_message_kind);
		}
		else {
			memory_copy_struct(dst, to_push);
		}
	}
	memory_zero_struct(to_push);
}

////////////////////////////////
//~ rjf: Token Type Functions

String8
content_string_from_token_flags_str8(TokenFlags flags, String8 string)
{
	U64 num_chop = 0;
	U64 num_skip = 0;
	{
		num_skip += 3 * !!(flags & TokenFlag_StringTriplet);
		num_chop += 3 * !!(flags & TokenFlag_StringTriplet);
		num_skip += 1 * (!(flags & TokenFlag_StringTriplet) && flags & TokenFlag_StringLiteral);
		num_chop += 1 * (!(flags & TokenFlag_StringTriplet) && flags & TokenFlag_StringLiteral);
	}
	String8 result = string;
	result = str8_chop(result, num_chop);
	result = str8_skip(result, num_skip);
	return result;
}

String8List
string_list_from_token_flags(Arena* arena, TokenFlags flags)
{
	return string_list_from_token_flags_alloc(arena_allocator(arena), flags);
}

String8List
string_list_from_token_flags_alloc(AllocatorInfo ainfo, TokenFlags flags)
{
	String8List strs = {0};
	if (flags & TokenFlag_Identifier          ){ str8_list_alloc(ainfo, &strs, str8_lit("Identifier"         )); }
	if (flags & TokenFlag_Numeric             ){ str8_list_alloc(ainfo, &strs, str8_lit("Numeric"            )); }
	if (flags & TokenFlag_StringLiteral       ){ str8_list_alloc(ainfo, &strs, str8_lit("StringLiteral"      )); }
	if (flags & TokenFlag_Symbol              ){ str8_list_alloc(ainfo, &strs, str8_lit("Symbol"             )); }
	if (flags & TokenFlag_Reserved            ){ str8_list_alloc(ainfo, &strs, str8_lit("Reserved"           )); }
	if (flags & TokenFlag_Comment             ){ str8_list_alloc(ainfo, &strs, str8_lit("Comment"            )); }
	if (flags & TokenFlag_Whitespace          ){ str8_list_alloc(ainfo, &strs, str8_lit("Whitespace"         )); }
	if (flags & TokenFlag_Newline             ){ str8_list_alloc(ainfo, &strs, str8_lit("Newline"            )); }
	if (flags & TokenFlag_BrokenComment       ){ str8_list_alloc(ainfo, &strs, str8_lit("BrokenComment"      )); }
	if (flags & TokenFlag_BrokenStringLiteral ){ str8_list_alloc(ainfo, &strs, str8_lit("BrokenStringLiteral")); }
	if (flags & TokenFlag_BadCharacter        ){ str8_list_alloc(ainfo, &strs, str8_lit("BadCharacter"       )); }
	return strs;
}

void
token_chunk_list_push(Arena* arena, TokenChunkList* list, U64 cap, Token token)
{
	token_chunk_list_alloc(arena_allocator(arena), list, cap, token);
}

void
token_chunk_list_alloc(AllocatorInfo ainfo, TokenChunkList* list, U64 cap, Token token)
{
	TokenChunkNode* node = list->last;
	if (node == 0 || node->count >= node->cap) {
		node      = alloc_array(ainfo, TokenChunkNode, 1);
		node->cap = cap;
		node->v   = alloc_array_no_zero(ainfo, Token, cap);
		sll_queue_push(list->first, list->last, node);
		list->chunk_count += 1;
	}
	memory_copy_struct(&node->v[node->count], &token);
	node->count             += 1;
	list->total_token_count += 1;
}

TokenArray
token_array_from_chunk_list_push(Arena* arena, TokenChunkList* chunks)
{
	return token_array_from_chunk_list_alloc(arena_allocator(arena), chunks);
}

TokenArray
token_array_from_chunk_list_alloc(AllocatorInfo ainfo, TokenChunkList* chunks)
{
	TokenArray result = {0};
	result.count = chunks->total_token_count;
	result.v     = alloc_array_no_zero(ainfo, Token, result.count);
	U64 write_idx = 0;
	for(TokenChunkNode *n = chunks->first; n != 0; n = n->next)
	{
		memory_copy(result.v + write_idx, n->v, size_of(Token) * n->count);
		write_idx += n->count;
	}
	return result;
}

////////////////////////////////
//~ rjf: Node Type Functions

//- rjf: iteration

NodeRec
node_rec_depth_first(Node* node, Node* subtree_root, U64 child_off, U64 sib_off)
{
	NodeRec 
	rec      = {0};
	rec.next = nil_node();
	if ( ! node_is_nil(*member_from_offset(Node**, node, child_off))) {
		rec.next       = *member_from_offset(Node**, node, child_off);
		rec.push_count = 1;
	}
	else for (Node* p = node; !node_is_nil(p) && p != subtree_root; p = p->parent, rec.pop_count += 1)
	{
		if ( ! node_is_nil(*member_from_offset(Node**, p, sib_off))) {
			rec.next = *member_from_offset(Node**, p, sib_off);
			break;
		}
	}
	return rec;
}

//- rjf: tree building

void
unhook(Node* node) {
	Node* parent = node->parent;
	if ( ! node_is_nil(parent))
	{
		if (node->kind == NodeKind_Tag) dll_remove_npz(nil_node(), parent->first_tag, parent->last_tag, node, next, prev);
		else                            dll_remove_npz(nil_node(), parent->first,     parent->last,     node, next, prev);
		node->parent = nil_node();
	}
}

//- rjf: tree introspection

String8
string_from_children(Arena* arena, Node*  root)
{
	String8 result;
	TempArena scratch = scratch_begin(&arena, 1);
	{
		String8List strs = {0};
		for each_node(child, root->first)
		{
			if (child->flags == child->prev->flags) {
				str8_list_push(scratch.arena, &strs, str8_lit(" "));
			}
			str8_list_push(scratch.arena, &strs, child->string);
		
		}
		result = str8_list_join(arena, &strs, 0);
	}
	scratch_end(scratch);
	return result;
}

//- rjf: tree comparison

B32
node_match(Node* a, Node* b, StringMatchFlags flags)
{
	B32 result = 0;
	if (a->kind == b->kind && str8_match(a->string, b->string, flags))
	{
		result = 1;
		if (result) {
			result = result && a->flags == b->flags;
		}
		if (result && a->kind != NodeKind_Tag)
		{
			for (Node* a_tag = a->first_tag, *b_tag = b->first_tag; !node_is_nil(a_tag) || !node_is_nil(b_tag); a_tag = a_tag->next, b_tag = b_tag->next)
			{
				if (node_match(a_tag, b_tag, flags))
				{
					for (Node* a_tag_arg = a_tag->first, *b_tag_arg = b_tag->first; !node_is_nil(a_tag_arg) || !node_is_nil(b_tag_arg); a_tag_arg = a_tag_arg->next, b_tag_arg = b_tag_arg->next)
					{
						if (!tree_match(a_tag_arg, b_tag_arg, flags)) {
							result = 0;
							goto end;
						}
					}
				}
				else
				{
					result = 0;
					goto end;
				}
			}
		}
	}
	end:;
	return result;
}

B32
tree_match(Node* a, Node* b, StringMatchFlags flags)
{
	B32 result = node_match(a, b, flags);
	if (result)
	{
		for(Node *a_child = a->first, *b_child = b->first; !node_is_nil(a_child) || !node_is_nil(b_child); a_child = a_child->next, b_child = b_child->next)
		{
			if ( ! tree_match(a_child, b_child, flags)) {
				result = 0;
				goto end;
			}
		}
	}
	end:;
	return result;
}

//- rjf: tree duplication

Node*
tree_copy(Arena* arena, Node* src_root)
{
	return tree_copy_alloc(arena_allocator(arena), src_root);
}

Node*
tree_copy_alloc(AllocatorInfo ainfo, Node* src_root)
{
	Node* dst_root   = nil_node();
	Node* dst_parent = dst_root;
	{
		NodeRec rec = {0};
		for(Node* src = src_root; !node_is_nil(src); src = rec.next)
		{
			Node* dst = alloc_array(ainfo, Node, 1);
			dst->first      = dst->last = dst->parent = dst->next = dst->prev = nil_node();
			dst->first_tag  = dst->last_tag = nil_node();
			dst->kind       = src->kind;
			dst->flags      = src->flags;
			dst->string     = str8_copy(ainfo, src->string);
			dst->raw_string = str8_copy(ainfo, src->raw_string);
			dst->src_offset = src->src_offset;
			dst->parent     = dst_parent;
			if (dst_parent != nil_node()) {
				dll_push_back_npz(nil_node(), dst_parent->first, dst_parent->last, dst, next, prev);
			}
			else {
				dst_root = dst_parent = dst;
			}

			rec = node_rec_depth_first_pre(src, src_root);
			if (rec.push_count != 0) {
				dst_parent = dst;
			}
			else for (U64 idx = 0; idx < rec.pop_count; idx += 1) {
				dst_parent = dst_parent->parent;
			}
		}
	}
	return dst_root;
}

////////////////////////////////
//~ rjf: Text -> Tokens Functions

TokenizeResult
tokenize_from_text(Arena* arena, String8 text) { 
	return tokenize_from_text_alloc(arena_allocator(arena), text);
}

TokenizeResult
tokenize_from_text_alloc(AllocatorInfo ainfo, String8 text)
{
	TempArena scratch = scratch_begin(ainfo);

	TokenChunkList tokens = {0};
	MsgList        msgs = {0};

	U8* byte_first = text.str;
	U8* byte_opl   = byte_first + text.size; // one-past-last
	U8* byte       = byte_first;

	//- rjf: scan string & produce tokens
	for (;byte < byte_opl;)
	{
		TokenFlags token_flags = 0;
		U8*        token_start = 0;
		U8*        token_opl   = 0;

		#define is_whitespace(byte) (*byte == ' ' || *byte == '\t' || *byte == '\v' || *byte == '\r')
		
		//- rjf: whitespace
		if (token_flags == 0 && is_whitespace(byte))
		{
			token_flags = TokenFlag_Whitespace;
			token_start = byte;
			token_opl   = byte;

			byte += 1;
			for (;byte <= byte_opl; byte += 1)
			{
				token_opl += 1;
				if (byte == byte_opl || !is_whitespace(byte)) {
					break;
				}
			}
		}
		#undef is_whitspace
		
		//- rjf: newlines
		if (token_flags == 0 && *byte == '\n')
		{
			token_flags = TokenFlag_Newline;
			token_start = byte;
			token_opl   = byte+1;

			byte += 1;
		}
		
		//- rjf: single-line comments
		if (token_flags == 0 && (byte + 1 < byte_opl && *byte == '/' && byte[1] == '/'))
		{
			token_flags = TokenFlag_Comment;
			token_start = byte;
			token_opl   = byte+2;

			byte += 2;

			B32 escaped = 0;
			for (;byte <= byte_opl; byte += 1)
			{
				token_opl += 1;
				if (byte == byte_opl) {
					break;
				}
				if (escaped) {
					escaped = 0;
				}
				else
				{
					if (*byte == '\n') {
						break;
					}
					else if (*byte == '\\') {
						escaped = 1;
					}
				}
			}
		}
		
		//- rjf: multi-line comments
		if (token_flags == 0 && (byte + 1 < byte_opl && *byte == '/' && byte[1] == '*'))
		{
			token_flags = TokenFlag_Comment;
			token_start = byte;
			token_opl   = byte + 2;

			byte += 2;
			for (;byte <= byte_opl; byte += 1)
			{
				token_opl += 1;
				if (byte == byte_opl) {
					token_flags |= TokenFlag_BrokenComment;
					break;
				}
				if (byte + 1 < byte_opl && byte[0] == '*' && byte[1] == '/') {
					token_opl += 2;
					break;
				}
			}
		}

		#define is_identifier(byte) (         \
			('A' <= *byte && *byte <= 'Z') || \
			('a' <= *byte && *byte <= 'z') || \
			*byte == '_'                   || \
			utf8_class(*byte >> 3) >= 2       \
		)
		#if 0
		(
			!('A' <= *byte && *byte <= 'Z') && 
			!('a' <= *byte && *byte <= 'z') && 
			!('0' <= *byte && *byte <= '9') && 
			*byte != '_' && 
			utf8_class(*byte>>3) < 2
		)
		#endif
		
		//- rjf: identifiers
		if (token_flags == 0 && is_identifier(byte))
		{
			token_flags = TokenFlag_Identifier;
			token_start = byte;
			token_opl   = byte;

			byte += 1;
			for(;byte <= byte_opl; byte += 1)
			{
				token_opl += 1;
				if (byte == byte_opl || !is_identifier(byte)) {
					break;
				}
			}
		}
		#undef is_identifier

		#define is_numeric(byte) (                                                       \
			('0'  <= *byte && *byte <= '9')                                           || \
			(*byte == '.' && byte + 1 < byte_opl && '0' <= byte[1] && byte[1] <= '9') || \
			(*byte == '-' && byte + 1 < byte_opl && '0' <= byte[1] && byte[1] <= '9') || \
		     *byte == '_'                                                                \
		)
		#define is_not_numeric(byte) (         \
			!('A' <= *byte && *byte <= 'Z') && \
			!('a' <= *byte && *byte <= 'z') && \
			!('0' <= *byte && *byte <= '9') && \
			*byte != '_' && *byte != '.'       \
		)
		
		//- rjf: numerics
		if (token_flags == 0 && is_numeric(byte)) 
		{
			token_flags = TokenFlag_Numeric;
			token_start = byte;
			token_opl   = byte;

			byte += 1;
			for (;byte <= byte_opl; byte += 1)
			{
				token_opl += 1;
				if (byte == byte_opl || is_not_numeric(byte)) {
					break;
				}
			}
		}
		#undef is_numeric
		#undef is_not_numeric

		#define is_triple_string_literal(byte) (                     \
			(byte[0] == '"' && byte[1] == '"' && byte[2] == '"' ) || \
			(byte[0] == '\''&& byte[1] == '\''&& byte[2] == '\'') || \
			(byte[0] == '`' && byte[1] == '`' && byte[2] == '`' )    \
		)
		
		//- rjf: triplet string literals
		if (token_flags == 0 && byte + 2 < byte_opl && is_triple_string_literal(byte))
		{
			U8 literal_style = byte[0];
			token_flags  = TokenFlag_StringLiteral | TokenFlag_StringTriplet;
			token_flags |= (literal_style == '\'') * TokenFlag_StringSingleQuote;
			token_flags |= (literal_style ==  '"') * TokenFlag_StringDoubleQuote;
			token_flags |= (literal_style ==  '`') * TokenFlag_StringTick;
			token_start  = byte;
			token_opl    = byte + 3;

			byte += 3;
			for (;byte <= byte_opl; byte += 1)
			{
				if (byte == byte_opl) {
					token_flags |= TokenFlag_BrokenStringLiteral;
					token_opl    = byte;
					break;
				}
				if (byte + 2 < byte_opl && (byte[0] == literal_style && byte[1] == literal_style && byte[2] == literal_style)) { 
					byte      += 3;
					token_opl  = byte;
					break;
				}
			}
		}
		#undef is_triple_string_literal
		
		//- rjf: singlet string literals
		if (token_flags == 0 && (byte[0] == '"' || byte[0] == '\'' || byte[0] == '`'))
		{
			U8 literal_style = byte[0];
			token_flags  = TokenFlag_StringLiteral;
			token_flags |= (literal_style == '\'') * TokenFlag_StringSingleQuote;
			token_flags |= (literal_style ==  '"') * TokenFlag_StringDoubleQuote;
			token_flags |= (literal_style ==  '`') * TokenFlag_StringTick;
			token_start  = byte;
			token_opl    = byte + 1;

			byte += 1;
			B32 escaped = 0;
			for (;byte <= byte_opl; byte += 1)
			{
				if (byte == byte_opl || *byte == '\n') {
					token_opl = byte;
					token_flags |= TokenFlag_BrokenStringLiteral;
					break;
				}
				if (!escaped && byte[0] == '\\') {
					escaped = 1;
				}
				else if (!escaped && byte[0] == literal_style) {
					token_opl = byte+1;
					byte += 1;
					break;
				}
				else if (escaped) {
					escaped = 0;
				}
			}
		}

		#define is_non_reserved_symbol(byte) (                                              \
			*byte == '~' || *byte == '!' || *byte == '$' || *byte == '%' || *byte == '^' || \
			*byte == '&' || *byte == '*' || *byte == '-' || *byte == '=' || *byte == '+' || \
			*byte == '<' || *byte == '.' || *byte == '>' || *byte == '/' || *byte == '?' || \
			*byte == '|'                                                                    \
		)
		#if 0
		(
			*byte != '~' && *byte != '!' && *byte != '$' && *byte != '%' && *byte != '^' &&
			*byte != '&' && *byte != '*' && *byte != '-' && *byte != '=' && *byte != '+' &&
			*byte != '<' && *byte != '.' && *byte != '>' && *byte != '/' && *byte != '?' &&
			*byte != '|'
		)
		#endif
		
		//- rjf: non-reserved symbols
		if (token_flags == 0 && is_non_reserved_symbol(byte))
		{
			token_flags = TokenFlag_Symbol;
			token_start = byte;
			token_opl   = byte;

			byte += 1;
			for (;byte <= byte_opl; byte += 1)
			{
				token_opl += 1;
				if (byte == byte_opl || !is_non_reserved_symbol(byte)) {
					break;
				}
			}
		}
		#undef is_non_reserved_symbol

		#define is_reserved_symbol(byte) (  \
			*byte == '{' || *byte == '}' || \
			*byte == '(' || *byte == ')' || \
			*byte == '[' || *byte == ']' || \
			*byte == '#' ||                 \
			*byte == ',' ||                 \
			*byte == '\\'||                 \
			*byte == ':' || *byte == ';' || \
			*byte == '@'                    \
		)
		
		//- rjf: reserved symbols
		if (token_flags == 0 && is_reserved_symbol(byte)) {
			token_flags = TokenFlag_Reserved;
			token_start = byte;
			token_opl   = byte+1;

			byte += 1;
		}
		#undef is_reserved_symbol
		
		//- rjf: bad characters in all other cases
		if (token_flags == 0) {
			token_flags = TokenFlag_BadCharacter;
			token_start = byte;
			token_opl   = byte+1;

			byte += 1;
		}
		
		//- rjf; push token if formed
		if (token_flags != 0 && token_start != 0 && token_opl > token_start) {
			Token token = {{(U64)(token_start - byte_first), (U64)(token_opl - byte_first)}, token_flags};
			token_chunk_list_push(scratch.arena, &tokens, 4096, token);
		}
		
		//- rjf: push errors on unterminated comments
		if (token_flags & TokenFlag_BrokenComment)
		{
			Node*   error        = alloc_node(ainfo, NodeKind_ErrorMarker, 0, str8_lit(""), str8_lit(""), token_start - byte_first);
			String8 error_string = str8_lit("Unterminated comment.");
			msg_list_alloc(ainfo, &msgs, error, MsgKind_Error, error_string);
		}
		
		//- rjf: push errors on unterminated strings
		if (token_flags & TokenFlag_BrokenStringLiteral) {
			Node*   error        = alloc_node(ainfo, NodeKind_ErrorMarker, 0, str8_lit(""), str8_lit(""), token_start - byte_first);
			String8 error_string = str8_lit("Unterminated string literal.");
			msg_list_alloc(ainfo, &msgs, error, MsgKind_Error, error_string);
		}
	}
	
	//- rjf: bake, fill & return
	TokenizeResult result = {0}; {
		result.tokens = token_array_from_chunk_list_alloc(ainfo, &tokens);
		result.msgs   = msgs;
	}
	scratch_end(scratch);
	return result;
}

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

typedef enum ParseWorkKind ParseWorkKind;
enum ParseWorkKind
{
	ParseWorkKind_Main,
	ParseWorkKind_MainImplicit,
	ParseWorkKind_NodeOptionalFollowUp,
	ParseWorkKind_NodeChildrenStyleScan,

	ParseWorkKind_UnderlyingType = MAX_U32,
};

typedef struct ParseWorkNode ParseWorkNode;
struct ParseWorkNode
{
	ParseWorkNode* next;
	ParseWorkKind  kind;
	Node*          parent;
	Node*          first_gathered_tag;
	Node*          last_gathered_tag;
	NodeFlags      gathered_node_flags;
	S32            counted_newlines;
};

inline
void parse__work_push(ParseWorkKind work_kind, Node* work_parent, ParseWorkNode* work_top, ParseWorkNode* work_free, TempArena* scratch)
{
	ParseWorkNode* work_node = work_free;
	if (work_node == 0) {
		work_node = push_array(scratch->arena, ParseWorkNode, 1);
	}
	else {
		sll_stack_pop(work_free);
	}
	work_node->kind   = (work_kind);
	work_node->parent = (work_parent);
	sll_stack_push(work_top, work_node);
}

inline
void parse__work_pop(ParseWorkNode* work_top, ParseWorkNode* broken_work) {
	sll_stack_pop(work_top);
	if (work_top == 0) {
		work_top = broken_work;
	}
}

ParseResult
parse_from_text_tokens_push(Arena* arena, String8 filename, String8 text, TokenArray tokens) {
	return parse_from_text_tokens_alloc(arena_allocator(arena), filename, text, tokens);
}

ParseResult
parse_from_text_tokens_alloc(AllocatorInfo ainfo, String8 filename, String8 text, TokenArray tokens)
{
	TempArena scratch = scratch_begin(ainfo);
	
	//- rjf: set up outputs
	MsgList msgs = {0};
	Node*   root = alloc_node(ainfo, NodeKind_File, 0, filename, text, 0);
	
	//- rjf: set up parse rule stack
	ParseWorkNode  first_work  = { 0, ParseWorkKind_Main, root };
	ParseWorkNode  broken_work = { 0, ParseWorkKind_Main, root };
	ParseWorkNode* work_top    = &first_work;
	ParseWorkNode* work_free   = 0;

	#define parse_work_push(work_kind, work_parent) parse__work_push(work_kind, work_parent, work_top, work_free, &scratch)
	#define parse_work_pop()                        parse__work_pop (work_top, &broken_work)
	
	//- rjf: parse
	Token* tokens_first = tokens.v;
	Token* tokens_opl   = tokens_first + tokens.count;
	Token* token        = tokens_first;
	for (;token < tokens_opl;)
	{
		//- rjf: unpack token
		String8 token_string = str8_substr(text, token[0].range);

		// Note(Ed): Each of the the follwoing conditionals will always terminate the iteration path to end_consume label.
		
		// TODO(Ed): Add opt-in support for whitespace awareness
		//- rjf: whitespace -> always no-op & inc
		if (token->flags & TokenFlag_Whitespace) {
			token += 1;
			goto end_consume;
			// <whitespace>
		}
		
		// TODO(Ed): Add opt-in support for comment awareness
		//- rjf: comments -> always no-op & inc
		if (token->flags & TokenFlagGroup_Comment) {
			token += 1;
			goto end_consume;
			// < // > <content> <unescaped newline>
			// or 
			// /* <content> */
		}
		
		//- rjf: [node follow up] : following label -> work top parent has children. 
		// we need to scan for explicit delimiters, else parse an implicitly delimited set of children
		if (work_top->kind == ParseWorkKind_NodeOptionalFollowUp && str8_match(token_string, str8_lit(":"), 0)) {
			Node* parent = work_top->parent;
			parse_work_pop();
			parse_work_push(ParseWorkKind_NodeChildrenStyleScan, parent);
			token += 1;
			goto end_consume;
			// .. <label> :
		}
		
		//- rjf: [node follow up] anything but : following label -> node has no children.
		// just pop & move on
		if (work_top->kind == ParseWorkKind_NodeOptionalFollowUp) {
			parse_work_pop();
			goto end_consume;
			// .. <label> 
		}

		B32 reserved_token  = token->flags & TokenFlag_Reserved;

		NodeFlags separator = (
			NodeFlag_IsBeforeComma     *!! str8_match(token_string, str8_lit(","), 0) |
			NodeFlag_IsBeforeSemicolon *!! str8_match(token_string, str8_lit(";"), 0)
		);
		B32 found_separator = reserved_token && separator;
		
		//- rjf: [main] separators -> mark & inc
		if (work_top->kind == ParseWorkKind_Main && found_separator)
		{
			Node* parent = work_top->parent;
			if (!node_is_nil(parent->last))
			{
				// mark last & working noe with separator flag
				parent->last->flags           |= separator;
				work_top->gathered_node_flags |= separator;
			}
			token += 1;
			goto end_consume;
			// <parent->last> , <work_top>
			// or
			// <parent->last> ; <work_top>
		}
		
		//- rjf: [main_implicit] separators -> pop
		if(work_top->kind == ParseWorkKind_MainImplicit && token->flags & found_separator) {
			parse_work_pop();
			goto end_consume;
		}

		B32 mode_main_or_main_implict = (work_top->kind == ParseWorkKind_Main || work_top->kind == ParseWorkKind_MainImplicit);
		B32 found_unexpected          = reserved_token && (str8_match(token_string, str8_lit("#"), 0) || str8_match(token_string, str8_lit("\\"), 0) || str8_match(token_string, str8_lit(":"), 0));
		
		//- rjf: [main, main_implicit] unexpected reserved tokens
		if (mode_main_or_main_implict && found_unexpected) {
			Node*   error        = alloc_node(ainfo, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
			String8 error_string = str8f(ainfo, "Unexpected reserved symbol \"%S\".", token_string);
			msg_list_alloc(ainfo, &msgs, error, MsgKind_Error, error_string);
			token += 1;
			goto end_consume;
		}

		B32 found_tag = reserved_token && str8_match(token_string, str8_lit("@"), 0);

		//- rjf: [main, main_implicit] tag signifier -> create new tag
		if (mode_main_or_main_implict && found_tag)
		{
			// Token after should be label.
			if (token + 1 >= tokens_opl || !(token[1].flags & TokenFlagGroup_Label))
			{
				Node*   error        = alloc_node(ainfo, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
				String8 error_string = str8_lit("Tag label expected after @ symbol.");
				msg_list_alloc(ainfo, &msgs, error, MsgKind_Error, error_string);

				token += 1;
				goto end_consume;
				 // <Tag> : @ <???> (was not label)
			}
			else
			{
				String8 tag_name_raw = str8_substr(text, token[1].range);
				String8 tag_name     = content_string_from_token_flags_str8(token[1].flags, tag_name_raw);

				Node* node = alloc_node(ainfo, NodeKind_Tag, node_flags_from_token_flags(token[1].flags), tag_name, tag_name_raw, token[0].range.min);
				dll_push_back_npz(nil_node(), work_top->first_gathered_tag, work_top->last_gathered_tag, node, next, prev);

				B32 found_argument_paren = token[2].flags & TokenFlag_Reserved && str8_match(str8_substr(text, token[2].range), str8_lit("("), 0);

				if (token + 2 < tokens_opl && found_argument_paren) {
					token += 3;
					parse_work_push(ParseWorkKind_Main, node);
					// <Tag> : @ <TagName> ( 
				}
				else {
					token += 2;
					// <Tag> : @ <TagName>
				}
				goto end_consume;
			}
		}
		
		//- rjf: [main, main_implicit] label -> create new main
		if (mode_main_or_main_implict && token->flags & TokenFlagGroup_Label)
		{
			String8   node_string_raw = token_string;
			String8   node_string     = content_string_from_token_flags_str8(token->flags, node_string_raw);
			NodeFlags flags           = node_flags_from_token_flags(token->flags)|work_top->gathered_node_flags;

			work_top->gathered_node_flags = 0;

			Node* node = alloc_node(ainfo, NodeKind_Main, flags, node_string, node_string_raw, token[0].range.min);
			node->first_tag = work_top->first_gathered_tag;
			node->last_tag  = work_top->last_gathered_tag;
			for (Node* tag = work_top->first_gathered_tag; !node_is_nil(tag); tag = tag->next) {
				tag->parent = node;
			}
			work_top->first_gathered_tag = work_top->last_gathered_tag = nil_node();

			node_push_child(work_top->parent, node);
			parse_work_push(ParseWorkKind_NodeOptionalFollowUp, node);
			token += 1;
			goto end_consume;
		}

		NodeFlags opening_delimiter = (
			NodeFlag_HasBraceLeft   *!! str8_match(token_string, str8_lit("{"), 0) |
			NodeFlag_HasBracketLeft *!! str8_match(token_string, str8_lit("["), 0) |
			NodeFlag_HasParenLeft   *!! str8_match(token_string, str8_lit("("), 0)
		);
		B32 found_opening_delimiter = reserved_token && opening_delimiter & (NodeFlag_HasBraceLeft | NodeFlag_HasBracketLeft | NodeFlag_HasParenLeft);
		
		//- rjf: [main] {s, [s, and (s -> create new main
		if (work_top->kind == ParseWorkKind_Main && found_opening_delimiter)
		{
			NodeFlags 
			flags  = node_flags_from_token_flags(token->flags) | work_top->gathered_node_flags;
			flags |= opening_delimiter;

			work_top->gathered_node_flags = 0;

			Node* 
			node = alloc_node(ainfo, NodeKind_Main, flags, str8_lit(""), str8_lit(""), token[0].range.min);
			node->first_tag = work_top->first_gathered_tag;
			node->last_tag  = work_top->last_gathered_tag;
			for (Node* tag = work_top->first_gathered_tag; !node_is_nil(tag); tag = tag->next) {
				tag->parent = node;
			}
			work_top->first_gathered_tag = work_top->last_gathered_tag = nil_node();

			node_push_child(work_top->parent, node);
			parse_work_push(ParseWorkKind_Main, node);
			token += 1;
			goto end_consume;
			// <main label> : 
		}
		
		//- rjf: [node children style scan] {s, [s, and (s -> explicitly delimited children
		if (work_top->kind == ParseWorkKind_NodeChildrenStyleScan && found_opening_delimiter)
		{
			Node *parent = work_top->parent;
			parent->flags |= opening_delimiter;
			parse_work_pop();
			parse_work_push(ParseWorkKind_Main, parent);
			token += 1;
			goto end_consume;
		}

		B32 newline_token = token->flags & TokenFlag_Newline;
		
		//- rjf: [node children style scan] count newlines
		if (work_top->kind == ParseWorkKind_NodeChildrenStyleScan && newline_token) {
			work_top->counted_newlines += 1;
			token += 1;
			goto end_consume;
		}
		
		//- rjf: [main_implicit] newline -> pop
		if (work_top->kind == ParseWorkKind_MainImplicit && newline_token) {
			parse_work_pop();
			token += 1;
			goto end_consume;
		}
		
		//- rjf: [all but main_implicit] newline -> no-op & inc
		if (work_top->kind != ParseWorkKind_MainImplicit && newline_token) {
			token += 1;
			goto end_consume;
		}

		//- rjf: [node children style scan] anything causing implicit set -> <2 newlines, all good,
		// >=2 newlines, houston we have a problem
		if (work_top->kind == ParseWorkKind_NodeChildrenStyleScan)
		{
			if (work_top->counted_newlines >= 2)
			{
				Node*   node         = work_top->parent;
				Node*   error        = alloc_node(ainfo, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
				String8 error_string = str8f(ainfo, "More than two newlines following \"%S\", which has implicitly-delimited children, resulting in an empty list of children.", node->string);
				msg_list_alloc(ainfo, &msgs, error, MsgKind_Warning, error_string);
				parse_work_pop();
			}
			else
			{
				Node *parent = work_top->parent;
				parse_work_pop();
				parse_work_push(ParseWorkKind_MainImplicit, parent);
			}
			goto end_consume;
		}

		NodeFlags closing_delimiter = (
			NodeFlag_HasBraceRight   *!! str8_match(token_string, str8_lit("}"), 0) | 
			NodeFlag_HasBracketRight *!! str8_match(token_string, str8_lit("]"), 0) |
			NodeFlag_HasParenRight   *!! str8_match(token_string, str8_lit(")"), 0)
		);
		B32 found_closing_delimiter = reserved_token && closing_delimiter & (NodeFlag_HasBraceRight | NodeFlag_HasBracketRight | NodeFlag_HasParenRight);
		
		//- rjf: [main] }s, ]s, and )s -> pop
		if (work_top->kind == ParseWorkKind_Main && found_closing_delimiter) {
			Node* parent = work_top->parent;
			parent->flags |= closing_delimiter;
			parse_work_pop();
			token += 1;
			goto end_consume;
			// <label> }
			// or
			// <label> ]
			// or
			// <label> )
		}
		
		//- rjf: [main implicit] }s, ]s, and )s -> pop without advancing
		if (work_top->kind == ParseWorkKind_MainImplicit && reserved_token && found_closing_delimiter) {
			parse_work_pop();
			goto end_consume;
			// <label> }
			// or
			// <label> ]
			// or
			// <label> )
			// (deferred)
		}
		
		//- rjf: no consumption -> unexpected token! we don't know what to do with this.
		{
			Node*   error        = alloc_node(ainfo, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
			String8 error_string = str8f(ainfo, "Unexpected \"%S\" token.", token_string);
			msg_list_alloc(ainfo, &msgs, error, MsgKind_Error, error_string);
			token += 1;
			// ???
		}
		
		end_consume:;
	}
	
	//- rjf: fill & return
	ParseResult result = {0};
	result.root = root;
	result.msgs = msgs;
	scratch_end(scratch);
	return result;
}

#undef parse_work_push
#undef parse_work_pop

////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

ParseResult
parse_from_text(Arena* arena, String8 filename, String8 text) {
	TempArena      scratch  = scratch_begin(&arena, 1);
	TokenizeResult tokenize = tokenize_from_text(scratch.arena, text);
	ParseResult    parse    = parse_from_text_tokens_push(arena, filename, text, tokenize.tokens); 
	scratch_end(scratch);
	return parse;
}

ParseResult
parse_from_text_alloc(AllocatorInfo ainfo, String8 filename, String8 text) {
	TempArena      scratch  = scratch_begin(ainfo);
	TokenizeResult tokenize = tokenize_from_text(scratch.arena, text);
	ParseResult    parse    = parse_from_text_tokens_alloc(ainfo, filename, text, tokenize.tokens); 
	scratch_end(scratch);
	return parse;
}

////////////////////////////////
//~ rjf: Tree -> Text Functions

String8List
debug_string_list_from_tree(Arena* arena, Node* root)
{
	String8List strings = {0};
	{
		// Depth-first traversal of tree.
		char* indentation = "                                                                                                                                ";
		S32   depth       = 0;
		for (Node* node = root, *next = nil_node(); !node_is_nil(node); node = next)
		{
			// rjf: get next recursion
			NodeRec 
			rec  = node_rec_depth_first_pre(node, root);
			next = rec.next;
			
			// rjf: extract node info
			String8 kind_string = str8_lit("Unknown");
			switch (node->kind)
			{
				default: {} break;

				case NodeKind_File:        { kind_string = str8_lit("File"       ); } break;
				case NodeKind_ErrorMarker: { kind_string = str8_lit("ErrorMarker"); } break;
				case NodeKind_Main:        { kind_string = str8_lit("Main"       ); } break;
				case NodeKind_Tag:         { kind_string = str8_lit("Tag"        ); } break;
				case NodeKind_List:        { kind_string = str8_lit("List"       ); } break;
				case NodeKind_Reference:   { kind_string = str8_lit("Reference"  ); } break;
			}
			
			// rjf: push node line
			str8_list_pushf(arena, &strings, "%.*s\"%S\" : %S", depth, indentation, node->string, kind_string);
			
			// rjf: children -> open brace
			if (rec.push_count != 0) {
				str8_list_pushf(arena, &strings, "%.*s\n{", depth, indentation);
			}
			
			// rjf: descend
			depth += rec.push_count;
			
			// rjf: popping -> close braces
			for (S32 pop_idx = 0; pop_idx < rec.pop_count; pop_idx += 1) {
				str8_list_pushf(arena, &strings, "%.*s\n}", depth - 1 - pop_idx, indentation);
			}
			
			// rjf: ascend
			depth -= rec.pop_count;
		}
	}
	return strings;
}
