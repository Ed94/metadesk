#ifdef INTELLISENSE_DIRECTIVES
#	include "strings.h"
#	include "debug.h"
#	include "platform.c"
#endif

#define md__printf_err( fmt, ... )   fprintf( stderr, fmt, __VA_ARGS__ )
#define md__printf_err_va( fmt, va ) vfprintf( stderr, fmt, va )

void md_assert_handler( char const* condition, char const* file, char const* function, MD_S32 line, char const* msg, ... )
{
	md__printf_err( "%s - %s:(%d): Assert Failure: ", file, function, line );

	if ( condition )
		md__printf_err( "`%s` \n", condition );

	if ( msg )
	{
		va_list va;
		va_start( va, msg );
		md__printf_err_va( msg, va );
		va_end( va );
	}

	md__printf_err( "%s", "\n" );
}
