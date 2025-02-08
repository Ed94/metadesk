#ifdef INTELLISENSE_DIRECTIVES
#	include "mdesk.h"
#endif

// Copyright (c) 2024 Epic Games Tools
// Licensed under the MIT license (https://opensource.org/license/mit/)

////////////////////////////////
//~ rjf: Message Type Functions

void
msg_list_push(Arena* arena, MsgList* msgs, Node* node, MsgKind kind, String8 string)
{
	Msg*
	msg = push_array(arena, Msg, 1);
	msg->node   = node;
	msg->kind   = kind;
	msg->string = string;

	sll_queue_push(msgs->first, msgs->last, msg);

	msgs->count             += 1;
	msgs->worst_message_kind = Max(kind, msgs->worst_message_kind);
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
	String8List strs = {0};
	if (flags & TokenFlag_Identifier          ){ str8_list_push(arena, &strs, str8_lit("Identifier"         )); }
	if (flags & TokenFlag_Numeric             ){ str8_list_push(arena, &strs, str8_lit("Numeric"            )); }
	if (flags & TokenFlag_StringLiteral       ){ str8_list_push(arena, &strs, str8_lit("StringLiteral"      )); }
	if (flags & TokenFlag_Symbol              ){ str8_list_push(arena, &strs, str8_lit("Symbol"             )); }
	if (flags & TokenFlag_Reserved            ){ str8_list_push(arena, &strs, str8_lit("Reserved"           )); }
	if (flags & TokenFlag_Comment             ){ str8_list_push(arena, &strs, str8_lit("Comment"            )); }
	if (flags & TokenFlag_Whitespace          ){ str8_list_push(arena, &strs, str8_lit("Whitespace"         )); }
	if (flags & TokenFlag_Newline             ){ str8_list_push(arena, &strs, str8_lit("Newline"            )); }
	if (flags & TokenFlag_BrokenComment       ){ str8_list_push(arena, &strs, str8_lit("BrokenComment"      )); }
	if (flags & TokenFlag_BrokenStringLiteral ){ str8_list_push(arena, &strs, str8_lit("BrokenStringLiteral")); }
	if (flags & TokenFlag_BadCharacter        ){ str8_list_push(arena, &strs, str8_lit("BadCharacter"       )); }
	return strs;
}

internal void
token_chunk_list_push(Arena* arena, TokenChunkList* list, U64 cap, Token token)
{
  TokenChunkNode *node = list->last;
  if(node == 0 || node->count >= node->cap)
  {
    node = push_array(arena, TokenChunkNode, 1);
    node->cap = cap;
    node->v = push_array_no_zero(arena, Token, cap);
    sll_queue_push(list->first, list->last, node);
    list->chunk_count += 1;
  }
  memory_copy_struct(&node->v[node->count], &token);
  node->count += 1;
  list->total_token_count += 1;
}

