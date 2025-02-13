//$ exe //

#include "metadesk.c"

MD_Arena *arena = 0;

static struct
{
    int number_of_tests;
    int number_passed;
}
test_ctx;

static void
begin_test(char* name)
{
    MD_U64 length = md_cstring_length((MD_U8*)name);
    MD_U64 spaces = 25 - length;
    if(spaces < 0)
    {
        spaces = 0;
    }
    printf("\"%s\" %.*s [", name, (int)spaces, "------------------------------");
    test_ctx.number_of_tests = 0;
    test_ctx.number_passed = 0;
}

static void
test_result(MD_B32 result)
{
    test_ctx.number_of_tests += 1;
    test_ctx.number_passed += !!result;
    printf(result ? "." : "X");
    
#if 0
    if(result == 0)
    {
        __debugbreak();
    }
#endif
}

static void
end_test(void)
{
    int spaces = 20 - test_ctx.number_of_tests;
    if(spaces < 0) { spaces = 0; }
    printf("]%.*s ", spaces, "                                      ");
    printf("[%i/%i] %i passed, %i tests, ",
           test_ctx.number_passed, test_ctx.number_of_tests,
           test_ctx.number_passed, test_ctx.number_of_tests);
    if(test_ctx.number_of_tests == test_ctx.number_passed)
    {
        printf("SUCCESS ( )");
    }
    else
    {
        printf("FAILED  (X)");
    }
    printf("\n");
}

#define test(name) for(int _i_ = (begin_test(name), 0); !_i_; _i_ += 1, end_test())

static MD_Node*
make_test_node(MD_NodeKind kind, MD_NodeFlags flags, MD_String8 string)
{
    return md_push_node(arena, kind, flags, string, string, 0);
}

static MD_B32
match_parsed_with_node(MD_Arena* arena, MD_String8 string, MD_Node* tree)
{
	MD_TokenizeResult lexed  = md_tokenize_from_text(arena, string);
	MD_ParseResult    parsed = md_parse_from_text_tokens(arena, md_str8_lit("match_parsed_with_node"), string, lexed.tokens);
	return md_node_match(tree, parsed.root, 0);
}

static MD_B32
token_match(MD_String8 text, MD_Token token, MD_String8 string, MD_TokenFlags flags)
{
	return md_str8_match(string, md_str8_substr(text, token.range), 0) && token.flags == flags;
}

