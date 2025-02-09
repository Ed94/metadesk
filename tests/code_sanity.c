// This is test strictly for the granular (non-generated) version of the library to make sure it operates correctly

// #define MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL
#include "metadesk.c"

// This program expects to be run from the build directory (where it would be after being built)
#define path_examples           "../examples"
#define path_intro              path_examples "/intro"
#define path_hello_world_medesk path_intro    "/hello_world.mdesk"

int main()
{
	Context ctx = {0};
	ctx.os_ctx.enable_large_pages = true;
	init(& ctx);



	printf("metadesk: got past init!");

	// deinit(& ctx);
}
