#ifdef INTELLISENSE_DIRECTIVES
#	include "debug.h"
#	include "time.h"
#endif

DateTime
date_time_from_unix_time(U64 unix_time)
{
	DateTime date = {0};
	date.year = 1970;
	date.day  = 1 +  (unix_time / 86400);
	date.sec  = (U32) unix_time % 60;
	date.min  = (U32)(unix_time / 60)   % 60;
	date.hour = (U32)(unix_time / 3600) % 24;

	for(;;)
	{
		for(date.month = 0; date.month < 12; ++date.month)
		{
			U64 c = 0;
			switch(date.month)
			{
				case Month_Jan: c = 31; break;
				case Month_Feb:
				{
					if((date.year % 4 == 0) && ((date.year % 100) != 0 || (date.year % 400) == 0))
					{
						c = 29;
					}
					else
					{
						c = 28;
					}
				} break;
				case Month_Mar: c = 31; break;
				case Month_Apr: c = 30; break;
				case Month_May: c = 31; break;
				case Month_Jun: c = 30; break;
				case Month_Jul: c = 31; break;
				case Month_Aug: c = 31; break;
				case Month_Sep: c = 30; break;
				case Month_Oct: c = 31; break;
				case Month_Nov: c = 30; break;
				case Month_Dec: c = 31; break;
				default: invalid_path;
			}
			if(date.day <= c)
			{
				goto exit;
			}
			date.day -= c;
		}
		++date.year;
	}
	exit:;

	return date;
}