int main(void)
{
    arena = md_arena_alloc(0);
    
    test("Lexer")
    {
        MD_String8 text = md_str8_lit("abc def 123 456 123_456 abc123 123abc +-*");
        MD_Token tokens[100];
        
		MD_TokenizeResult lexed =  md_tokenize_from_text(arena, text);
        
        test_result(token_match(text, tokens[0],  md_str8_lit("abc"),     MD_TokenFlag_Identifier));
        test_result(token_match(text, tokens[1],  md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[2],  md_str8_lit("def"),     MD_TokenFlag_Identifier));
        test_result(token_match(text, tokens[3],  md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[4],  md_str8_lit("123"),     MD_TokenFlag_Numeric));
        test_result(token_match(text, tokens[5],  md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[6],  md_str8_lit("456"),     MD_TokenFlag_Numeric));
        test_result(token_match(text, tokens[7],  md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[8],  md_str8_lit("123_456"), MD_TokenFlag_Numeric));
        test_result(token_match(text, tokens[9],  md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[10], md_str8_lit("abc123"),  MD_TokenFlag_Identifier));
        test_result(token_match(text, tokens[11], md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[12], md_str8_lit("123abc"),  MD_TokenFlag_Numeric));
        test_result(token_match(text, tokens[13], md_str8_lit(" "),       MD_TokenFlag_Whitespace));
        test_result(token_match(text, tokens[14], md_str8_lit("+-*"),     MD_TokenFlag_Symbol));
    }
    
    test("Empty Sets")
    {
        test_result(matched_parsed_with_node(md_str8_lit("{}"), make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""))));
        test_result(matched_parsed_with_node(md_str8_lit("()"), make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""))));
        test_result(matched_parsed_with_node(md_str8_lit("[]"), make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""))));
        test_result(matched_parsed_with_node(md_str8_lit("[)"), make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""))));
        test_result(matched_parsed_with_node(md_str8_lit("(]"), make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""))));
    }
    
    test("Simple Unnamed Sets")
    {
        {
            MD_String8 string = md_str8_lit("{a, b, c}");
            MD_Node*   tree   = make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("a")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("b")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("c")));
            test_result(matched_parsed_with_node(string, tree));
        }
        {
            MD_String8 string = md_str8_lit("(1 2 3 4 5)");
            MD_Node*   tree = make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("1")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("2")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("3")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("4")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("5")));
            test_result(matched_parsed_with_node(string, tree));
        }
        {
            MD_String8 string = md_str8_lit("{a}");
            MD_Node* tree = make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("a")));
            test_result(matched_parsed_with_node(string, tree));
        }
    }
    
    test("Simple Named Sets")
    {
        MD_String8 string = md_str8_lit("simple_set: {a, b, c}");
        MD_Node* tree = make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("simple_set"));
        md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("a")));
        md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("b")));
        md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("c")));
        test_result(matched_parsed_with_node(string, tree));
    }
    
    test("Nested Sets")
    {
        {
            MD_String8 string = md_str8_lit("{a b:{1 2 3} c}");
            MD_Node* tree = make_test_node(MD_NodeKind_Main, 0, md_str8_lit(""));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("a")));
            {
                MD_Node* sub = make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("b"));
                md_node_push_child(sub, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("1")));
                md_node_push_child(sub, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("2")));
                md_node_push_child(sub, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("3")));
                md_node_push_child(tree, sub);
            }
            MD_PushChild(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("c")));
            test_result(matched_parsed_with_node(string, tree));
        }
        
        {
            MD_String8 string = md_str8_lit("foo: { (size: u64) -> *void }");
            MD_Node* tree   = make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("foo"));
            MD_Node* params = make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit(""));
            MD_Node* size   = make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("size"));
            md_node_push_child(size, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("u64")));
            md_node_push_child(params, size);
            md_node_push_child(tree, params);
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("->")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("*")));
            md_node_push_child(tree, make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("void")));
            test_result(matched_parsed_with_node(string, tree));
        }
    }
    
    test("Non-Sets")
    {
        test_result(matched_parsed_with_node(md_str8_lit("foo"), make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("foo"))));
        test_result(matched_parsed_with_node(md_str8_lit("123"), make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("123"))));
        test_result(matched_parsed_with_node(md_str8_lit("+"),   make_test_node(MD_NodeKind_Main, MD_NodeFlag_Identifier, md_str8_lit("+"))));
    }
    
    test("Set Border Flags")
    {
        {
			MD_String8        str   = md_str8_lit("(0, 100)");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, md_str8_zero(), str, lexed.tokens);
            test_result(parse.root->flags & MD_NodeFlag_HasParenLeft &&
                        parse.root->flags & MD_NodeFlag_HasParenRight);
        }
        
        {
			MD_String8        str   = md_str8_lit("(0, 100]");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, md_str8_zero(), str, lexed.tokens);
            test_result(parse.root->flags & MD_NodeFlag_HasParenLeft &&
                        parse.root->flags & MD_NodeFlag_HasBracketRight);
        }
        
        {
			MD_String8        str   = md_str8_lit("[0, 100)");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, fmd_str8_zero(), str, lexed.tokens);
            test_result(parse.root->flags & MD_NodeFlag_HasBracketLeft &&
                        parse.root->flags & MD_NodeFlag_HasParenRight);
        }
        
        {
			MD_String8        str   = md_str8_lit("[0, 100]");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, md_str8_zero(), str, lexed.tokens);
            test_result(parse.root->flags & MD_NodeFlag_HasBracketLeft &&
                        parse.root->flags & MD_NodeFlag_HasBracketRight);
        }
        
        {
			MD_String8        str   = md_str8_lit("{0, 100}");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, md_str8_zero(), str, lexed.tokens);
            test_result(parse.root->flags & MD_NodeFlag_HasBraceLeft &&
                        parse.root->flags & MD_NodeFlag_HasBraceRight);
        }
    }
    
    test("Node Separator Flags")
    {
        {
			MD_String8        str   = md_str8_lit("(a, b)");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, md_str8_zero(), str, lexed.tokens);
            test_result(parse.root->first->flags       & MD_NodeFlag_IsBeforeComma);
            test_result(parse.root->first->next->flags & MD_NodeFlag_IsAfterComma);
        }
        {
			MD_String8        str   = md_str8_lit("(a; b)");
			MD_TokenizeResult lexed = md_tokenize_from_text(arena, str);
            MD_ParseResult    parse = md_parse_from_text_tokens(arena, md_str8_zero(), str, lexed.tokens);
            test_result(parse.root->first->flags       & MD_NodeFlag_IsBeforeSemicolon);
            test_result(parse.root->first->next->flags & MD_NodeFlag_IsAfterSemicolon);
        }
    }
    
    test("Node Text Flags")
    {
        test_result(md_tree_from_string(arena, md_str8_lit("123_456_789"))->flags & MD_NodeFlag_Numeric);
        test_result(md_tree_from_string(arena, md_str8_lit("abc")        )->flags & MD_NodeFlag_Identifier);
        {
            MD_Node* root = md_tree_from_string(arena, md_str8_lit("\"foo\""));
            test_result(root->flags & MD_NodeFlag_StringLiteral &&
                        root->flags & MD_NodeFlag_StringDoubleQuote);
        }
        {
            MD_Node* root = md_tree_from_string(arena, md_str8_lit("'foo'"));
            test_result(root->flags & MD_NodeFlag_StringLiteral &&
                        root->flags & MD_NodeFlag_StringSingleQuote);
        }
        {
            MD_Node* root = md_tree_from_string(arena, md_str8_lit("`foo`"), 0);
            test_result(root->flags & MD_NodeFlag_StringLiteral &&
                        root->flags & MD_NodeFlag_StringTick);
        }
        {
            MD_Node* root = md_tree_from_string(arena, md_str8_lit("\"\"\"foo\"\"\""), 0);
            test_result(parse.root->flags & MD_NodeFlag_StringLiteral     &&
                        parse.root->flags & MD_NodeFlag_StringDoubleQuote &&
                        parse.root->flags & MD_NodeFlag_StringTriplet);
        }
        {
            MD_Node* root = md_tree_from_string(arena, md_str8_lit("'''foo'''"), 0);
            test_result(parse.root->flags & MD_NodeFlag_StringLiteral     &&
                        parse.root->flags & MD_NodeFlag_StringSingleQuote &&
                        parse.root->flags & MD_NodeFlag_StringTriplet);
        }
        {
            MD_Node* root = md_tree_from_string(arena, md_str8_lit("```foo```"), 0);
            test_result(parse.root->flags & MD_NodeFlag_StringLiteral &&
                        parse.root->flags & MD_NodeFlag_StringTick    &&
                        parse.root->flags & MD_NodeFlag_StringTriplet);
        }
    }
    
    test("Style Strings")
    {
        {
            MD_String8 str = md_str8_style(arena, MD_S8Lit("THIS_IS_A_TEST"), MD_IdentifierStyle_UpperCamelCase, MD_S8Lit(" "));
            test_result(MD_S8Match(str, MD_S8Lit("This Is A Test"), 0));
        }
        {
            MD_String8 str = MD_S8Stylize(arena, MD_S8Lit("this_is_a_test"), MD_IdentifierStyle_UpperCamelCase, MD_S8Lit(" "));
            test_result(MD_S8Match(str, MD_S8Lit("This Is A Test"), 0));
        }
        {
            MD_String8 str = MD_S8Stylize(arena, MD_S8Lit("ThisIsATest"), MD_IdentifierStyle_UpperCamelCase, MD_S8Lit(" "));
            test_result(MD_S8Match(str, MD_S8Lit("This Is A Test"), 0));
        }
        {
            MD_String8 str = MD_S8Stylize(arena, MD_S8Lit("Here is another test."), MD_IdentifierStyle_UpperCamelCase, MD_S8Lit(""));
            test_result(MD_S8Match(str, MD_S8Lit("HereIsAnotherTest."), 0));
        }
    }
    
    test("Enum Strings")
    {
        test_result(MD_S8Match(MD_StringFromNodeKind(MD_NodeKind_Main), MD_S8Lit("Main"), 0));
        test_result(MD_S8Match(MD_StringFromNodeKind(MD_NodeKind_Main), MD_S8Lit("Main"), 0));
        MD_String8List list = MD_StringListFromNodeFlags(arena, 
                                                         MD_NodeFlag_StringLiteral |
                                                         MD_NodeFlag_HasParenLeft |
                                                         MD_NodeFlag_IsBeforeSemicolon);
        MD_B32 match = 1;
        for(MD_String8Node *node = list.first; node; node = node->next)
            if(!md_str8_match(node->string, md_str8_lit("StringLiteral"), 0)     &&
               !md_str8_match(node->string, md_str8_lit("HasParenLeft"), 0)       &&
               !md_str8_match(node->string, md_str8_lit("IsBeforeSemicolon"), 0))
            {
                match = 0;
                break;
            }
        }
        test_result(match);
    }
    
    test("Node Comments")
    {
        
        // NOTE(rjf): Pre-Comments:
        {
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("/*foobar*/ (a b c)"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->prev_comment, MD_S8Lit("foobar"), 0));
            }
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("// foobar\n(a b c)"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->prev_comment, MD_S8Lit(" foobar"), 0));
            }
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("// foobar\n\n(a b c)"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->prev_comment, MD_S8Lit(""), 0));
            }
        }
        
        // NOTE(rjf): Post-Comments:
        {
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("(a b c) /*foobar*/"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->next_comment, MD_S8Lit("foobar"), 0));
            }
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("(a b c) // foobar"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->next_comment, MD_S8Lit(" foobar"), 0));
            }
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("(a b c)\n// foobar"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->next_comment, MD_S8Lit(""), 0));
            }
            {
                ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("(a b c)\n\n// foobar"), 0);
                test_result(parse.node->kind == NodeKind_Main &&
                           MD_S8Match(parse.node->next_comment, MD_S8Lit(""), 0));
            }
        }
    }
    
    test("Errors")
    {
        struct { char *s; MD_u64 columns[2]; } tests[] = {
            {"{", {1}},
            {"}", {1}},
            {"'", {1}},
            {"a:'''\nmulti-line text literal", {3}},
            {"/* foo", {1}},
            {"label: {1, 2, 3} /* /* unterminated comment */", {18}},
            {"{a,,#b,}", {4, 5}},
            {"foo""\x80""bar", {4}},
        };
        
        int max_error_count = MD_ArrayCount(tests[0].columns);
        
        for(int i_test = 0; i_test < MD_ArrayCount(tests); ++i_test)
        {
            ParseResult parse = MD_ParseWholeString(arena, MD_S8Lit("test.mdesk"), MD_S8CString(tests[i_test].s));
            
            MD_b32 columns_match = 1;
            {
                MD_Message *e = parse.errors.first;
                for(int i_error = 0; i_error < max_error_count && tests[i_test].columns[i_error]; ++i_error)
                {
                    if(!e || MD_CodeLocFromNode(e->node).column != tests[i_test].columns[i_error])
                    {
                        columns_match = 0;
                        break;
                    }
                    e = e->next;
                }
                
                if(e && e->next)
                {
                    columns_match = 0;
                }
            }
            test_result(columns_match);
        }
        
        {
            ParseResult parse = MD_ParseWholeFile(arena, MD_S8Lit("__does_not_exist.mdesk"));
            test_result(parse.node->kind == NodeKind_File && parse.errors.first != 0);
        }
        
    }
    
    test("Hash maps")
    {
        MD_String8 key_strings[] = 
        {
            MD_S8LitComp("\xed\x80\x73\x71\x78\xba\xff\xd6\x87\x83\xcd\x20\x28\xf7\x1c\xc1\x5f\xca\x98\x9c\x5a\xab\x0c\xae\x9a\x60\x57\x03\xeb\x1f\xde\x99"),
            MD_S8LitComp("\x4c\x80\xb7\x8b\xbf\x65\x5a\x4b\xc1\x2a\xc3\x5f\xe1\x66\xfb\x0d\x72\x83\x1c\x63\xba\xb5\x97\x02\x3f\x6a\xe0\x2a\x1b\x82\x07\x76"),
            MD_S8LitComp("\xd8\xfd\x11\x4b\x04\xdf\xe5\x20\x5b\xd6\x4f\x87\x00\x70\x6a\xc8\xde\xed\xc7\x79\xdb\x87\x24\x36\xa8\x7a\x31\x41\x00\x57\xbd\x8d"),
        };
        
        MD_MapKey keys[MD_ArrayCount(key_strings)*2];
        for (MD_u64 i = 0; i < MD_ArrayCount(key_strings); i += 1)
        {
            keys[i] = MD_MapKeyStr(key_strings[i]);
        }
        for (MD_u64 i = MD_ArrayCount(key_strings); i < MD_ArrayCount(keys); i += 1)
        {
            keys[i] = MD_MapKeyPtr((void *)i);
        }
        
        {
            MD_Map map = MD_MapMake(arena);
            for (MD_u64 i = 0; i < MD_ArrayCount(keys); i += 1)
            {
                MD_MapInsert(arena, &map, keys[i], (void *)i);
            }
            for (MD_u64 i = 0; i < MD_ArrayCount(keys); i += 1)
            {
                MD_MapSlot *slot = MD_MapLookup(&map, keys[i]);
                test_result(slot && slot->val == (void *)i);
            }
            for (MD_u64 i = 0; i < MD_ArrayCount(keys); i += 1)
            {
                MD_MapOverwrite(arena, &map, keys[i], (void *)(i + 10));
            }
            for (MD_u64 i = 0; i < MD_ArrayCount(keys); i += 1)
            {
                MD_MapSlot *slot = MD_MapLookup(&map, keys[i]);
                test_result(slot && slot->val == (void *)(i + 10));
            }
        }
    }
    
    test("String Inner & Outer")
    {
        MD_String8 samples[6] = {
            MD_S8LitComp("'foo-bar'"),
            MD_S8LitComp("'''foo-bar'''"),
            MD_S8LitComp("\"foo-bar\""),
            MD_S8LitComp("\"\"\"foo-bar\"\"\""),
            MD_S8LitComp("`foo-bar`"),
            MD_S8LitComp("```foo-bar```"),
        };
        
        Node *nodes[MD_ArrayCount(samples)];
        for (int i = 0; i < MD_ArrayCount(samples); i += 1)
        {
            ParseResult result = MD_ParseOneNode(arena, samples[i], 0);
            nodes[i] = result.node;
        }
        
        test_result(MD_S8Match(nodes[0]->string, MD_S8Lit("foo-bar"), 0));
        test_result(MD_S8Match(nodes[1]->string, MD_S8Lit("foo-bar"), 0));
        test_result(MD_S8Match(nodes[2]->string, MD_S8Lit("foo-bar"), 0));
        test_result(MD_S8Match(nodes[3]->string, MD_S8Lit("foo-bar"), 0));
        test_result(MD_S8Match(nodes[4]->string, MD_S8Lit("foo-bar"), 0));
        test_result(MD_S8Match(nodes[5]->string, MD_S8Lit("foo-bar"), 0));
        
        test_result(MD_S8Match(nodes[0]->raw_string, samples[0], 0));
        test_result(MD_S8Match(nodes[1]->raw_string, samples[1], 0));
        test_result(MD_S8Match(nodes[2]->raw_string, samples[2], 0));
        test_result(MD_S8Match(nodes[3]->raw_string, samples[3], 0));
        test_result(MD_S8Match(nodes[4]->raw_string, samples[4], 0));
        test_result(MD_S8Match(nodes[5]->raw_string, samples[5], 0));
    }
    
    test("String escaping")
    {
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("`\\``"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit("\\`"), 0));
        }
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("``` \\``` ```"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit(" \\``` "), 0));
        }
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("`````\\````"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit("``\\`"), 0));
        }
        
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("`\\'`"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit("\\'"), 0));
        }
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("''' \\''' '''"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit(" \\''' "), 0));
        }
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("'''''\\''''"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit("''\\'"), 0));
        }
        
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("`\\\"`"), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit("\\\""), 0));
        }
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("\"\"\" \\\"\"\" \"\"\""), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit(" \\\"\"\" "), 0));
        }
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("\"\"\"\"\"\\\"\"\"\""), 0);
            test_result(MD_S8Match(parse.node->string, MD_S8Lit("\"\"\\\""), 0));
        }
    }
    
    test("Node-With-Flags Seeking")
    {
        
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("foo:{x y z; a b c}"), 0);
            Node *node = parse.node;
            Node *group_first = node->first_child;
            Node *group_opl = MD_FirstNodeWithFlags(group_first->next, NodeFlag_IsAfterSemicolon);
            
            test_result(MD_S8Match(group_first->string,                    MD_S8Lit("x"), 0));
            test_result(MD_S8Match(group_first->next->string,              MD_S8Lit("y"), 0));
            test_result(MD_S8Match(group_first->next->next->string,        MD_S8Lit("z"), 0));
            test_result(group_opl == group_first->next->next->next);
            
            group_first = group_opl;
            group_opl = MD_FirstNodeWithFlags(group_first->next, NodeFlag_IsAfterSemicolon);
            
            test_result(MD_S8Match(group_first->string,                    MD_S8Lit("a"), 0));
            test_result(MD_S8Match(group_first->next->string,              MD_S8Lit("b"), 0));
            test_result(MD_S8Match(group_first->next->next->string,        MD_S8Lit("c"), 0));
            test_result(group_opl == group_first->next->next->next);
        }
        
        {
            ParseResult parse = MD_ParseOneNode(arena, MD_S8Lit("foo:{a b c , d e f , g h i}"), 0);
            Node *node = parse.node;
            Node *group_first = 0;
            Node *group_opl = 0;
            
            group_first = node->first_child;
            group_opl = MD_FirstNodeWithFlags(group_first->next, NodeFlag_IsAfterComma);
            test_result(MD_S8Match(group_first->string,                    MD_S8Lit("a"), 0));
            test_result(MD_S8Match(group_first->next->string,              MD_S8Lit("b"), 0));
            test_result(MD_S8Match(group_first->next->next->string,        MD_S8Lit("c"), 0));
            test_result(group_opl == group_first->next->next->next);
            
            group_first = group_opl;
            group_opl = MD_FirstNodeWithFlags(group_first->next, NodeFlag_IsAfterComma);
            test_result(MD_S8Match(group_first->string,                    MD_S8Lit("d"), 0));
            test_result(MD_S8Match(group_first->next->string,              MD_S8Lit("e"), 0));
            test_result(MD_S8Match(group_first->next->next->string,        MD_S8Lit("f"), 0));
            test_result(group_opl == group_first->next->next->next);
            
            group_first = group_opl;
            group_opl = MD_FirstNodeWithFlags(group_first->next, NodeFlag_IsAfterComma);
            test_result(MD_S8Match(group_first->string,                    MD_S8Lit("g"), 0));
            test_result(MD_S8Match(group_first->next->string,              MD_S8Lit("h"), 0));
            test_result(MD_S8Match(group_first->next->next->string,        MD_S8Lit("i"), 0));
            test_result(group_opl == group_first->next->next->next);
        }
        
    }
    
    test("Unscoped Subtleties")
    {
        MD_String8 file_name = MD_S8Lit("raw_text");
        
        // nested unscoped sets with following children
        {
            MD_String8 text = MD_S8Lit("a:\nb:\nc\nd");
            
            Node *tree = MD_MakeNode(arena, NodeKind_File, file_name, text, 0);
            Node *a = MakeTestNode(NodeKind_Main, MD_S8Lit("a"));
            Node *b = MakeTestNode(NodeKind_Main, MD_S8Lit("b"));
            Node *c = MakeTestNode(NodeKind_Main, MD_S8Lit("c"));
            Node *d = MakeTestNode(NodeKind_Main, MD_S8Lit("d"));
            MD_PushChild(a, b);
            MD_PushChild(b, c);
            MD_PushChild(tree, a);
            MD_PushChild(tree, d);
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first == 0);
            test_result(MD_NodeDeepMatch(tree, result.node, 0));
        }
        
        // finished unscoped set
        {
            MD_String8 text = MD_S8Lit("a:\nb:\nc");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first == 0);
            test_result(result.node->first_child == result.node->last_child);
        }
        
        // unfinished unscoped set
        {
            MD_String8 text = MD_S8Lit("a:\nb:\n\n");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first != 0);
        }
        {
            MD_String8 text = MD_S8Lit("a:\nb:\n");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first != 0);
        }
        {
            MD_String8 text = MD_S8Lit("a:\nb:");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first != 0);
        }
        
        // labeled scoped set in unscoped set
        {
            MD_String8 text = MD_S8Lit("a: b: {\nx\n} c");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first == 0);
            test_result(MD_ChildCountFromNode(result.node) == 1);
            test_result(MD_ChildCountFromNode(result.node->first_child) == 2);
        }
        {
            MD_String8 text = MD_S8Lit("a: b: {\nx\n}\nc");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first == 0);
            test_result(MD_ChildCountFromNode(result.node) == 2);
            test_result(MD_ChildCountFromNode(result.node->first_child) == 1);
        }
        
        // scoped set is not unscoped
        {
            MD_String8 text = MD_S8Lit("a: {\nx\ny\n} c");
            ParseResult result = MD_ParseWholeString(arena, file_name, text);
            test_result(result.errors.first == 0);
            test_result(result.node->first_child != result.node->last_child);
        }
    }
    
    test("Tags")
    {
        MD_String8 file_name = MD_S8Lit("raw_text");
        
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("@foo bar"));
            test_result(MD_NodeHasTag(result.node->first_child, MD_S8Lit("foo"), 0));
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("@+ bar"));
            test_result(MD_NodeHasTag(result.node->first_child, MD_S8Lit("+"), 0));
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("@'a b c' bar"));
            test_result(MD_NodeHasTag(result.node->first_child, MD_S8Lit("a b c"), 0));
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("@100 bar"));
            test_result(MD_NodeHasTag(result.node->first_child, MD_S8Lit("100"), 0));
        }
    }
    
    test("Tagged & Unlabeled")
    {
        MD_String8 file_name = MD_S8Lit("raw_text");
        
        // tagged in scoped set always legal
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("foo:{@tag {bar}}\n"));
            test_result(result.errors.first == 0);
        }
        
        // tagged label in unscoped set legal
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("foo:@tag bar\n"));
            test_result(result.errors.first == 0);
        }
        
        // unlabeled scoped set in unscoped set illegal
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("foo:bar {bar}\n"));
            test_result(result.errors.first != 0);
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("foo:bar @tag {bar}\n"));
            test_result(result.errors.first != 0);
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, MD_S8Lit("foo:@tag {bar}\n"));
            test_result(result.errors.first != 0);
        }
    }
    
    test("Integer Lexing")
    {
        MD_String8 file_name = MD_S8Lit("raw_text");
        
        MD_String8 test_strings[] = {
            MD_S8LitComp("0765"),
            MD_S8LitComp("0xABC"),
            MD_S8LitComp("0x123"),
            MD_S8LitComp("0b010"),
        };
        
        MD_String8 *string = test_strings;
        for (int i = 0; i < MD_ArrayCount(test_strings); i += 1, string += 1)
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, *string);
            test_result((result.errors.first == 0) &&
                       (result.node->first_child == result.node->last_child) &&
                       (result.node->first_child->flags & NodeFlag_Numeric));
        }
    }
    
    test("Float Lexing")
    {
        MD_String8 file_name = MD_S8Lit("raw_text");
        
        MD_String8 test_strings[] = {
            MD_S8LitComp("0"),
            MD_S8LitComp("1"),
            MD_S8LitComp("0.5"),
            MD_S8LitComp("1e2"),
            MD_S8LitComp("1e+2"),
            MD_S8LitComp("1e-2"),
            MD_S8LitComp("1.5e2"),
            MD_S8LitComp("1.5e+2"),
            MD_S8LitComp("1.5e-2"),
        };
        
        MD_String8 *string = test_strings;
        for (int i = 0; i < MD_ArrayCount(test_strings); i += 1, string += 1)
        {
            ParseResult result = MD_ParseWholeString(arena, file_name, *string);
            test_result((result.errors.first == 0) &&
                       (result.node->first_child == result.node->last_child) &&
                       (result.node->first_child->flags & NodeFlag_Numeric));
        }
    }
    
    test("Labels are Not Reserved")
    {
        MD_String8 file_name = MD_S8Lit("raw_text");
        
        {
            ParseResult result = MD_ParseWholeString(arena, file_name,
                                                        MD_S8Lit("foo: '(' )"));
														test_result(result.errors.first != 0);
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name,
                                                        MD_S8Lit("foo ':' ( )"));
            test_result(result.errors.first == 0);
            test_result(MD_ChildCountFromNode(result.node) == 3);
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name,
                                                        MD_S8Lit("'@'bar foo"));
            test_result(result.errors.first == 0);
            test_result(MD_ChildCountFromNode(result.node) == 3);
        }
        {
            ParseResult result = MD_ParseWholeString(arena, file_name,
                                                        MD_S8Lit("foo: '(' ')'"));
            test_result(result.errors.first == 0);
            test_result(MD_ChildCountFromNode(result.node) == 1);
            test_result(MD_ChildCountFromNode(result.node->first_child) == 2);
        }
    }
    
    test("Debug Strings")
    {
        {
            MD_String8 code     = MD_S8Lit("@foo @bar @baz a: { b c d e f }");
            MD_String8 expected = MD_S8Lit("@foo\n@bar\n@baz\na:\n{\n b,\n c,\n d,\n e,\n f,\n}");
            ParseResult parse = MD_ParseOneNode(arena, code, 0);
            MD_String8List actual_strings = {0};
            MD_DebugDumpFromNode(arena, &actual_strings, parse.node,
                                 0, MD_S8Lit(" "), MD_GenerateFlags_Tree);
            MD_String8 actual = MD_S8ListJoin(arena, actual_strings, 0);
            test_result(MD_S8Match(expected, actual, 0));
        }
        {
            MD_String8 code     = MD_S8Lit("@foo(1, 2, 3) a: { x y }");
            MD_String8 expected = MD_S8Lit("@foo(1,\n     2,\n     3)\na:\n{\n x,\n y,\n}");
            ParseResult parse = MD_ParseOneNode(arena, code, 0);
            MD_String8List actual_strings = {0};
            MD_DebugDumpFromNode(arena, &actual_strings, parse.node,
                                 0, MD_S8Lit(" "), MD_GenerateFlags_Tree);
            MD_String8 actual = MD_S8ListJoin(arena, actual_strings, 0);
            test_result(MD_S8Match(expected, actual, 0));
        }
        {
            MD_String8 code     = MD_S8Lit("// foo\na");
            MD_String8 expected = MD_S8Lit("/*\n foo\n*/\na");
            ParseResult parse = MD_ParseOneNode(arena, code, 0);
            MD_String8List actual_strings = {0};
            MD_DebugDumpFromNode(arena, &actual_strings, parse.node, 0, MD_S8Lit(" "),
                                 MD_GenerateFlags_Tree|MD_GenerateFlag_Comments);
            MD_String8 actual = MD_S8ListJoin(arena, actual_strings, 0);
            test_result(MD_S8Match(expected, actual, 0));
        }
        {
            MD_String8 code = MD_S8Lit("@foo @bar @baz a: { b c d e f }");
            ParseResult parse1 = MD_ParseOneNode(arena, code, 0);
            MD_String8List reconstruction_strs = {0};
            MD_ReconstructionFromNode(arena, &reconstruction_strs, parse1.node, 0, MD_S8Lit(" "));
            MD_String8 reconstruction = MD_S8ListJoin(arena, reconstruction_strs, 0);
            ParseResult parse2 = MD_ParseOneNode(arena, reconstruction, 0);
            test_result(MD_NodeDeepMatch(parse1.node, parse2.node, MD_NodeMatchFlag_TagArguments|MD_NodeMatchFlag_NodeFlags));
        }
        {
            MD_String8 code = MD_S8Lit("@foo(x: y: z) @bar(a: b: c) @baz(1: 2: 3) abcdefg: { b: 4, c d: 5; e; f, }");
            ParseResult parse1 = MD_ParseOneNode(arena, code, 0);
            MD_String8List reconstruction_strs = {0};
            MD_ReconstructionFromNode(arena, &reconstruction_strs, parse1.node, 0, MD_S8Lit(" "));
            MD_String8 reconstruction = MD_S8ListJoin(arena, reconstruction_strs, 0);
            ParseResult parse2 = MD_ParseOneNode(arena, reconstruction, 0);
            test_result(MD_NodeDeepMatch(parse1.node, parse2.node, MD_NodeMatchFlag_TagArguments|MD_NodeMatchFlag_NodeFlags));
        }
        {
            MD_String8 code = MD_S8Lit("@foo(x: y: z)\n"
                                       "@bar(a: b: c)\n"
                                       "@baz(1: 2: 3)\n"
                                       "abcdefg:\n"
                                       "{\n"
                                       "  b: 4,\n"
                                       "  c\n"
                                       "  d: 5;\n"
                                       "  e;\n"
                                       "  f,\n"
                                       "}\n");
            ParseResult parse1 = MD_ParseOneNode(arena, code, 0);
            MD_String8List reconstruction_strs = {0};
            MD_ReconstructionFromNode(arena, &reconstruction_strs, parse1.node, 0, MD_S8Lit(" "));
            MD_String8 reconstruction = MD_S8ListJoin(arena, reconstruction_strs, 0);
            ParseResult parse2 = MD_ParseOneNode(arena, reconstruction, 0);
            test_result(MD_NodeDeepMatch(parse1.node, parse2.node, MD_NodeMatchFlag_TagArguments|MD_NodeMatchFlag_NodeFlags));
        }
    }
    
    return 0;
}
