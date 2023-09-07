#ifndef _QTIME_H
#define _QTIME_H



#include <inttypes.h>
#include <time.h>
#include <sys/time.h>

#include "qcp.h"

#define MS_TO_US(ms)            ((ms) * 1000)
#define SEC_TO_MS(sec)          ((sec) * 1000)
#define MINUTE_TO_SEC(minute)   ((minute) * 60)
#define HOUR_TO_MINUTE(hour)    ((hour) * 60)


#define MS_TO_SEC(ms)           ((unsigned int)((ms) / 1000))
#define US_TO_MS(us)            ((unsigned int)((us) / 1000))
#define SEC_TO_US(sec)          (MS_TO_US(SEC_TO_MS(sec)))
#define MS_TO_NS(ms)            ((ms) * 1000 * 1000)
#define SEC_TO_NS(sec)          (MS_TO_NS(SEC_TO_MS(sec)))

#define LOOP_TIMES(timeout_total, timeout_once)                         \
                ((timeout_total) / (timeout_once))                      \
        + ((((timeout_total) % (timeout_once)) == 0) ? 0 : 1 )


// 每天秒数
#define SEC_PER_DAY		(MINUTE_TO_SEC(HOUR_TO_MINUTE(24)))
                                                        
// 使用下面的elapse系列代码，注意unsigned int能否容纳elapse time，即溢出。
// 越精确，能测量的时间越短。
// 为了减少后期处理难度，所有输出用unsigned int
// 假设： sizeof(int) == 4
// 请自行考虑到函数进入和返回的时间

// usuage:
// 所有函数用法类似，以ms为例
// elapse_ms_t id;
// elapse_ms_open(&id);
// do something...
// unsigned int elapse_ms = elapse_ms_close(&id);

typedef struct {
        struct timespec t1;
} elapse_ns_t;
cp_t elapse_ns_open(elapse_ns_t *id);
// count max: 42 9496 7295ns = 4294 967 us = 4294 ms = 4.29 sec
unsigned int elapse_ns_close(elapse_ns_t *id);

typedef struct {
        struct timeval t1;
} elapse_us_t;
cp_t elapse_us_open(elapse_us_t *id);
// count max: 4294 9672 95us = 429 4967ms = 4294 sec = 71min
unsigned int elapse_us_close(elapse_us_t *id);

typedef struct {
        struct timeval t1;
} elapse_ms_t;
cp_t elapse_ms_open(elapse_ms_t *id);
// count max: 4294 9672 95ms = 429 4967 sec = 71582.7min = 1193 hour = 49.7 day
unsigned int elapse_ms(elapse_ms_t *id);
unsigned int elapse_ms_close(elapse_ms_t *id);

typedef struct {
        struct timeval t1;
} elapse_sec_t;
cp_t elapse_sec_open(elapse_sec_t *id);
// count max: 42 9496 7295sec = 7158 2788min = 119 3046 hour = 49710 day = 
//              7101week = 1657 month
unsigned int elapse_sec(elapse_sec_t *id);
unsigned int elapse_sec_close(elapse_sec_t *id);



// 休眠，等待
// 也可用select函数+timeval结构（精确到微秒）或用pselect函数+timespec(精确到纳秒) 。
// 经过测试serial_read_timeout()确定: 下面函数是线程sleep，"不是"进程。
// return: ok?
//#define sleep_ns(sec, ns)               (0 == nanosleep((struct timespec[]){{sec, ns}}, NULL))
cp_t sleep_ns(int sec, long int ns);
#define sleep_us(n)                     sleep_ns(0, (n)*1000) 
#define sleep_ms_in_1000(n)             sleep_ns(0, (n)*1000*1000)
cp_t sleep_ms(int ms);
#define sleep_sec(n)                    sleep_ns(n, 0)


cp_t timeval_valid(const struct timeval *t);
void printf_timeval(const char *title, const struct timeval *t, const char *tail);
bool timeval_sub(struct timeval *result, 
                const struct timeval *x, const struct timeval *y);
cp_t timeval_t1_bigger_t0(const struct timeval *t1, const struct timeval *t0);
cp_t timeval_ms_in_range(int range_ms, 
                const struct timeval *t1, const struct timeval *t0);
struct timeval *timeval_add_ms(struct timeval *tv, int ms);
void timeval_to_timespec(struct timespec *ts, const struct timeval *tv);
cp_t timeval_now(struct timeval *tv);
int timeval_to_sec(const struct timeval *tv);
int timeval_to_ms(const struct timeval *tv);
int timeval_to_us(const struct timeval *tv);


cp_t timespec_valid(const struct timespec *ts);
cp_t timespec_now(struct timespec *ts);
bool timespec_add_ms(struct timespec *ts, int ms);


