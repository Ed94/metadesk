#ifdef INTELLISENSE_DIRECTIVES
#	pragma once
#	include "base_types.h"
#endif

////////////////////////////////
//~ allen: Time

typedef enum WeekDay WeekDay;
enum WeekDay
{
	WeekDay_Sun,
	WeekDay_Mon,
	WeekDay_Tue,
	WeekDay_Wed,
	WeekDay_Thu,
	WeekDay_Fri,
	WeekDay_Sat,
	WeekDay_COUNT,
};

typedef enum Month Month;
enum Month
{
	Month_Jan,
	Month_Feb,
	Month_Mar,
	Month_Apr,
	Month_May,
	Month_Jun,
	Month_Jul,
	Month_Aug,
	Month_Sep,
	Month_Oct,
	Month_Nov,
	Month_Dec,
	Month_COUNT,
};

typedef struct DateTime DateTime;
struct DateTime
{
	U16 micro_sec; // [0,999]
	U16 msec;      // [0,999]
	U16 sec;       // [0,60]
	U16 min;       // [0,59]
	U16 hour;      // [0,24]
	U16 day;       // [0,30]
	union
	{
		WeekDay week_day;
		U32     wday;
	};
	union
	{
		Month month;
		U32   mon;
	};
	U32 year; // 1 = 1 CE, 0 = 1 BC
};

typedef U64 DenseTime;

////////////////////////////////
//~ rjf: Time Functions

DenseTime dense_time_from_date_time   (DateTime  date_time);
DateTime  date_time_from_dense_time   (DenseTime time);
DateTime  date_time_from_micro_seconds(U64       time);
DateTime  date_time_from_unix_time    (U64       unix_time);

////////////////////////////////
//~ rjf: Time Functions

inline DenseTime
dense_time_from_date_time(DateTime date_time) {
	DenseTime result = 0;
	result += date_time.year; result *= 12;
	result += date_time.mon;  result *= 31;
	result += date_time.day;  result *= 24;
	result += date_time.hour; result *= 60;
	result += date_time.min;  result *= 61;
	result += date_time.sec;  result *= 1000;
	result += date_time.msec; 
	return(result);
}

inline DateTime
date_time_from_dense_time(DenseTime time) {
	DateTime result = {0};
	result.msec = time % 1000; time /= 1000;
	result.sec  = time % 61;   time /= 61;
	result.min  = time % 60;   time /= 60;
	result.hour = time % 24;   time /= 24;
	result.day  = time % 31;   time /= 31;
	result.mon  = time % 12;   time /= 12;
	assert(time <= MAX_U32);
	result.year = (U32)time;
	return(result);
}

inline DateTime
date_time_from_micro_seconds(U64 time){
	DateTime result = {0};
	result.micro_sec = time % 1000; time /= 1000;
	result.msec      = time % 1000; time /= 1000;
	result.sec       = time % 60;   time /= 60;
	result.min       = time % 60;   time /= 60; 
	result.hour      = time % 24;   time /= 24;
	result.day       = time % 31;   time /= 31;
	result.mon       = time % 12;   time /= 12;
	assert(time <= MAX_U32);
	result.year      = (U32)time;
	return(result);
}
