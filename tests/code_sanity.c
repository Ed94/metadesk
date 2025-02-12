// This is test strictly for the granular (non-generated) version of the library to make sure it operates correctly

// #define MD_LINKED_LIST_PURE_MACRO 1
// #define MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL 1
#include "metadesk.c"

// This program expects to be run from the build directory (where it would be after being built)
#define path_examples          "../examples"
#define path_intro             path_examples "/intro"
#define path_hello_world_mdesk path_intro    "/hello_world.mdesk"

#define text str8_lit

int main()
{
	Context ctx = {0};
	ctx.os_ctx.enable_large_pages = true;
	init(& ctx);

	printf("metadesk: got past init!\n");

	Arena* arena = arena_alloc();
	String8 hello_world_mdesk = os_data_from_file_path(arena, text(path_hello_world_mdesk));

	TokenizeResult lexed  = tokenize_from_text    (arena, hello_world_mdesk);
	ParseResult    parsed = parse_from_text_tokens(arena, text(path_hello_world_mdesk), hello_world_mdesk, lexed.tokens);

	#define str8_list_iter(list, elem) list.first; elem; elem = elem->next
	String8List debug_list = debug_string_list_from_tree(arena, parsed.root);

	printf("Parsed listing:\n");
	for (String8Node* elem = str8_list_iter(debug_list, elem))
	{
		String8 entry = elem->string;
		if (str8_find_needle(entry, 0, str8_lit("\n"), 0) < entry.size) {
			printf("%*s", (int)entry.size, entry.str);
		}
		else {
			printf("%*s\n", (int)entry.size, entry.str);
		}
	}

	SSIZE what = 54;
	String8 test = str8_from(arena, what);

	deinit(& ctx);
}
