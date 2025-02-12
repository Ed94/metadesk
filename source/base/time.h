#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ allen: Time

typedef enum MD_WeekDay MD_WeekDay;
enum MD_WeekDay
{
	MD_WeekDay_Sun,
	MD_WeekDay_Mon,
	MD_WeekDay_Tue,
	MD_WeekDay_Wed,
	MD_WeekDay_Thu,
	MD_WeekDay_Fri,
	MD_WeekDay_Sat,
	MD_WeekDay_COUNT,
};

typedef enum MD_Month MD_Month;
enum MD_Month
{
	MD_Month_Jan,
	MD_Month_Feb,
	MD_Month_Mar,
	MD_Month_Apr,
	MD_Month_May,
	MD_Month_Jun,
	MD_Month_Jul,
	MD_Month_Aug,
	MD_Month_Sep,
	MD_Month_Oct,
	MD_Month_Nov,
	MD_Month_Dec,
	MD_Month_COUNT,
};

typedef struct MD_DateTime MD_DateTime;
struct MD_DateTime
{
	MD_U16 micro_sec; // [0,999]
	MD_U16 msec;      // [0,999]
	MD_U16 sec;       // [0,60]
	MD_U16 md_min;       // [0,59]
	MD_U16 hour;      // [0,24]
	MD_U16 day;       // [0,30]
	union
	{
		MD_WeekDay week_day;
		MD_U32     wday;
	};
	union
	{
		MD_Month month;
		MD_U32   mon;
	};
	MD_U32 year; // 1 = 1 CE, 0 = 1 BC
};

typedef MD_U64 MD_DenseTime;

////////////////////////////////
//~ rjf: Time Functions

MD_DenseTime md_dense_time_from_date_time   (MD_DateTime  date_time);
MD_DateTime  md_date_time_from_dense_time   (MD_DenseTime time);
MD_DateTime  md_date_time_from_micro_seconds(MD_U64       time);
MD_DateTime  md_date_time_from_unix_time    (MD_U64       unix_time);

////////////////////////////////
//~ rjf: Time Functions

inline MD_DenseTime
md_dense_time_from_date_time(MD_DateTime date_time) {
	MD_DenseTime result = 0;
	result += date_time.year; result *= 12;
	result += date_time.mon;  result *= 31;
	result += date_time.day;  result *= 24;
	result += date_time.hour; result *= 60;
	result += date_time.md_min;  result *= 61;
	result += date_time.sec;  result *= 1000;
	result += date_time.msec; 
	return(result);
}

inline MD_DateTime
md_date_time_from_dense_time(MD_DenseTime time) {
	MD_DateTime result = {0};
	result.msec = time % 1000; time /= 1000;
	result.sec  = time % 61;   time /= 61;
	result.md_min  = time % 60;   time /= 60;
	result.hour = time % 24;   time /= 24;
	result.day  = time % 31;   time /= 31;
	result.mon  = time % 12;   time /= 12;
	md_assert(time <= MD_MAX_U32);
	result.year = (MD_U32)time;
	return(result);
}

inline MD_DateTime
md_date_time_from_micro_seconds(MD_U64 time){
	MD_DateTime result = {0};
	result.micro_sec = time % 1000; time /= 1000;
	result.msec      = time % 1000; time /= 1000;
	result.sec       = time % 60;   time /= 60;
	result.md_min       = time % 60;   time /= 60; 
	result.hour      = time % 24;   time /= 24;
	result.day       = time % 31;   time /= 31;
	result.mon       = time % 12;   time /= 12;
	md_assert(time <= MD_MAX_U32);
	result.year      = (MD_U32)time;
	return(result);
}
