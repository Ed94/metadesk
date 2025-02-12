#ifdef INTELLISENSE_DIRECTIVES
#	include "debug.h"
#	include "time.h"
#endif

MD_DateTime
md_date_time_from_unix_time(MD_U64 unix_time)
{
	MD_DateTime date = {0};
	date.year = 1970;
	date.day  = 1 +  (unix_time / 86400);
	date.sec  = (MD_U32) unix_time % 60;
	date.min  = (MD_U32)(unix_time / 60)   % 60;
	date.hour = (MD_U32)(unix_time / 3600) % 24;

	for(;;)
	{
		for(date.month = 0; date.month < 12; ++date.month)
		{
			MD_U64 c = 0;
			switch(date.month)
			{
				case MD_Month_Jan: c = 31; break;
				case MD_Month_Feb:
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
				case MD_Month_Mar: c = 31; break;
				case MD_Month_Apr: c = 30; break;
				case MD_Month_May: c = 31; break;
				case MD_Month_Jun: c = 30; break;
				case MD_Month_Jul: c = 31; break;
				case MD_Month_Aug: c = 31; break;
				case MD_Month_Sep: c = 30; break;
				case MD_Month_Oct: c = 31; break;
				case MD_Month_Nov: c = 30; break;
				case MD_Month_Dec: c = 31; break;
				default: md_invalid_path;
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