cp_t time_now(time_t *t);
#define DATE_STR_SIZE      (11)
// 2013-05-07
void DateStr(char str[DATE_STR_SIZE], const time_t *t);
std::string DateStrNow(void);
#define TIME_STR_SIZE		(9)
// 14:52:35
void TimeStr(char str[TIME_STR_SIZE], const time_t *t);
std::string TimeStrNow(void);
#define DATE_TIME_STR_SIZE      (20)
// 2013-05-07 14:52:35
void time_str(char str[DATE_TIME_STR_SIZE], const time_t *t);
std::string DateTimeStrNow(void);


#define DATE_TIME_US_SIZE_MAX   (50)
cp_t time_str_us(char buf[DATE_TIME_US_SIZE_MAX]);


// 含义：
// exp( var, value )在指定时间内，表达式为true，返回true
//
// usuage:
// #define equal( x, y )  ((x)==(y))
// if( !expect_true( a, b, equal, 100, 10 ) ){
//		...
// }
//
// suggest:
// var & value use volatile keyword if possible, such as:
// volatile int var = 0;
//
#define EQUAL(x, y)		((x) == (y))
#define not_equal(x, y)	        ((x) != (y))
#define more_than(x, y)	        ((x) > (y))
#define less_than(x, y)	        ((x) < (y))
#define more_equal_than(x, y)   ((x) >= (y))
#define less_equal_than(x, y)   ((x) <= (y))

typedef void (*void_fun_t)(void);
#define expect(exp, var, value, timeout_total_ms, timeout_once_ms, do_sth_while_loop) \
(bool)({								\
	bool ok = false;					        \
	const int loop_times = LOOP_TIMES(timeout_total_ms, timeout_once_ms); \
	int i;								\
	for (i=0; i<loop_times; i++) {					\
                if (NULL != ((void*)do_sth_while_loop)) {                        \
                        void_fun_t vfun = (void_fun_t)do_sth_while_loop; \
                        vfun();                                          \
                }                                                       \
		if (exp(var, value)) { 					\
			ok = true;					\
                        break;                                          \
                }                                                       \
		sleep_ms(timeout_once_ms);				\
	}							        \
	ok;								\
})



typedef struct {
        int year; 
        int month; 
        int day;

        int hour; 
        int minute; 
        int second; 
        int ms;
} qtime_datetime_t;
#define QTIME_DATETIME_V {              \
        0, 0, 0,                        \
        0, 0, 0, 0,                     \
}
cp_t qtime_get_datetime(qtime_datetime_t *dt);


// 当递减时间时，rest每次递减sub，当减到0那一次时，返回true。其它情况返回false。
// 例如：你要倒计时rest时间，每次递减sub，sub可以是不固定的值。
// 当返回true时，表示递减时间到，可以动作。随后，返回false，均不动作
bool DecRestWhenArrive0(uint32_t &rest, uint32_t sub);

#if 0
long long mktime64 (unsigned int year, unsigned int mon,
		unsigned int day, unsigned int hour,
		unsigned int min, unsigned int sec);
int gmtime64 (const long long *t, struct tm *tp);
#endif

// 注：mktime()在linux平台上 const DateTimeS dts = {1970, 1, 1, 8, 0, 0};时返回0
// 注：mktime64()在linux平台上 const DateTimeS dts = {1970, 1, 1, 0, 0, 0};时返回0

// mktime64就是为了解决32位time_t的2038年的限值
// 在营业报停控中，会下发2099-9-9的日期
typedef long long time64_t;
time64_t pivot_time_t (const time_t * now, time64_t * _t);
struct tm *gmtime64_r (const time64_t * _t, struct tm *p);
struct tm *pivotal_gmtime_r (const time_t * now, const time_t * _t, struct tm *p);
time64_t mktime64 (struct tm * t);
struct tm *pivotal_localtime_r (const time_t * now, const time_t * _t, struct tm *p);
struct tm *localtime64_r (const time64_t * _t, struct tm *p);
time64_t pivotal_timestamp ();

bool LeapYear(int year);
int DaysInMonth(int year, int month);
// 从1970.1.1开始的天数
// year >= 1970
// month: 1 ~ 12
// day: 1 ~ 31
// return: 天数
uint32_t DayFrom1970(uint16_t year, uint8_t month, uint8_t day);

