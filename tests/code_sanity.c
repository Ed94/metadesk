// This is test strictly for the granular (non-generated) version of the library to make sure it operates correctly

#include "metadesk.c"

int main()
{
	Context ctx = {0};
	ctx.os_ctx.enable_large_pages = true;
	init(& ctx);

	printf("metadesk: got past init!");

	deinit(& ctx);
}
