// This is tests both the segregated and single-header libraris

#if TEST_SINGLEHEADER
#define MD_IMPLEMENTATION
#include "metadesk_singleheader.h"
#else
#include "metadesk_deps.c"
#include "metadesk.c"
#endif

// This program expects to be run from the build directory (where it would be after being built)
#define path_root
#define path_examples          path_root     "examples/"
#define path_intro             path_examples "intro/"
#define path_hello_world_mdesk path_intro    "hello_world.mdesk"

#define lit md_str8_lit

int main()
{
	MD_Context ctx = {0};
	ctx.os_ctx.enable_large_pages = true;
	md_init(& ctx);

	printf("metadesk: got past init!\n");

	MD_Arena* arena = md_arena_alloc();
	MD_String8 hello_world_mdesk = md_os_data_from_file_path(arena, lit(path_hello_world_mdesk));

	MD_TokenizeResult lexed  = md_tokenize_from_text    (arena, hello_world_mdesk);
	MD_ParseResult    parsed = md_parse_from_text_tokens(arena, lit(path_hello_world_mdesk), hello_world_mdesk, lexed.tokens);

	#define md_str8_list_iter(list, elem) list.first; elem; elem = elem->next
	MD_String8List debug_list = md_debug_string_list_from_tree(arena, parsed.root);

	printf("Parsed listing:\n");
	for (MD_String8Node* elem = md_str8_list_iter(debug_list, elem))
	{
		MD_String8 entry = elem->string;
		if (md_str8_find_needle(entry, 0, lit("\n"), 0) < entry.size) {
			printf("%*s", (int)entry.size, entry.str);
		}
		else {
			printf("%*s\n", (int)entry.size, entry.str);
		}
	}

	md_deinit(& ctx);
}