struct DateTimeS
{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
};
// 无效的日期时间
extern const DateTimeS kDateTimeSInvalid;
extern const DateTimeS kDateTimeSValidExample;	// 一个合法的日期时间
// 年月日是否有效
cp_t DateTimeSYearMonthDayValid(const DateTimeS &dts);
// 有些代码，年月日填0xff，时分秒是合法的，单独提出来，代码可以复用
cp_t DateTimeSHourMinuteSecondValid(const DateTimeS &dts);
// 判断日期时间结构体是否合法
cp_t DateTimeSValid(const DateTimeS &dts);
// 获得当前DateTimeS
cp_t DateTimeSNow(DateTimeS &dts);
// 判断日期结构体是否相等
cp_t DateTimeSPtrEqual(const void *lhs, const void *rhs, int size);
cp_t DateTimeSEqual(const DateTimeS &lhs, const DateTimeS &rhs);
bool operator==(const DateTimeS &lhs, const DateTimeS &rhs);
// 打印
cp_t DateTimeSPrint(std::ostream &os, const void *mem, int size);
std::ostream & operator<<(std::ostream &os, const DateTimeS &dts);
std::string DateTimeSStr(const DateTimeS &dts);
// struct tm <-> DateTimeS 结构体转换
void DateTimeS2Tm(struct tm &tms, const DateTimeS &dts);
void Tm2DateTimeS(DateTimeS &dts, const struct tm &tms);
// time_t <-> DateTimeS 结构体转换
cp_t DateTimeS2TimeT32(time_t &sec_from_1970, const DateTimeS &dts);
cp_t TimeT2DateTimeS32(DateTimeS &dts, const time_t sec_from_1970);
cp_t DateTimeS2TimeT64(time64_t &sec_from_1970, const DateTimeS &dts);
cp_t TimeT2DateTimeS64(DateTimeS &dts, const time64_t sec_from_1970);
// lhs < rhs，返回0
cp_t DateTimeSLhsLittleThanRhs(const DateTimeS &lhs, const DateTimeS &rhs);
// 时间是否在范围内，左闭右开，返回0表示在范围内
cp_t DateTimeSInRangeStartStop(const DateTimeS &dts, const DateTimeS &start, const DateTimeS &stop);
cp_t DateTimeSInRangeStartDelay(const DateTimeS &dts, const DateTimeS &start, int delay_time_sec);
// 小时:分钟:秒 是否在指定的范围内，忽略年月日的设置，用在 时段控中
cp_t DateTimeSHHMMSSInRangeStartStop(const DateTimeS &dts, const DateTimeS &start, const DateTimeS &stop);
// 指定填写年月日
DateTimeS DateTimeSFillYearMonthDay(const DateTimeS &dts, unsigned int year, unsigned char month, unsigned char day);
// 把年月日填写成2000-1-1
DateTimeS DateTimeSFillYYMMDD20000101(const DateTimeS &dts);
// 将 hour/min/sec 填写为0，用在营业报停控中
DateTimeS DateTimeSFillHourMinSecZero(const DateTimeS &dts);
// return 0, 在整点
// tolerance_sec	误差范围，例如：误差=10s,则1:00:00 ~ 1:00:10(左闭右开)都认为是整点。tolerance_sec需小于60秒
cp_t DateTimeSOnTheHour(const DateTimeS &dts, unsigned int tolerance_sec);
// 星期是否合法，星期一 ~ 星期天 1 ~ 7，返回0表示错误
cp_t WeekValid(int week);
// 星期，星期一 ~ 星期天 1 ~ 7，返回0表示错误
int WeekOfDateTimeS(const DateTimeS &dts);
// 星期，星期一 ~ 星期天 1 ~ 7，返回0表示错误
int WeekOfDay(unsigned int year, unsigned char month, unsigned char day);


#if 0
//时间数据类型time struct Time
//时间数据类型time
typedef struct 
{
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} Dlt698Time;
extern const Dlt698Time kTimeInvalid;
cp_t Dlt698TimeValid(const Dlt698Time &time);
std::ostream & operator<<(std::ostream &os, const Dlt698Time &time);
Dlt698Time DateTimeS2Time(const DateTimeS &dts);
// 返回距离当天0点的秒数
uint32_t Dlt698Time2Sec(const Dlt698Time &time);
// 时间是否在范围内，左闭右开，返回0表示在范围内
cp_t Dlt698TimeInRange(const Dlt698Time &time, const Dlt698Time &start, const Dlt698Time &stop);
#endif



// month、day，按照自然习惯设置
typedef struct 
{
	uint16_t year;
	uint8_t month;	// 1~12
	uint8_t day;	// 1~31
} YYMMDD;
#define YYMMDD_V {		\
	0, 0, 0				\
}
#define INVALID_DAY		(0)
// day
cp_t YymmddValid(const YYMMDD *yymmdd);
cp_t DayValid(uint32_t day);
uint32_t YymmddToDay(const YYMMDD *yymmdd);

#endif
