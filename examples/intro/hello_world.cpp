/* 
** Example: hello world
*/

//~ includes and globals //////////////////////////////////////////////////////

#include "gen_cpp17/gen/metadesk.cpp"

//~ main //////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv)
{
	using md;
	Context ctx = {0};
	init(& ctx);

    // setup the global arena
    // arena = MD_ArenaAlloc();
    
    // parse a string
    String8 name        = s8_lit("<name>");
    String8 hello_world = s8_lit("hello world");
    ParseResult parse   = parse_whole_string(arena, name, hello_world);
    
    // print the results
    // MD_PrintDebugDumpFromNode(stdout, parse.node, MD_GenerateFlags_All);
	print_debug_dump_from_node(stdout, parse.node, MD_GenerateFlags_All);
}
