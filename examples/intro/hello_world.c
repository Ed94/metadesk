/* 
** Example: hello world
*/

//~ includes and globals //////////////////////////////////////////////////////

#include "gen_c11/gen/metadesk.c"

//~ main //////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
	MD_Context ctx = {0};
	init(& ctx);

    // setup the global arena
    // arena = MD_ArenaAlloc();
    
    // parse a string
    String8 name        = md_s8_lit("<name>");
    String8 hello_world = md_s8_lit("hello world");
    ParseResult parse   = md_parse_whole_string(arena, name, hello_world);
    
    // print the results
    // MD_PrintDebugDumpFromNode(stdout, parse.node, MD_GenerateFlags_All);
	md_print_debug_dump_from_node(stdout, parse.node, MD_GenerateFlags_All);
}
