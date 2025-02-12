#ifdef INTELLISENSE_DIRECTIVES
#	include "base/thread_context.h"
#	include "mdesk.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
// MD_Context

void md_init(MD_Context* ctx)
{
	if ( ! ctx->dont_init_os) {
		md_os_init(& ctx->os_ctx, & ctx->thread_ctx);
	}

	
}

void md_deinit(MD_Context* ctx)
{
	// Nothing for now
}

////////////////////////////////
//~ rjf: Message Type Functions

void
md_msg_list_push__ainfo(MD_AllocatorInfo ainfo, MD_MsgList* msgs, MD_Node* node, MD_MsgKind kind, MD_String8 string)
{
	MD_Msg*
	msg = md_alloc_array(ainfo, MD_Msg, 1);
	msg->node   = node;
	msg->kind   = kind;
	msg->string = string;

	md_sll_queue_push(msgs->first, msgs->last, msg);

	msgs->count             += 1;
	msgs->worst_message_kind = md_max(kind, msgs->worst_message_kind);
}

void
md_msg_list_concat_in_place(MD_MsgList* dst, MD_MsgList* to_push)
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
			md_memory_copy_struct(dst, to_push);
		}
	}
	md_memory_zero_struct(to_push);
}

////////////////////////////////
//~ rjf: MD_Token Type Functions

MD_String8
content_string_from_token_flags_str8(MD_TokenFlags flags, MD_String8 string)
{
	MD_U64 num_chop = 0;
	MD_U64 num_skip = 0;
	{
		num_skip += 3 * !!(flags & MD_TokenFlag_StringTriplet);
		num_chop += 3 * !!(flags & MD_TokenFlag_StringTriplet);
		num_skip += 1 * (!(flags & MD_TokenFlag_StringTriplet) && flags & MD_TokenFlag_StringLiteral);
		num_chop += 1 * (!(flags & MD_TokenFlag_StringTriplet) && flags & MD_TokenFlag_StringLiteral);
	}
	MD_String8 result = string;
	result = md_str8_chop(result, num_chop);
	result = md_str8_skip(result, num_skip);
	return result;
}