internal TokenArray
token_array_from_chunk_list(Arena* arena, TokenChunkList* chunks)
{
	TokenArray result = {0};
	result.count = chunks->total_token_count;
	result.v     = push_array_no_zero(arena, Token, result.count);
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

//- rjf: flag conversions

internal NodeFlags
node_flags_from_token_flags(TokenFlags flags)
{
  NodeFlags result = 0;
  result |=         NodeFlag_Identifier*!!(flags&TokenFlag_Identifier);
  result |=            NodeFlag_Numeric*!!(flags&TokenFlag_Numeric);
  result |=      NodeFlag_StringLiteral*!!(flags&TokenFlag_StringLiteral);
  result |=             NodeFlag_Symbol*!!(flags&TokenFlag_Symbol);
  result |= NodeFlag_StringSingleQuote *!!(flags&TokenFlag_StringSingleQuote);
  result |= NodeFlag_StringDoubleQuote *!!(flags&TokenFlag_StringDoubleQuote);
  result |=         NodeFlag_StringTick*!!(flags&TokenFlag_StringTick);
  result |=      NodeFlag_StringTriplet*!!(flags&TokenFlag_StringTriplet);
  return result;
}

//- rjf: nil

internal B32
node_is_nil(Node *node)
{
  return (node == 0 || node == nil_node() || node->kind == NodeKind_Nil);
}

//- rjf: iteration

internal NodeRec
node_rec_depth_first(Node *node, Node *subtree_root, U64 child_off, U64 sib_off)
{
  NodeRec rec = {0};
  rec.next = nil_node();
  if(!node_is_nil(*member_from_offset(Node **, node, child_off)))
  {
    rec.next = *member_from_offset(Node **, node, child_off);
    rec.push_count = 1;
  }
  else for(Node *p = node; !node_is_nil(p) && p != subtree_root; p = p->parent, rec.pop_count += 1)
  {
    if(!node_is_nil(*member_from_offset(Node **, p, sib_off)))
    {
      rec.next = *member_from_offset(Node **, p, sib_off);
      break;
    }
  }
  return rec;
}

//- rjf: tree building

internal Node *
push_node(Arena *arena, NodeKind kind, NodeFlags flags, String8 string, String8 raw_string, U64 src_offset)
{
  Node *node = push_array(arena, Node, 1);
  node->first = node->last = node->parent = node->next = node->prev = node->first_tag = node->last_tag = nil_node();
  node->kind       = kind;
  node->flags      = flags;
  node->string     = string;
  node->raw_string = raw_string;
  node->src_offset = src_offset;
  return node;
}

internal void
node_insert_child(Node *parent, Node *prev_child, Node *node)
{
  node->parent = parent;
  dll_insert_npz(nil_node(), parent->first, parent->last, prev_child, node, next, prev);
}

internal void
node_insert_tag(Node *parent, Node *prev_child, Node *node)
{
  node->kind = NodeKind_Tag;
  node->parent = parent;
  dll_insert_npz(nil_node(), parent->first_tag, parent->last_tag, prev_child, node, next, prev);
}

internal void
node_push_child(Node *parent, Node *node)
{
  node->parent = parent;
  dll_push_back_npz(nil_node(), parent->first, parent->last, node, next, prev);
}

internal void
node_push_tag(Node *parent, Node *node)
{
  node->kind = NodeKind_Tag;
  node->parent = parent;
  dll_push_back_npz(nil_node(), parent->first_tag, parent->last_tag, node, next, prev);
}

internal void
unhook(Node *node)
{
  Node *parent = node->parent;
  if(!node_is_nil(parent))
  {
    if(node->kind == NodeKind_Tag)
    {
      dll_remove_npz(nil_node(), parent->first_tag, parent->last_tag, node, next, prev);
    }
    else
    {
      dll_remove_npz(nil_node(), parent->first, parent->last, node, next, prev);
    }
    node->parent = nil_node();
  }
}

//- rjf: tree introspection

internal Node *
node_from_chain_string(Node *first, Node *opl, String8 string, StringMatchFlags flags)
{
  Node *result = nil_node();
  for(Node *n = first; !node_is_nil(n) && n != opl; n = n->next)
  {
    if(str8_match(n->string, string, flags))
    {
      result = n;
      break;
    }
  }
  return result;
}

internal Node *
node_from_chain_index(Node *first, Node *opl, U64 index)
{
  Node *result = nil_node();
  S64 idx = 0;
  for(Node *n = first; !node_is_nil(n) && n != opl; n = n->next, idx += 1)
  {
    if(index == idx)
    {
      result = n;
      break;
    }
  }
  return result;
}

internal Node *
node_from_chain_flags(Node *first, Node *opl, NodeFlags flags)
{
  Node *result = nil_node();
  for(Node *n = first; !node_is_nil(n) && n != opl; n = n->next)
  {
    if(n->flags & flags)
    {
      result = n;
      break;
    }
  }
  return result;
}

internal U64
index_from_node(Node *node)
{
  U64 index = 0;
  for(Node *n = node->prev; !node_is_nil(n); n = n->prev)
  {
    index += 1;
  }
  return index;
}

internal Node *
root_from_node(Node *node)
{
  Node *result = node;
  for(Node *p = node->parent; (p->kind == NodeKind_Main || p->kind == NodeKind_Tag) && !node_is_nil(p); p = p->parent)
  {
    result = p;
  }
  return result;
}

internal Node *
child_from_string(Node *node, String8 child_string, StringMatchFlags flags)
{
  return node_from_chain_string(node->first, nil_node(), child_string, flags);
}

internal Node *
tag_from_string(Node *node, String8 tag_string, StringMatchFlags flags)
{
  return node_from_chain_string(node->first_tag, nil_node(), tag_string, flags);
}

internal Node *
child_from_index(Node *node, U64 index)
{
  return node_from_chain_index(node->first, nil_node(), index);
}

internal Node *
tag_from_index(Node *node, U64 index)
{
  return node_from_chain_index(node->first_tag, nil_node(), index);
}

internal Node *
tag_arg_from_index(Node *node, String8 tag_string, StringMatchFlags flags, U64 index)
{
  Node *tag = tag_from_string(node, tag_string, flags);
  return child_from_index(tag, index);
}

internal Node *
tag_arg_from_string(Node *node, String8 tag_string, StringMatchFlags tag_str_flags, String8 arg_string, StringMatchFlags arg_str_flags)
{
  Node *tag = tag_from_string(node, tag_string, tag_str_flags);
  Node *arg = child_from_string(tag, arg_string, arg_str_flags);
  return arg;
}

internal B32
node_has_child(Node *node, String8 string, StringMatchFlags flags)
{
  return !node_is_nil(child_from_string(node, string, flags));
}

internal B32
node_has_tag(Node *node, String8 string, StringMatchFlags flags)
{
  return !node_is_nil(tag_from_string(node, string, flags));
}

internal U64
child_count_from_node(Node *node)
{
  U64 result = 0;
  for(Node *child = node->first; !node_is_nil(child); child = child->next)
  {
    result += 1;
  }
  return result;
}

internal U64
tag_count_from_node(Node *node)
{
  U64 result = 0;
  for(Node *child = node->first_tag; !node_is_nil(child); child = child->next)
  {
    result += 1;
  }
  return result;
}

internal String8
string_from_children(Arena *arena, Node *root)
{
  TempArena scratch = scratch_begin(&arena, 1);
  String8List strs = {0};
  for MD_EachNode(child, root->first)
  {
    if(child->flags == child->prev->flags)
    {
      str8_list_push(scratch.arena, &strs, str8_lit(" "));
    }
    str8_list_push(scratch.arena, &strs, child->string);
  }
  String8 result = str8_list_join(arena, &strs, 0);
  scratch_end(scratch);
  return result;
}

//- rjf: tree comparison

internal B32
node_match(Node *a, Node *b, StringMatchFlags flags)
{
  B32 result = 0;
  if(a->kind == b->kind && str8_match(a->string, b->string, flags))
  {
    result = 1;
    if(result)
    {
      result = result && a->flags == b->flags;
    }
    if(result && a->kind != NodeKind_Tag)
    {
      for(Node *a_tag = a->first_tag, *b_tag = b->first_tag;
          !node_is_nil(a_tag) || !node_is_nil(b_tag);
          a_tag = a_tag->next, b_tag = b_tag->next)
      {
        if(node_match(a_tag, b_tag, flags))
        {
          for(Node *a_tag_arg = a_tag->first, *b_tag_arg = b_tag->first;
              !node_is_nil(a_tag_arg) || !node_is_nil(b_tag_arg);
              a_tag_arg = a_tag_arg->next, b_tag_arg = b_tag_arg->next)
          {
            if(!tree_match(a_tag_arg, b_tag_arg, flags))
            {
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

internal B32
tree_match(Node *a, Node *b, StringMatchFlags flags)
{
  B32 result = node_match(a, b, flags);
  if(result)
  {
    for(Node *a_child = a->first, *b_child = b->first;
        !node_is_nil(a_child) || !node_is_nil(b_child);
        a_child = a_child->next, b_child = b_child->next)
    {
      if(!tree_match(a_child, b_child, flags))
      {
        result = 0;
        goto end;
      }
    }
  }
  end:;
  return result;
}

//- rjf: tree duplication

internal Node *
tree_copy(Arena *arena, Node *src_root)
{
  Node *dst_root = nil_node();
  Node *dst_parent = dst_root;
  {
    NodeRec rec = {0};
    for(Node *src = src_root; !node_is_nil(src); src = rec.next)
    {
      Node *dst = push_array(arena, Node, 1);
      dst->first = dst->last = dst->parent = dst->next = dst->prev = nil_node();
      dst->first_tag = dst->last_tag = nil_node();
      dst->kind  = src->kind;
      dst->flags = src->flags;
      dst->string = push_str8_copy(arena, src->string);
      dst->raw_string = push_str8_copy(arena, src->raw_string);
      dst->src_offset = src->src_offset;
      dst->parent = dst_parent;
      if(dst_parent != nil_node())
      {
        dll_push_back_npz(nil_node(), dst_parent->first, dst_parent->last, dst, next, prev);
      }
      else
      {
        dst_root = dst_parent = dst;
      }
      rec = node_rec_depth_first_pre(src, src_root);
      if(rec.push_count != 0)
      {
        dst_parent = dst;
      }
      else for(U64 idx = 0; idx < rec.pop_count; idx += 1)
      {
        dst_parent = dst_parent->parent;
      }
    }
  }
  return dst_root;
}

////////////////////////////////
//~ rjf: Text -> Tokens Functions

internal TokenizeResult
tokenize_from_text(Arena *arena, String8 text)
{
  TempArena scratch = scratch_begin(&arena, 1);
  TokenChunkList tokens = {0};
  MsgList msgs = {0};
  U8 *byte_first = text.str;
  U8 *byte_opl = byte_first + text.size;
  U8 *byte = byte_first;
  
  //- rjf: scan string & produce tokens
  for(;byte < byte_opl;)
  {
    TokenFlags token_flags = 0;
    U8 *token_start = 0;
    U8 *token_opl = 0;
    
    //- rjf: whitespace
    if(token_flags == 0 && (*byte == ' ' || *byte == '\t' || *byte == '\v' || *byte == '\r'))
    {
      token_flags = TokenFlag_Whitespace;
      token_start = byte;
      token_opl = byte;
      byte += 1;
      for(;byte <= byte_opl; byte += 1)
      {
        token_opl += 1;
        if(byte == byte_opl || (*byte != ' ' && *byte != '\t' && *byte != '\v' && *byte != '\r'))
        {
          break;
        }
      }
    }
    
    //- rjf: newlines
    if(token_flags == 0 && *byte == '\n')
    {
      token_flags = TokenFlag_Newline;
      token_start = byte;
      token_opl = byte+1;
      byte += 1;
    }
    
    //- rjf: single-line comments
    if(token_flags == 0 && (byte+1 < byte_opl && *byte == '/' && byte[1] == '/'))
    {
      token_flags = TokenFlag_Comment;
      token_start = byte;
      token_opl = byte+2;
      byte += 2;
      B32 escaped = 0;
      for(;byte <= byte_opl; byte += 1)
      {
        token_opl += 1;
        if(byte == byte_opl)
        {
          break;
        }
        if(escaped)
        {
          escaped = 0;
        }
        else
        {
          if(*byte == '\n')
          {
            break;
          }
          else if(*byte == '\\')
          {
            escaped = 1;
          }
        }
      }
    }
    
    //- rjf: multi-line comments
    if(token_flags == 0 && (byte+1 < byte_opl && *byte == '/' && byte[1] == '*'))
    {
      token_flags = TokenFlag_Comment;
      token_start = byte;
      token_opl = byte+2;
      byte += 2;
      for(;byte <= byte_opl; byte += 1)
      {
        token_opl += 1;
        if(byte == byte_opl)
        {
          token_flags |= TokenFlag_BrokenComment;
          break;
        }
        if(byte+1 < byte_opl && byte[0] == '*' && byte[1] == '/')
        {
          token_opl += 2;
          break;
        }
      }
    }
    
    //- rjf: identifiers
    if(token_flags == 0 && (('A' <= *byte && *byte <= 'Z') ||
                            ('a' <= *byte && *byte <= 'z') ||
                            *byte == '_' ||
                            utf8_class(*byte>>3) >= 2 ))
    {
      token_flags = TokenFlag_Identifier;
      token_start = byte;
      token_opl = byte;
      byte += 1;
      for(;byte <= byte_opl; byte += 1)
      {
        token_opl += 1;
        if(byte == byte_opl ||
           (!('A' <= *byte && *byte <= 'Z') &&
            !('a' <= *byte && *byte <= 'z') &&
            !('0' <= *byte && *byte <= '9') &&
            *byte != '_' &&
            utf8_class(*byte>>3) < 2))
        {
          break;
        }
      }
    }
    
    //- rjf: numerics
    if(token_flags == 0 && (('0' <= *byte && *byte <= '9') ||
                            (*byte == '.' && byte+1 < byte_opl && '0' <= byte[1] && byte[1] <= '9') ||
                            (*byte == '-' && byte+1 < byte_opl && '0' <= byte[1] && byte[1] <= '9') ||
                            *byte == '_'))
    {
      token_flags = TokenFlag_Numeric;
      token_start = byte;
      token_opl = byte;
      byte += 1;
      for(;byte <= byte_opl; byte += 1)
      {
        token_opl += 1;
        if(byte == byte_opl ||
           (!('A' <= *byte && *byte <= 'Z') &&
            !('a' <= *byte && *byte <= 'z') &&
            !('0' <= *byte && *byte <= '9') &&
            *byte != '_' &&
            *byte != '.'))
        {
          break;
        }
      }
    }
    
    //- rjf: triplet string literals
    if(token_flags == 0 && byte+2 < byte_opl &&
       ((byte[0] == '"' && byte[1] == '"' && byte[2] == '"') ||
        (byte[0] == '\''&& byte[1] == '\''&& byte[2] == '\'') ||
        (byte[0] == '`' && byte[1] == '`' && byte[2] == '`')))
    {
      U8 literal_style = byte[0];
      token_flags = TokenFlag_StringLiteral|TokenFlag_StringTriplet;
      token_flags |= (literal_style == '\'')*TokenFlag_StringSingleQuote;
      token_flags |= (literal_style ==  '"')*TokenFlag_StringDoubleQuote;
      token_flags |= (literal_style ==  '`')*TokenFlag_StringTick;
      token_start = byte;
      token_opl = byte+3;
      byte += 3;
      for(;byte <= byte_opl; byte += 1)
      {
        if(byte == byte_opl)
        {
          token_flags |= TokenFlag_BrokenStringLiteral;
          token_opl = byte;
          break;
        }
        if(byte+2 < byte_opl && (byte[0] == literal_style && byte[1] == literal_style && byte[2] == literal_style))
        {
          byte += 3;
          token_opl = byte;
          break;
        }
      }
    }
    
    //- rjf: singlet string literals
    if(token_flags == 0 && (byte[0] == '"' || byte[0] == '\'' || byte[0] == '`'))
    {
      U8 literal_style = byte[0];
      token_flags = TokenFlag_StringLiteral;
      token_flags |= (literal_style == '\'')*TokenFlag_StringSingleQuote;
      token_flags |= (literal_style ==  '"')*TokenFlag_StringDoubleQuote;
      token_flags |= (literal_style ==  '`')*TokenFlag_StringTick;
      token_start = byte;
      token_opl = byte+1;
      byte += 1;
      B32 escaped = 0;
      for(;byte <= byte_opl; byte += 1)
      {
        if(byte == byte_opl || *byte == '\n')
        {
          token_opl = byte;
          token_flags |= TokenFlag_BrokenStringLiteral;
          break;
        }
        if(!escaped && byte[0] == '\\')
        {
          escaped = 1;
        }
        else if(!escaped && byte[0] == literal_style)
        {
          token_opl = byte+1;
          byte += 1;
          break;
        }
        else if(escaped)
        {
          escaped = 0;
        }
      }
    }
    
    //- rjf: non-reserved symbols
    if(token_flags == 0 && (*byte == '~' || *byte == '!' || *byte == '$' || *byte == '%' || *byte == '^' ||
                            *byte == '&' || *byte == '*' || *byte == '-' || *byte == '=' || *byte == '+' ||
                            *byte == '<' || *byte == '.' || *byte == '>' || *byte == '/' || *byte == '?' ||
                            *byte == '|'))
    {
      token_flags = TokenFlag_Symbol;
      token_start = byte;
      token_opl = byte;
      byte += 1;
      for(;byte <= byte_opl; byte += 1)
      {
        token_opl += 1;
        if(byte == byte_opl ||
           (*byte != '~' && *byte != '!' && *byte != '$' && *byte != '%' && *byte != '^' &&
            *byte != '&' && *byte != '*' && *byte != '-' && *byte != '=' && *byte != '+' &&
            *byte != '<' && *byte != '.' && *byte != '>' && *byte != '/' && *byte != '?' &&
            *byte != '|'))
        {
          break;
        }
      }
    }
    
    //- rjf: reserved symbols
    if(token_flags == 0 && (*byte == '{' || *byte == '}' || *byte == '(' || *byte == ')' ||
                            *byte == '[' || *byte == ']' || *byte == '#' || *byte == ',' ||
                            *byte == '\\'|| *byte == ':' || *byte == ';' || *byte == '@'))
    {
      token_flags = TokenFlag_Reserved;
      token_start = byte;
      token_opl = byte+1;
      byte += 1;
    }
    
    //- rjf: bad characters in all other cases
    if(token_flags == 0)
    {
      token_flags = TokenFlag_BadCharacter;
      token_start = byte;
      token_opl = byte+1;
      byte += 1;
    }
    
    //- rjf; push token if formed
    if(token_flags != 0 && token_start != 0 && token_opl > token_start)
    {
      Token token = {{(U64)(token_start - byte_first), (U64)(token_opl - byte_first)}, token_flags};
      token_chunk_list_push(scratch.arena, &tokens, 4096, token);
    }
    
    //- rjf: push errors on unterminated comments
    if(token_flags & TokenFlag_BrokenComment)
    {
      Node *error = push_node(arena, NodeKind_ErrorMarker, 0, str8_lit(""), str8_lit(""), token_start - byte_first);
      String8 error_string = str8_lit("Unterminated comment.");
      msg_list_push(arena, &msgs, error, MsgKind_Error, error_string);
    }
    
    //- rjf: push errors on unterminated strings
    if(token_flags & TokenFlag_BrokenStringLiteral)
    {
      Node *error = push_node(arena, NodeKind_ErrorMarker, 0, str8_lit(""), str8_lit(""), token_start - byte_first);
      String8 error_string = str8_lit("Unterminated string literal.");
      msg_list_push(arena, &msgs, error, MsgKind_Error, error_string);
    }
  }
  
  //- rjf: bake, fill & return
  TokenizeResult result = {0};
  {
    result.tokens = token_array_from_chunk_list(arena, &tokens);
    result.msgs = msgs;
  }
  scratch_end(scratch);
  return result;
}

////////////////////////////////
//~ rjf: Tokens -> Tree Functions

internal ParseResult
parse_from_text_tokens(Arena *arena, String8 filename, String8 text, TokenArray tokens)
{
  TempArena scratch = scratch_begin(&arena, 1);
  
  //- rjf: set up outputs
  MsgList msgs = {0};
  Node *root = push_node(arena, NodeKind_File, 0, filename, text, 0);
  
  //- rjf: set up parse rule stack
  typedef enum MD_ParseWorkKind
  {
    MD_ParseWorkKind_Main,
    MD_ParseWorkKind_MainImplicit,
    MD_ParseWorkKind_NodeOptionalFollowUp,
    MD_ParseWorkKind_NodeChildrenStyleScan,
  }
  MD_ParseWorkKind;
  typedef struct MD_ParseWorkNode MD_ParseWorkNode;
  struct MD_ParseWorkNode
  {
    MD_ParseWorkNode *next;
    MD_ParseWorkKind kind;
    Node *parent;
    Node *first_gathered_tag;
    Node *last_gathered_tag;
    NodeFlags gathered_node_flags;
    S32 counted_newlines;
  };
  MD_ParseWorkNode first_work =
  {
    0,
    MD_ParseWorkKind_Main,
    root,
  };
  MD_ParseWorkNode broken_work = { 0, MD_ParseWorkKind_Main, root,};
  MD_ParseWorkNode *work_top = &first_work;
  MD_ParseWorkNode *work_free = 0;
#define MD_ParseWorkPush(work_kind, work_parent) do\
{\
MD_ParseWorkNode *work_node = work_free;\
if(work_node == 0) {work_node = push_array(scratch.arena, MD_ParseWorkNode, 1);}\
else { SLLStackPop(work_free); }\
work_node->kind = (work_kind);\
work_node->parent = (work_parent);\
SLLStackPush(work_top, work_node);\
}while(0)
#define MD_ParseWorkPop() do\
{\
SLLStackPop(work_top);\
if(work_top == 0) {work_top = &broken_work;}\
}while(0)
  
  //- rjf: parse
  Token *tokens_first = tokens.v;
  Token *tokens_opl = tokens_first + tokens.count;
  Token *token = tokens_first;
  for(;token < tokens_opl;)
  {
    //- rjf: unpack token
    String8 token_string = str8_substr(text, token[0].range);
    
    //- rjf: whitespace -> always no-op & inc
    if(token->flags & TokenFlag_Whitespace)
    {
      token += 1;
      goto end_consume;
    }
    
    //- rjf: comments -> always no-op & inc
    if(token->flags & TokenGroup_Comment)
    {
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [node follow up] : following label -> work top parent has children. we need
    // to scan for explicit delimiters, else parse an implicitly delimited set of children
    if(work_top->kind == MD_ParseWorkKind_NodeOptionalFollowUp && str8_match(token_string, str8_lit(":"), 0))
    {
      Node *parent = work_top->parent;
      MD_ParseWorkPop();
      MD_ParseWorkPush(MD_ParseWorkKind_NodeChildrenStyleScan, parent);
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [node follow up] anything but : following label -> node has no children. just
    // pop & move on
    if(work_top->kind == MD_ParseWorkKind_NodeOptionalFollowUp)
    {
      MD_ParseWorkPop();
      goto end_consume;
    }
    
    //- rjf: [main] separators -> mark & inc
    if(work_top->kind == MD_ParseWorkKind_Main && token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit(","), 0) ||
        str8_match(token_string, str8_lit(";"), 0)))
    {
      Node *parent = work_top->parent;
      if(!node_is_nil(parent->last))
      {
        parent->last->flags |=     NodeFlag_IsBeforeComma*!!str8_match(token_string, str8_lit(","), 0);
        parent->last->flags |= NodeFlag_IsBeforeSemicolon*!!str8_match(token_string, str8_lit(";"), 0);
        work_top->gathered_node_flags |=     NodeFlag_IsAfterComma*!!str8_match(token_string, str8_lit(","), 0);
        work_top->gathered_node_flags |= NodeFlag_IsAfterSemicolon*!!str8_match(token_string, str8_lit(";"), 0);
      }
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [main_implicit] separators -> pop
    if(work_top->kind == MD_ParseWorkKind_MainImplicit && token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit(","), 0) ||
        str8_match(token_string, str8_lit(";"), 0)))
    {
      MD_ParseWorkPop();
      goto end_consume;
    }
    
    //- rjf: [main, main_implicit] unexpected reserved tokens
    if((work_top->kind == MD_ParseWorkKind_Main || work_top->kind == MD_ParseWorkKind_MainImplicit) &&
       token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit("#"), 0) ||
        str8_match(token_string, str8_lit("\\"), 0) ||
        str8_match(token_string, str8_lit(":"), 0)))
    {
      Node *error = push_node(arena, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
      String8 error_string = push_str8f(arena, "Unexpected reserved symbol \"%S\".", token_string);
      msg_list_push(arena, &msgs, error, MsgKind_Error, error_string);
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [main, main_implicit] tag signifier -> create new tag
    if((work_top->kind == MD_ParseWorkKind_Main || work_top->kind == MD_ParseWorkKind_MainImplicit) &&
       token[0].flags & TokenFlag_Reserved && str8_match(token_string, str8_lit("@"), 0))
    {
      if(token+1 >= tokens_opl ||
         !(token[1].flags & TokenGroup_Label))
      {
        Node *error = push_node(arena, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
        String8 error_string = str8_lit("Tag label expected after @ symbol.");
        msg_list_push(arena, &msgs, error, MsgKind_Error, error_string);
        token += 1;
        goto end_consume;
      }
      else
      {
        String8 tag_name_raw = str8_substr(text, token[1].range);
        String8 tag_name = content_string_from_token_flags_str8(token[1].flags, tag_name_raw);
        Node *node = push_node(arena, NodeKind_Tag, node_flags_from_token_flags(token[1].flags), tag_name, tag_name_raw, token[0].range.min);
        dll_push_back_npz(nil_node(), work_top->first_gathered_tag, work_top->last_gathered_tag, node, next, prev);
        if(token+2 < tokens_opl && token[2].flags & TokenFlag_Reserved && str8_match(str8_substr(text, token[2].range), str8_lit("("), 0))
        {
          token += 3;
          MD_ParseWorkPush(MD_ParseWorkKind_Main, node);
        }
        else
        {
          token += 2;
        }
        goto end_consume;
      }
    }
    
    //- rjf: [main, main_implicit] label -> create new main
    if((work_top->kind == MD_ParseWorkKind_Main || work_top->kind == MD_ParseWorkKind_MainImplicit) &&
       token->flags & TokenGroup_Label)
    {
      String8 node_string_raw = token_string;
      String8 node_string = content_string_from_token_flags_str8(token->flags, node_string_raw);
      NodeFlags flags = node_flags_from_token_flags(token->flags)|work_top->gathered_node_flags;
      work_top->gathered_node_flags = 0;
      Node *node = push_node(arena, NodeKind_Main, flags, node_string, node_string_raw, token[0].range.min);
      node->first_tag = work_top->first_gathered_tag;
      node->last_tag = work_top->last_gathered_tag;
      for(Node *tag = work_top->first_gathered_tag; !node_is_nil(tag); tag = tag->next)
      {
        tag->parent = node;
      }
      work_top->first_gathered_tag = work_top->last_gathered_tag = nil_node();
      node_push_child(work_top->parent, node);
      MD_ParseWorkPush(MD_ParseWorkKind_NodeOptionalFollowUp, node);
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [main] {s, [s, and (s -> create new main
    if(work_top->kind == MD_ParseWorkKind_Main && token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit("{"), 0) ||
        str8_match(token_string, str8_lit("["), 0) ||
        str8_match(token_string, str8_lit("("), 0)))
    {
      NodeFlags flags = node_flags_from_token_flags(token->flags)|work_top->gathered_node_flags;
      flags |=   NodeFlag_HasBraceLeft*!!str8_match(token_string, str8_lit("{"), 0);
      flags |= NodeFlag_HasBracketLeft*!!str8_match(token_string, str8_lit("["), 0);
      flags |=   NodeFlag_HasParenLeft*!!str8_match(token_string, str8_lit("("), 0);
      work_top->gathered_node_flags = 0;
      Node *node = push_node(arena, NodeKind_Main, flags, str8_lit(""), str8_lit(""), token[0].range.min);
      node->first_tag = work_top->first_gathered_tag;
      node->last_tag = work_top->last_gathered_tag;
      for(Node *tag = work_top->first_gathered_tag; !node_is_nil(tag); tag = tag->next)
      {
        tag->parent = node;
      }
      work_top->first_gathered_tag = work_top->last_gathered_tag = nil_node();
      node_push_child(work_top->parent, node);
      MD_ParseWorkPush(MD_ParseWorkKind_Main, node);
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [node children style scan] {s, [s, and (s -> explicitly delimited children
    if(work_top->kind == MD_ParseWorkKind_NodeChildrenStyleScan && token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit("{"), 0) ||
        str8_match(token_string, str8_lit("["), 0) ||
        str8_match(token_string, str8_lit("("), 0)))
    {
      Node *parent = work_top->parent;
      parent->flags |=   NodeFlag_HasBraceLeft*!!str8_match(token_string, str8_lit("{"), 0);
      parent->flags |= NodeFlag_HasBracketLeft*!!str8_match(token_string, str8_lit("["), 0);
      parent->flags |=   NodeFlag_HasParenLeft*!!str8_match(token_string, str8_lit("("), 0);
      MD_ParseWorkPop();
      MD_ParseWorkPush(MD_ParseWorkKind_Main, parent);
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [node children style scan] count newlines
    if(work_top->kind == MD_ParseWorkKind_NodeChildrenStyleScan && token->flags & TokenFlag_Newline)
    {
      work_top->counted_newlines += 1;
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [main_implicit] newline -> pop
    if(work_top->kind == MD_ParseWorkKind_MainImplicit && token->flags & TokenFlag_Newline)
    {
      MD_ParseWorkPop();
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [all but main_implicit] newline -> no-op & inc
    if(work_top->kind != MD_ParseWorkKind_MainImplicit && token->flags & TokenFlag_Newline)
    {
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [node children style scan] anything causing implicit set -> <2 newlines, all good,
    // >=2 newlines, houston we have a problem
    if(work_top->kind == MD_ParseWorkKind_NodeChildrenStyleScan)
    {
      if(work_top->counted_newlines >= 2)
      {
        Node *node = work_top->parent;
        Node *error = push_node(arena, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
        String8 error_string = push_str8f(arena, "More than two newlines following \"%S\", which has implicitly-delimited children, resulting in an empty list of children.", node->string);
        msg_list_push(arena, &msgs, error, MsgKind_Warning, error_string);
        MD_ParseWorkPop();
      }
      else
      {
        Node *parent = work_top->parent;
        MD_ParseWorkPop();
        MD_ParseWorkPush(MD_ParseWorkKind_MainImplicit, parent);
      }
      goto end_consume;
    }
    
    //- rjf: [main] }s, ]s, and )s -> pop
    if(work_top->kind == MD_ParseWorkKind_Main && token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit("}"), 0) ||
        str8_match(token_string, str8_lit("]"), 0) ||
        str8_match(token_string, str8_lit(")"), 0)))
    {
      Node *parent = work_top->parent;
      parent->flags |=   NodeFlag_HasBraceRight*!!str8_match(token_string, str8_lit("}"), 0);
      parent->flags |= NodeFlag_HasBracketRight*!!str8_match(token_string, str8_lit("]"), 0);
      parent->flags |=   NodeFlag_HasParenRight*!!str8_match(token_string, str8_lit(")"), 0);
      MD_ParseWorkPop();
      token += 1;
      goto end_consume;
    }
    
    //- rjf: [main implicit] }s, ]s, and )s -> pop without advancing
    if(work_top->kind == MD_ParseWorkKind_MainImplicit && token->flags & TokenFlag_Reserved &&
       (str8_match(token_string, str8_lit("}"), 0) ||
        str8_match(token_string, str8_lit("]"), 0) ||
        str8_match(token_string, str8_lit(")"), 0)))
    {
      MD_ParseWorkPop();
      goto end_consume;
    }
    
    //- rjf: no consumption -> unexpected token! we don't know what to do with this.
    {
      Node *error = push_node(arena, NodeKind_ErrorMarker, 0, token_string, token_string, token->range.min);
      String8 error_string = push_str8f(arena, "Unexpected \"%S\" token.", token_string);
      msg_list_push(arena, &msgs, error, MsgKind_Error, error_string);
      token += 1;
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

////////////////////////////////
//~ rjf: Bundled Text -> Tree Functions

internal ParseResult
parse_from_text(Arena *arena, String8 filename, String8 text)
{
  TempArena scratch = scratch_begin(&arena, 1);
  TokenizeResult tokenize = tokenize_from_text(scratch.arena, text);
  ParseResult parse = parse_from_text_tokens(arena, filename, text, tokenize.tokens); 
  scratch_end(scratch);
  return parse;
}

////////////////////////////////
//~ rjf: Tree -> Text Functions

internal String8List
debug_string_list_from_tree(Arena *arena, Node *root)
{
  String8List strings = {0};
  {
    char *indentation = "                                                                                                                                ";
    S32 depth = 0;
    for(Node *node = root, *next = nil_node(); !node_is_nil(node); node = next)
    {
      // rjf: get next recursion
      NodeRec rec = node_rec_depth_first_pre(node, root);
      next = rec.next;
      
      // rjf: extract node info
      String8 kind_string = str8_lit("Unknown");
      switch(node->kind)
      {
        default:{}break;
        case NodeKind_File:       {kind_string = str8_lit("File");       }break;
        case NodeKind_ErrorMarker:{kind_string = str8_lit("ErrorMarker");}break;
        case NodeKind_Main:       {kind_string = str8_lit("Main");       }break;
        case NodeKind_Tag:        {kind_string = str8_lit("Tag");        }break;
        case NodeKind_List:       {kind_string = str8_lit("List");       }break;
        case NodeKind_Reference:  {kind_string = str8_lit("Reference");  }break;
      }
      
      // rjf: push node line
      str8_list_pushf(arena, &strings, "%.*s\"%S\" : %S", depth, indentation, node->string, kind_string);
      
      // rjf: children -> open brace
      if(rec.push_count != 0)
      {
        str8_list_pushf(arena, &strings, "%.*s{", depth, indentation);
      }
      
      // rjf: descend
      depth += rec.push_count;
      
      // rjf: popping -> close braces
      for(S32 pop_idx = 0; pop_idx < rec.pop_count; pop_idx += 1)
      {
        str8_list_pushf(arena, &strings, "%.*s}", depth-1-pop_idx, indentation);
      }
      
      // rjf: ascend
      depth -= rec.pop_count;
    }
  }
  return strings;
}
