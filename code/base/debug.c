#ifdef INTELLISENSE_DIRECTIVES
#	include "strings.h"
#	include "debug.h"
#endif

void assert_handler( char const* condition, char const* file, char const* function, S32 line, char const* msg, ... )
{
	// TODO(Ed): Change this to metadesks's procs
	_printf_err( "%s - %s:(%d): Assert Failure: ", file, function, line );

	if ( condition )
		// TODO(Ed): Change this to metadesks's procs
		_printf_err( "`%s` \n", condition );

	if ( msg )
	{
		va_list va;
		va_start( va, msg );
		_printf_err_va( msg, va );
		va_end( va );
	}

	// TODO(Ed): Change this to metadesks's procs
	_printf_err( "%s", "\n" );
}