MD_String8List
md_string_list_from_token_flags__ainfo(MD_AllocatorInfo ainfo, MD_TokenFlags flags)
{
	MD_String8List strs = {0};
	if (flags & MD_TokenFlag_Identifier          ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Identifier"         )); }
	if (flags & MD_TokenFlag_Numeric             ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Numeric"            )); }
	if (flags & MD_TokenFlag_StringLiteral       ){ md_str8_list_push(ainfo, &strs, md_str8_lit("StringLiteral"      )); }
	if (flags & MD_TokenFlag_Symbol              ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Symbol"             )); }
	if (flags & MD_TokenFlag_Reserved            ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Reserved"           )); }
	if (flags & MD_TokenFlag_Comment             ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Comment"            )); }
	if (flags & MD_TokenFlag_Whitespace          ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Whitespace"         )); }
	if (flags & MD_TokenFlag_Newline             ){ md_str8_list_push(ainfo, &strs, md_str8_lit("Newline"            )); }
	if (flags & MD_TokenFlag_BrokenComment       ){ md_str8_list_push(ainfo, &strs, md_str8_lit("BrokenComment"      )); }
	if (flags & MD_TokenFlag_BrokenStringLiteral ){ md_str8_list_push(ainfo, &strs, md_str8_lit("BrokenStringLiteral")); }
	if (flags & MD_TokenFlag_BadCharacter        ){ md_str8_list_push(ainfo, &strs, md_str8_lit("BadCharacter"       )); }
	return strs;
}

void
md_token_chunk_list_push__ainfo(MD_AllocatorInfo ainfo, MD_TokenChunkList* list, MD_U64 cap, MD_Token token)
{
	MD_TokenChunkNode* node = list->last;
	if (node == 0 || node->count >= node->cap) {
		node      = md_alloc_array(ainfo, MD_TokenChunkNode, 1);
		node->cap = cap;
		node->v   = md_alloc_array_no_zero(ainfo, MD_Token, cap);
		md_sll_queue_push(list->first, list->last, node);
		list->chunk_count += 1;
	}
	md_memory_copy_struct(&node->v[node->count], &token);
	node->count             += 1;
	list->total_token_count += 1;
}

MD_TokenArray
md_token_array_from_chunk_list__ainfo(MD_AllocatorInfo ainfo, MD_TokenChunkList* chunks)
{
	MD_TokenArray result = {0};
	result.count = chunks->total_token_count;
	result.v     = md_alloc_array_no_zero(ainfo, MD_Token, result.count);
	MD_U64 write_idx = 0;
	for(MD_TokenChunkNode *n = chunks->first; n != 0; n = n->next)
	{
		md_memory_copy(result.v + write_idx, n->v, size_of(MD_Token) * n->count);
		write_idx += n->count;
	}
	return result;
}

////////////////////////////////
//~ rjf: MD_Node Type Functions

//- rjf: iteration

MD_NodeRec
md_node_rec_depth_first(MD_Node* node, MD_Node* subtree_root, MD_U64 child_off, MD_U64 sib_off)
{
	MD_NodeRec 
	rec      = {0};
	rec.next = md_nil_node();
	if ( ! md_node_is_nil(*md_member_from_offset(MD_Node**, node, child_off))) {
		rec.next       = *md_member_from_offset(MD_Node**, node, child_off);
		rec.md_push_count = 1;
	}
	else for (MD_Node* p = node; !md_node_is_nil(p) && p != subtree_root; p = p->parent, rec.pop_count += 1)
	{
		if ( ! md_node_is_nil(*md_member_from_offset(MD_Node**, p, sib_off))) {
			rec.next = *md_member_from_offset(MD_Node**, p, sib_off);
			break;
		}
	}
	return rec;
}

//- rjf: tree building

void
md_unhook(MD_Node* node) {
	MD_Node* parent = node->parent;
	if ( ! md_node_is_nil(parent))
	{
		if (node->kind == MD_NodeKind_Tag) md_dll_remove_npz(md_nil_node(), parent->first_tag, parent->last_tag, node, next, prev);
		else                            md_dll_remove_npz(md_nil_node(), parent->first,     parent->last,     node, next, prev);
		node->parent = md_nil_node();
	}
}

//- rjf: tree introspection

MD_String8
md_string_from_children__ainfo(MD_AllocatorInfo ainfo, MD_Node*  root)
{
	MD_String8 result;
	MD_TempArena scratch = md_scratch_begin(ainfo);
	{
		MD_String8List strs = {0};
		for md_each_node(child, root->first)
		{
			if (child->flags == child->prev->flags) {
				md_str8_list_push(scratch.arena, &strs, md_str8_lit(" "));
			}
			md_str8_list_push(scratch.arena, &strs, child->string);
		
		}
		result = md_str8_list_join(ainfo, &strs, 0);
	}
	scratch_end(scratch);
	return result;
}

//- rjf: tree comparison

MD_B32
md_node_match(MD_Node* a, MD_Node* b, MD_StringMatchFlags flags)
{
	MD_B32 result = 0;
	if (a->kind == b->kind && md_str8_match(a->string, b->string, flags))
	{
		result = 1;
		if (result) {
			result = result && a->flags == b->flags;
		}
		if (result && a->kind != MD_NodeKind_Tag)
		{
			for (MD_Node* a_tag = a->first_tag, *b_tag = b->first_tag; !md_node_is_nil(a_tag) || !md_node_is_nil(b_tag); a_tag = a_tag->next, b_tag = b_tag->next)
			{
				if (md_node_match(a_tag, b_tag, flags))
				{
					for (MD_Node* a_tag_arg = a_tag->first, *b_tag_arg = b_tag->first; !md_node_is_nil(a_tag_arg) || !md_node_is_nil(b_tag_arg); a_tag_arg = a_tag_arg->next, b_tag_arg = b_tag_arg->next)
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

MD_B32
tree_match(MD_Node* a, MD_Node* b, MD_StringMatchFlags flags)
{
	MD_B32 result = md_node_match(a, b, flags);
	if (result)
	{
		for(MD_Node *a_child = a->first, *b_child = b->first; !md_node_is_nil(a_child) || !md_node_is_nil(b_child); a_child = a_child->next, b_child = b_child->next)
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

MD_Node*
md_tree_copy__ainfo(MD_AllocatorInfo ainfo, MD_Node* src_root)
{
	MD_Node* dst_root   = md_nil_node();
	MD_Node* dst_parent = dst_root;
	{
		MD_NodeRec rec = {0};
		for(MD_Node* src = src_root; !md_node_is_nil(src); src = rec.next)
		{
			MD_Node* dst = md_alloc_array(ainfo, MD_Node, 1);
			dst->first      = dst->last = dst->parent = dst->next = dst->prev = md_nil_node();
			dst->first_tag  = dst->last_tag = md_nil_node();
			dst->kind       = src->kind;
			dst->flags      = src->flags;
			dst->string     = md_str8_copy(ainfo, src->string);
			dst->raw_string = md_str8_copy(ainfo, src->raw_string);
			dst->src_offset = src->src_offset;
			dst->parent     = dst_parent;
			if (dst_parent != md_nil_node()) {
				md_dll_push_back_npz(md_nil_node(), dst_parent->first, dst_parent->last, dst, next, prev);
			}
			else {
				dst_root = dst_parent = dst;
			}

			rec = md_node_rec_depth_first_pre(src, src_root);
			if (rec.md_push_count != 0) {
				dst_parent = dst;
			}
			else for (MD_U64 idx = 0; idx < rec.pop_count; idx += 1) {
				dst_parent = dst_parent->parent;
			}
		}
	}
	return dst_root;
}

////////////////////////////////
//~ rjf: Text -> Tokens Functions

MD_TokenizeResult
md_tokenize_from_text__ainfo(MD_AllocatorInfo ainfo, MD_String8 text)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);

	MD_TokenChunkList tokens = {0};
	MD_MsgList        msgs = {0};

	MD_U8* byte_first = text.str;
	MD_U8* byte_opl   = byte_first + text.size; // one-past-last
	MD_U8* byte       = byte_first;

	//- rjf: scan string & produce tokens
	for (;byte < byte_opl;)
	{
		MD_TokenFlags md_token_flags = 0;
		MD_U8*        md_token_start = 0;
		MD_U8*        md_token_opl   = 0;

		#define is_whitespace(byte) (*byte == ' ' || *byte == '\t' || *byte == '\v' || *byte == '\r')
		
		//- rjf: whitespace
		if (md_token_flags == 0 && is_whitespace(byte))
		{
			md_token_flags = MD_TokenFlag_Whitespace;
			md_token_start = byte;
			md_token_opl   = byte;

			byte += 1;
			for (;byte <= byte_opl; byte += 1)
			{
				md_token_opl += 1;
				if (byte == byte_opl || !is_whitespace(byte)) {
					break;
				}
			}
		}
		#undef is_whitspace
		
		//- rjf: newlines
		if (md_token_flags == 0 && *byte == '\n')
		{
			md_token_flags = MD_TokenFlag_Newline;
			md_token_start = byte;
			md_token_opl   = byte+1;

			byte += 1;
		}
		
		//- rjf: single-line comments
		if (md_token_flags == 0 && (byte + 1 < byte_opl && *byte == '/' && byte[1] == '/'))
		{
			md_token_flags = MD_TokenFlag_Comment;
			md_token_start = byte;
			md_token_opl   = byte+2;

			byte += 2;

			MD_B32 escaped = 0;
			for (;byte <= byte_opl; byte += 1)
			{
				md_token_opl += 1;
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
		if (md_token_flags == 0 && (byte + 1 < byte_opl && *byte == '/' && byte[1] == '*'))
		{
			md_token_flags = MD_TokenFlag_Comment;
			md_token_start = byte;
			md_token_opl   = byte + 2;

			byte += 2;
			for (;byte <= byte_opl; byte += 1)
			{
				md_token_opl += 1;
				if (byte == byte_opl) {
					md_token_flags |= MD_TokenFlag_BrokenComment;
					break;
				}
				if (byte + 1 < byte_opl && byte[0] == '*' && byte[1] == '/') {
					md_token_opl += 2;
					break;
				}
			}
		}

		#define is_identifier(byte) (         \
			('A' <= *byte && *byte <= 'Z') || \
			('a' <= *byte && *byte <= 'z') || \
			*byte == '_'                   || \
			md_utf8_class(*byte >> 3) >= 2       \
		)
		#if 0
		(
			!('A' <= *byte && *byte <= 'Z') && 
			!('a' <= *byte && *byte <= 'z') && 
			!('0' <= *byte && *byte <= '9') && 
			*byte != '_' && 
			md_utf8_class(*byte>>3) < 2
		)
		#endif
		
		//- rjf: identifiers
		if (md_token_flags == 0 && is_identifier(byte))
		{
			md_token_flags = MD_TokenFlag_Identifier;
			md_token_start = byte;
			md_token_opl   = byte;

			byte += 1;
			for(;byte <= byte_opl; byte += 1)
			{
				md_token_opl += 1;
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
		if (md_token_flags == 0 && is_numeric(byte)) 
		{
			md_token_flags = MD_TokenFlag_Numeric;
			md_token_start = byte;
			md_token_opl   = byte;

			byte += 1;
			for (;byte <= byte_opl; byte += 1)
			{
				md_token_opl += 1;
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
		if (md_token_flags == 0 && byte + 2 < byte_opl && is_triple_string_literal(byte))
		{
			MD_U8 literal_style = byte[0];
			md_token_flags  = MD_TokenFlag_StringLiteral | MD_TokenFlag_StringTriplet;
			md_token_flags |= (literal_style == '\'') * MD_TokenFlag_StringSingleQuote;
			md_token_flags |= (literal_style ==  '"') * MD_TokenFlag_StringDoubleQuote;
			md_token_flags |= (literal_style ==  '`') * MD_TokenFlag_StringTick;
			md_token_start  = byte;
			md_token_opl    = byte + 3;

			byte += 3;
			for (;byte <= byte_opl; byte += 1)
			{
				if (byte == byte_opl) {
					md_token_flags |= MD_TokenFlag_BrokenStringLiteral;
					md_token_opl    = byte;
					break;
				}
				if (byte + 2 < byte_opl && (byte[0] == literal_style && byte[1] == literal_style && byte[2] == literal_style)) { 
					byte      += 3;
					md_token_opl  = byte;
					break;
				}
			}
		}
		#undef is_triple_string_literal
		
		//- rjf: singlet string literals
		if (md_token_flags == 0 && (byte[0] == '"' || byte[0] == '\'' || byte[0] == '`'))
		{
			MD_U8 literal_style = byte[0];
			md_token_flags  = MD_TokenFlag_StringLiteral;
			md_token_flags |= (literal_style == '\'') * MD_TokenFlag_StringSingleQuote;
			md_token_flags |= (literal_style ==  '"') * MD_TokenFlag_StringDoubleQuote;
			md_token_flags |= (literal_style ==  '`') * MD_TokenFlag_StringTick;
			md_token_start  = byte;
			md_token_opl    = byte + 1;

			byte += 1;
			MD_B32 escaped = 0;
			for (;byte <= byte_opl; byte += 1)
			{
				if (byte == byte_opl || *byte == '\n') {
					md_token_opl = byte;
					md_token_flags |= MD_TokenFlag_BrokenStringLiteral;
					break;
				}
				if (!escaped && byte[0] == '\\') {
					escaped = 1;
				}
				else if (!escaped && byte[0] == literal_style) {
					md_token_opl = byte+1;
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
		if (md_token_flags == 0 && is_non_reserved_symbol(byte))
		{
			md_token_flags = MD_TokenFlag_Symbol;
			md_token_start = byte;
			md_token_opl   = byte;

			byte += 1;
			for (;byte <= byte_opl; byte += 1)
			{
				md_token_opl += 1;
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
		if (md_token_flags == 0 && is_reserved_symbol(byte)) {
			md_token_flags = MD_TokenFlag_Reserved;
			md_token_start = byte;
			md_token_opl   = byte+1;

			byte += 1;
		}
		#undef is_reserved_symbol
		
		//- rjf: bad characters in all other cases
		if (md_token_flags == 0) {
			md_token_flags = MD_TokenFlag_BadCharacter;
			md_token_start = byte;
			md_token_opl   = byte+1;

			byte += 1;
		}
		
		//- rjf; push token if formed
		if (md_token_flags != 0 && md_token_start != 0 && md_token_opl > md_token_start) {
			MD_Token token = {{(MD_U64)(md_token_start - byte_first), (MD_U64)(md_token_opl - byte_first)}, md_token_flags};
			md_token_chunk_list_push(scratch.arena, &tokens, 4096, token);
		}
		
		//- rjf: push errors on unterminated comments
		if (md_token_flags & MD_TokenFlag_BrokenComment)
		{
			MD_Node*   error        = md_push_node(ainfo, MD_NodeKind_ErrorMarker, 0, md_str8_lit(""), md_str8_lit(""), md_token_start - byte_first);
			MD_String8 error_string = md_str8_lit("Unterminated comment.");
			md_msg_list_push(ainfo, &msgs, error, MD_MsgKind_Error, error_string);
		}
		
		//- rjf: push errors on unterminated strings
		if (md_token_flags & MD_TokenFlag_BrokenStringLiteral) {
			MD_Node*   error        = md_push_node(ainfo, MD_NodeKind_ErrorMarker, 0, md_str8_lit(""), md_str8_lit(""), md_token_start - byte_first);
			MD_String8 error_string = md_str8_lit("Unterminated string literal.");
			md_msg_list_push(ainfo, &msgs, error, MD_MsgKind_Error, error_string);
		}
	}
	
	//- rjf: bake, fill & return
	MD_TokenizeResult result = {0}; {
		result.tokens = md_token_array_from_chunk_list(ainfo, &tokens);
		result.msgs   = msgs;
	}
	scratch_end(scratch);
	return result;
}

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

typedef enum MD_ParseWorkKind MD_ParseWorkKind;
enum MD_ParseWorkKind
{
	ParseWorkKind_Main,
	ParseWorkKind_MainImplicit,
	ParseWorkKind_NodeOptionalFollowUp,
	ParseWorkKind_NodeChildrenStyleScan,

	ParseWorkKind_UnderlyingType = MD_MAX_U32,
};

typedef struct MD_ParseWorkNode MD_ParseWorkNode;
struct MD_ParseWorkNode
{
	MD_ParseWorkNode* next;
	MD_ParseWorkKind  kind;
	MD_Node*          parent;
	MD_Node*          first_gathered_tag;
	MD_Node*          last_gathered_tag;
	NodeFlags      gathered_node_flags;
	MD_S32            counted_newlines;
};

inline
void md_parse__work_push(MD_ParseWorkKind work_kind, MD_Node* work_parent, MD_ParseWorkNode* work_top, MD_ParseWorkNode* work_free, MD_TempArena* scratch)
{
	MD_ParseWorkNode* work_node = work_free;
	if (work_node == 0) {
		work_node = md_push_array_(scratch->arena, MD_ParseWorkNode, 1);
	}
	else {
		md_sll_stack_pop(work_free);
	}
	work_node->kind   = (work_kind);
	work_node->parent = (work_parent);
	md_sll_stack_push(work_top, work_node);
}

inline
void md_parse__work_pop(MD_ParseWorkNode* work_top, MD_ParseWorkNode* broken_work) {
	md_sll_stack_pop(work_top);
	if (work_top == 0) {
		work_top = broken_work;
	}
}

MD_ParseResult
md_parse_from_text_tokens__ainfo(MD_AllocatorInfo ainfo, MD_String8 filename, MD_String8 text, MD_TokenArray tokens)
{
	MD_TempArena scratch = md_scratch_begin(ainfo);
	
	//- rjf: set up outputs
	MD_MsgList msgs = {0};
	MD_Node*   root = md_push_node(ainfo, MD_NodeKind_File, 0, filename, text, 0);
	
	//- rjf: set up parse rule stack
	MD_ParseWorkNode  first_work  = { 0, ParseWorkKind_Main, root };
	MD_ParseWorkNode  broken_work = { 0, ParseWorkKind_Main, root };
	MD_ParseWorkNode* work_top    = &first_work;
	MD_ParseWorkNode* work_free   = 0;

	#define parse_work_push(work_kind, work_parent) md_parse__work_push(work_kind, work_parent, work_top, work_free, &scratch)
	#define parse_work_pop()                        md_parse__work_pop (work_top, &broken_work)
	
	//- rjf: parse
	MD_Token* tokens_first = tokens.v;
	MD_Token* tokens_opl   = tokens_first + tokens.count;
	MD_Token* token        = tokens_first;
	for (;token < tokens_opl;)
	{
		//- rjf: unpack token
		MD_String8 md_token_string = md_str8_substr(text, token[0].range);

		// Note(Ed): Each of the the follwoing conditionals will always terminate the iteration path to end_consume label.
		
		// TODO(Ed): Add opt-in support for whitespace awareness
		//- rjf: whitespace -> always no-op & inc
		if (token->flags & MD_TokenFlag_Whitespace) {
			token += 1;
			goto end_consume;
			// <whitespace>
		}
		
		// TODO(Ed): Add opt-in support for comment awareness
		//- rjf: comments -> always no-op & inc
		if (token->flags & MD_TokenFlagGroup_Comment) {
			token += 1;
			goto end_consume;
			// < // > <content> <unescaped newline>
			// or 
			// /* <content> */
		}
		
		//- rjf: [node follow up] : following label -> work top parent has children. 
		// we need to scan for explicit delimiters, else parse an implicitly delimited set of children
		if (work_top->kind == ParseWorkKind_NodeOptionalFollowUp && md_str8_match(md_token_string, md_str8_lit(":"), 0)) {
			MD_Node* parent = work_top->parent;
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

		MD_B32 reserved_token  = token->flags & MD_TokenFlag_Reserved;

		NodeFlags separator = (
			MD_NodeFlag_IsBeforeComma     *!! md_str8_match(md_token_string, md_str8_lit(","), 0) |
			MD_NodeFlag_IsBeforeSemicolon *!! md_str8_match(md_token_string, md_str8_lit(";"), 0)
		);
		MD_B32 found_separator = reserved_token && separator;
		
		//- rjf: [main] separators -> mark & inc
		if (work_top->kind == ParseWorkKind_Main && found_separator)
		{
			MD_Node* parent = work_top->parent;
			if (!md_node_is_nil(parent->last))
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

		MD_B32 mode_main_or_main_implict = (work_top->kind == ParseWorkKind_Main || work_top->kind == ParseWorkKind_MainImplicit);
		MD_B32 found_unexpected          = reserved_token && (md_str8_match(md_token_string, md_str8_lit("#"), 0) || md_str8_match(md_token_string, md_str8_lit("\\"), 0) || md_str8_match(md_token_string, md_str8_lit(":"), 0));
		
		//- rjf: [main, main_implicit] unexpected reserved tokens
		if (mode_main_or_main_implict && found_unexpected) {
			MD_Node*   error        = md_push_node(ainfo, MD_NodeKind_ErrorMarker, 0, md_token_string, md_token_string, token->range.md_min);
			MD_String8 error_string = md_str8f(ainfo, "Unexpected reserved symbol \"%S\".", md_token_string);
			md_msg_list_push(ainfo, &msgs, error, MD_MsgKind_Error, error_string);
			token += 1;
			goto end_consume;
		}

		MD_B32 found_tag = reserved_token && md_str8_match(md_token_string, md_str8_lit("@"), 0);

		//- rjf: [main, main_implicit] tag signifier -> create new tag
		if (mode_main_or_main_implict && found_tag)
		{
			// MD_Token after should be label.
			if (token + 1 >= tokens_opl || !(token[1].flags & MD_TokenFlagGroup_Label))
			{
				MD_Node*   error        = md_push_node(ainfo, MD_NodeKind_ErrorMarker, 0, md_token_string, md_token_string, token->range.md_min);
				MD_String8 error_string = md_str8_lit("Tag label expected after @ symbol.");
				md_msg_list_push(ainfo, &msgs, error, MD_MsgKind_Error, error_string);

				token += 1;
				goto end_consume;
				 // <Tag> : @ <???> (was not label)
			}
			else
			{
				MD_String8 tag_name_raw = md_str8_substr(text, token[1].range);
				MD_String8 tag_name     = content_string_from_token_flags_str8(token[1].flags, tag_name_raw);

				MD_Node* node = md_push_node(ainfo, MD_NodeKind_Tag, md_node_flags_from_token_flags(token[1].flags), tag_name, tag_name_raw, token[0].range.md_min);
				md_dll_push_back_npz(md_nil_node(), work_top->first_gathered_tag, work_top->last_gathered_tag, node, next, prev);

				MD_B32 found_argument_paren = token[2].flags & MD_TokenFlag_Reserved && md_str8_match(md_str8_substr(text, token[2].range), md_str8_lit("("), 0);

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
		if (mode_main_or_main_implict && token->flags & MD_TokenFlagGroup_Label)
		{
			MD_String8   md_node_string_raw = md_token_string;
			MD_String8   md_node_string     = content_string_from_token_flags_str8(token->flags, md_node_string_raw);
			NodeFlags flags           = md_node_flags_from_token_flags(token->flags)|work_top->gathered_node_flags;

			work_top->gathered_node_flags = 0;

			MD_Node* node = md_push_node(ainfo, MD_NodeKind_Main, flags, md_node_string, md_node_string_raw, token[0].range.md_min);
			node->first_tag = work_top->first_gathered_tag;
			node->last_tag  = work_top->last_gathered_tag;
			for (MD_Node* tag = work_top->first_gathered_tag; !md_node_is_nil(tag); tag = tag->next) {
				tag->parent = node;
			}
			work_top->first_gathered_tag = work_top->last_gathered_tag = md_nil_node();

			md_node_push_child(work_top->parent, node);
			parse_work_push(ParseWorkKind_NodeOptionalFollowUp, node);
			token += 1;
			goto end_consume;
		}

		NodeFlags opening_delimiter = (
			MD_NodeFlag_HasBraceLeft   *!! md_str8_match(md_token_string, md_str8_lit("{"), 0) |
			MD_NodeFlag_HasBracketLeft *!! md_str8_match(md_token_string, md_str8_lit("["), 0) |
			MD_NodeFlag_HasParenLeft   *!! md_str8_match(md_token_string, md_str8_lit("("), 0)
		);
		MD_B32 found_opening_delimiter = reserved_token && opening_delimiter & (MD_NodeFlag_HasBraceLeft | MD_NodeFlag_HasBracketLeft | MD_NodeFlag_HasParenLeft);
		
		//- rjf: [main] {s, [s, and (s -> create new main
		if (work_top->kind == ParseWorkKind_Main && found_opening_delimiter)
		{
			NodeFlags 
			flags  = md_node_flags_from_token_flags(token->flags) | work_top->gathered_node_flags;
			flags |= opening_delimiter;

			work_top->gathered_node_flags = 0;

			MD_Node* 
			node = md_push_node(ainfo, MD_NodeKind_Main, flags, md_str8_lit(""), md_str8_lit(""), token[0].range.md_min);
			node->first_tag = work_top->first_gathered_tag;
			node->last_tag  = work_top->last_gathered_tag;
			for (MD_Node* tag = work_top->first_gathered_tag; !md_node_is_nil(tag); tag = tag->next) {
				tag->parent = node;
			}
			work_top->first_gathered_tag = work_top->last_gathered_tag = md_nil_node();

			md_node_push_child(work_top->parent, node);
			parse_work_push(ParseWorkKind_Main, node);
			token += 1;
			goto end_consume;
			// <main label> : 
		}
		
		//- rjf: [node children style scan] {s, [s, and (s -> explicitly delimited children
		if (work_top->kind == ParseWorkKind_NodeChildrenStyleScan && found_opening_delimiter)
		{
			MD_Node *parent = work_top->parent;
			parent->flags |= opening_delimiter;
			parse_work_pop();
			parse_work_push(ParseWorkKind_Main, parent);
			token += 1;
			goto end_consume;
		}

		MD_B32 newline_token = token->flags & MD_TokenFlag_Newline;
		
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
				MD_Node*   node         = work_top->parent;
				MD_Node*   error        = md_push_node(ainfo, MD_NodeKind_ErrorMarker, 0, md_token_string, md_token_string, token->range.md_min);
				MD_String8 error_string = md_str8f(ainfo, "More than two newlines following \"%S\", which has implicitly-delimited children, resulting in an empty list of children.", node->string);
				md_msg_list_push(ainfo, &msgs, error, MD_MsgKind_Warning, error_string);
				parse_work_pop();
			}
			else
			{
				MD_Node *parent = work_top->parent;
				parse_work_pop();
				parse_work_push(ParseWorkKind_MainImplicit, parent);
			}
			goto end_consume;
		}

		NodeFlags closing_delimiter = (
			MD_NodeFlag_HasBraceRight   *!! md_str8_match(md_token_string, md_str8_lit("}"), 0) | 
			MD_NodeFlag_HasBracketRight *!! md_str8_match(md_token_string, md_str8_lit("]"), 0) |
			MD_NodeFlag_HasParenRight   *!! md_str8_match(md_token_string, md_str8_lit(")"), 0)
		);
		MD_B32 found_closing_delimiter = reserved_token && closing_delimiter & (MD_NodeFlag_HasBraceRight | MD_NodeFlag_HasBracketRight | MD_NodeFlag_HasParenRight);
		
		//- rjf: [main] }s, ]s, and )s -> pop
		if (work_top->kind == ParseWorkKind_Main && found_closing_delimiter) {
			MD_Node* parent = work_top->parent;
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
			MD_Node*   error        = md_push_node(ainfo, MD_NodeKind_ErrorMarker, 0, md_token_string, md_token_string, token->range.md_min);
			MD_String8 error_string = md_str8f(ainfo, "Unexpected \"%S\" token.", md_token_string);
			md_msg_list_push(ainfo, &msgs, error, MD_MsgKind_Error, error_string);
			token += 1;
			// ???
		}
		
		end_consume:;
	}
	
	//- rjf: fill & return
	MD_ParseResult result = {0};
	result.root = root;
	result.msgs = msgs;
	scratch_end(scratch);
	return result;
}

#undef parse_work_push
#undef parse_work_pop

////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

MD_ParseResult
md_parse_from_text__ainfo(MD_AllocatorInfo ainfo, MD_String8 filename, MD_String8 text) {
	MD_TempArena      scratch  = md_scratch_begin(ainfo);
	MD_TokenizeResult tokenize = md_tokenize_from_text(scratch.arena, text);
	MD_ParseResult    parse    = md_parse_from_text_tokens__ainfo(ainfo, filename, text, tokenize.tokens); 
	scratch_end(scratch);
	return parse;
}

////////////////////////////////
//~ rjf: Tree -> Text Functions

MD_String8List
md_debug_string_list_from_tree__ainfo(MD_AllocatorInfo ainfo, MD_Node* root)
{
	MD_String8List strings = {0};
	{
		// Depth-first traversal of tree.
		char* indentation = "                                                                                                                                ";
		MD_S32   depth       = 0;
		for (MD_Node* node = root, *next = md_nil_node(); !md_node_is_nil(node); node = next)
		{
			// rjf: get next recursion
			MD_NodeRec 
			rec  = md_node_rec_depth_first_pre(node, root);
			next = rec.next;
			
			// rjf: extract node info
			MD_String8 kind_string = md_str8_lit("Unknown");
			switch (node->kind)
			{
				default: {} break;

				case MD_NodeKind_File:        { kind_string = md_str8_lit("File"       ); } break;
				case MD_NodeKind_ErrorMarker: { kind_string = md_str8_lit("ErrorMarker"); } break;
				case MD_NodeKind_Main:        { kind_string = md_str8_lit("Main"       ); } break;
				case MD_NodeKind_Tag:         { kind_string = md_str8_lit("Tag"        ); } break;
				case MD_NodeKind_List:        { kind_string = md_str8_lit("List"       ); } break;
				case MD_NodeKind_Reference:   { kind_string = md_str8_lit("Reference"  ); } break;
			}
			
			// rjf: push node line
			md_str8_list_pushf(ainfo, &strings, "%.*s\"%S\" : %S", depth, indentation, node->string, kind_string);
			
			// rjf: children -> open brace
			if (rec.md_push_count != 0) {
				md_str8_list_pushf(ainfo, &strings, "%.*s\n{", depth, indentation);
			}
			
			// rjf: descend
			depth += rec.md_push_count;
			
			// rjf: popping -> close braces
			for (MD_S32 pop_idx = 0; pop_idx < rec.pop_count; pop_idx += 1) {
				md_str8_list_pushf(ainfo, &strings, "%.*s\n}", depth - 1 - pop_idx, indentation);
			}
			
			// rjf: ascend
			depth -= rec.pop_count;
		}
	}
	return strings;
}
