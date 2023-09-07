/*
This program is licensed under the GPLv2 License:
Copyright 2023 Huanglin <hlqqlm@qq.com>


This file is part of Split698.

Split698 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

Split698 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Split698. If not, see <https://www.gnu.org/licenses/>.

20220318---------------------------------------------------------------------
增加原则：
所有year、month、day，均按照习惯设置范围。如：
month 1 ~ 12
day 1 ~ 31

20130311---------------------------------------------------------------------
20130311        huanglin        create
*/
// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include <iomanip>
#include <iostream>

// other directory header
#include "qtime.h"
// FILEID_REGISTER();
#define this_file_id   0xf7e6336c




// ref: Linux下应用程序内测量时间的各种方法 
cp_t elapse_ns_open(elapse_ns_t *id)
{
        dvb(NULL != id);
        ifbr(0 == clock_gettime(CLOCK_REALTIME, &id->t1));
        return 0;
}
unsigned int elapse_ns_close(elapse_ns_t *id)
{
        dvb(NULL != id);

        struct timespec *t1 = &id->t1;
        struct timespec t2;
        if (0 != clock_gettime(CLOCK_REALTIME, &t2)) 
        {
                dvb(false);
                return 0;
        }

        // calc diff between t1 t2.
        dvb(t2.tv_sec >= t1->tv_sec);
        const unsigned int sec = t2.tv_sec - t1->tv_sec;
        dvb(     ((t2.tv_sec == t1->tv_sec)&&(t2.tv_nsec >= t1->tv_nsec)) ||
                ((t2.tv_sec > t1->tv_sec))
                );
        const int ns = t2.tv_nsec - t1->tv_nsec;

        const unsigned int elapse_ns = sec * 1000 * 1000 * 1000 + ns;
        return elapse_ns;
}




cp_t elapse_us_open(elapse_us_t *id)
{
        dvb(NULL != id);
        ifbr(0 == gettimeofday(&id->t1, NULL));
        return 0;
}
unsigned int elapse_us_close(elapse_us_t *id)
{
        dvb(NULL != id);
        struct timeval *t1 = &id->t1;

        struct timeval t2;
        if (0 != gettimeofday(&t2, NULL)) 
        {
                dvb(false);
                return 0;
        }

        // calc diff between t1 t2.
        dvb(t2.tv_sec >= t1->tv_sec);
        const unsigned int sec = t2.tv_sec - t1->tv_sec;
        dvb(     ((t2.tv_sec == t1->tv_sec) && (t2.tv_usec >= t1->tv_usec)) ||
                ((t2.tv_sec > t1->tv_sec))
                );
        const int us = t2.tv_usec - t1->tv_usec;
        
        const unsigned int elapse_us = sec * 1000 * 1000 + us;
        return elapse_us;
}




cp_t elapse_ms_open(elapse_ms_t *id)
{
        dvb(NULL != id);
        ifbr(0 == gettimeofday(&id->t1, NULL));
        return 0;
}
unsigned int elapse_ms(elapse_ms_t *id)
{
        dvb(0 != id);
        struct timeval *t1 = &id->t1;

        struct timeval t2;
        if (0 != gettimeofday(&t2, NULL)) 
        {
                dvb(false);
                return 0;
        }

        // calc diff between t1 t2.
        dvb(t2.tv_sec >= t1->tv_sec);
        const unsigned int sec = t2.tv_sec - t1->tv_sec;
        dvb(     ((t2.tv_sec == t1->tv_sec) && (t2.tv_usec >= t1->tv_usec)) ||
                ((t2.tv_sec > t1->tv_sec))
                );
        const int us = t2.tv_usec - t1->tv_usec;
        
        const unsigned int v_elapse_ms = (sec * 1000) + (us / 1000);
        return v_elapse_ms;
}
unsigned int elapse_ms_close(elapse_ms_t *id)
{
        dvb(0 != id);
        unsigned int r = elapse_ms(id);
        return r;
}




// can use time() calc also.
cp_t elapse_sec_open(elapse_sec_t *id)
{
        dvb(NULL != id);
        ifbr(0 == gettimeofday(&id->t1, NULL));
        return 0;
}
unsigned int elapse_sec(elapse_sec_t *id)
{
        dvb(0 != id);
        struct timeval *t1 = &id->t1;

        struct timeval t2;
        if (0 != gettimeofday(&t2, NULL)) 
        {
                dvb(false);
                return 0;
        }

        // calc diff between t1 t2.
        dvb(t2.tv_sec >= t1->tv_sec);
        const unsigned int sec = t2.tv_sec - t1->tv_sec;
        return sec;
}
unsigned int elapse_sec_close(elapse_sec_t *id)
{
        dvb(0 != id);
        unsigned int r = elapse_sec(id);
        return r;
}




cp_t sleep_ns(int sec, long int ns)
{
        dvb(0 <= sec);
        dvb(0 <= ns);

        struct timespec ts = {sec, ns};
        //ifbr(0 == nanosleep((struct timespec[]){{sec, ns}}, NULL));
        ifbr(0 == nanosleep(&ts, NULL));
        return 0;
}
cp_t sleep_ms(int ms)
{
        dvb(0 <= ms);
        if (ms < 1000)
        {
                sleep_ns(0, ms*1000*1000);
        }
        
        int sec = ms / 1000;
        int ms_r = ms % 1000;
        ifer(sleep_ns(sec, 0));
        ifer(sleep_ns(0, ms_r*1000*1000));
        return 0;
}





cp_t timeval_now(struct timeval *t)
{
        ifbr(0 == gettimeofday(t, NULL));
        return 0;
                
}
cp_t timeval_valid(const struct timeval *t)
{
        ifbr(NULL != t);
        ifbr(t->tv_usec <= 1000000);
        return 0;
}
// time diff:
// r = x - y
// return: Return 1 if the difference is negative(x<y), otherwise 0.
//
// ref:
// http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
//
// example:
// struct timeval starttime,endtime,timediff;
// gettimeofday(&starttime,0x0);
// your code ...
// gettimeofday(&endtime,0x0);
// timeval_subtract(&timediff,&endtime,&starttime);
//
void printf_timeval(const char *title, const struct timeval *t, const char *tail)
{
        printf("%s%ld.%ld%s", (NULL == title? "" : title), t->tv_sec, t->tv_usec, (NULL == tail ? "" : tail) );
}
/* Return false if result is negative. */
bool timeval_sub(struct timeval *result, const struct timeval *x, const struct timeval *y)
{
        dve(timeval_valid(x));
        dve(timeval_valid(y));

        struct timeval xx = *x;
        struct timeval yy = *y;

        /* Perform the carry for the later subtraction by updating y. */
        if (xx.tv_usec < yy.tv_usec) 
        {
                int nsec = (yy.tv_usec - xx.tv_usec) / 1000000 + 1;
                yy.tv_usec -= 1000000 * nsec;
                yy.tv_sec += nsec;
        }
        if (xx.tv_usec - yy.tv_usec > 1000000) 
        {
                int nsec = (xx.tv_usec - yy.tv_usec) / 1000000;
                yy.tv_usec += 1000000 * nsec;
                yy.tv_sec -= nsec;
        }

        /* Compute the time remaining to wait.
         *      tv_usec is certainly positive. */
        result->tv_sec = xx.tv_sec - yy.tv_sec;
        result->tv_usec = xx.tv_usec - yy.tv_usec;

        dve(timeval_valid(result));
        /* Return false if result is negative. */
        return xx.tv_sec >= yy.tv_sec;
}
cp_t timeval_t1_bigger_t0(const struct timeval *t1, const struct timeval *t0)
{
	dve(timeval_valid(t0));
	dve(timeval_valid(t1));

	if ((t1->tv_sec == t0->tv_sec) && (t1->tv_usec >= t0->tv_usec))
		return 0;

	if (t1->tv_sec > t0->tv_sec)
		return 0;

	return cph;
}
// 判断时间在指定范围内
// t1 >= t0
// diff = t1 - t0
// return: if diff in [0, range_ms], return true.
// note: t1 == t0, range_ms = 0, return true;
cp_t timeval_ms_in_range(int range_ms, const struct timeval *t1, const struct timeval *t0)
{
        dvb(0 <= range_ms);
        dve(timeval_valid(t0));
        dve(timeval_valid(t1));
	/*
	// 保证t1 >= t0
        if (!(     ((t1->tv_sec == t0->tv_sec) && (t1->tv_usec >= t0->tv_usec)) ||
                ((t1->tv_sec > t0->tv_sec))
                ))
	{
		const struct timeval *temp = t1;
		t1 = t0;
		t0 = temp;
	}
	*/

        dvb(t1->tv_sec >= t0->tv_sec);
        dvb(     ((t1->tv_sec == t0->tv_sec) && (t1->tv_usec >= t0->tv_usec)) ||
                ((t1->tv_sec > t0->tv_sec))
                );

        int r_sec = range_ms / 1000;
        int r_us = (range_ms % 1000) * 1000;

        // calc diff between t0 t1.
        struct timeval sub;
        ifbr(timeval_sub(&sub, t1, t0));
        dvb(sub.tv_sec >= 0);
        dvb(sub.tv_usec >= 0);
        if (sub.tv_sec > r_sec)
        {
                return cph;
        }

        if (sub.tv_sec == r_sec) 
        {
                if (sub.tv_usec > r_us)
                {
                        return cph;
                }
        }

        return 0;
}
struct timeval *timeval_add_ms(struct timeval *tv, int ms)
{
        dve(timeval_valid(tv));

        const int sec = ms / 1000;
        const int usec = MS_TO_US(ms % 1000);

        tv->tv_sec += sec;
        tv->tv_usec += usec;

        const int base = SEC_TO_US(1);
        if (0 > tv->tv_usec) 
        {
                tv->tv_sec += tv->tv_usec / base  - 1;
                tv->tv_usec = base + tv->tv_usec % base;
        }
        else if (SEC_TO_US(1) <= tv->tv_usec) 
        {
                tv->tv_sec += tv->tv_usec / base;
                tv->tv_usec = tv->tv_usec % base;
        }
                
        dve(timeval_valid(tv));
        return tv;
}
int timeval_to_sec(const struct timeval *tv)
{
        dvb(NULL != tv);
        return ((tv->tv_sec) + (tv->tv_usec / 1000 / 1000));
}
int timeval_to_ms(const struct timeval *tv)
{
        dvb(NULL != tv);
        return ((tv->tv_sec * 1000) + (tv->tv_usec / 1000));
}
int timeval_to_us(const struct timeval *tv)
{
        dvb(NULL != tv);
        return ((tv->tv_sec * 1000 * 1000) + (tv->tv_usec));
}
void timeval_to_timespec(struct timespec *ts, const struct timeval *tv)
{
        dvb(NULL != ts);
        dve(timeval_valid(tv));

        ts->tv_sec = tv->tv_sec;
        ts->tv_nsec = tv->tv_usec * 1000;
        dve(timespec_valid(ts));

        return;
}





cp_t timespec_valid(const struct timespec *ts)
{
        ifbr(NULL != ts);
        ifbr(0 <= ts->tv_nsec);
        ifbr((1000 * 1000 * 1000) >= ts->tv_nsec);
        return 0;
}
cp_t timespec_now(struct timespec *ts)
{
        dvb(NULL != ts);

        struct timeval now; 
        ifer(timeval_now(&now));
        ts->tv_sec = now.tv_sec;
        ts->tv_nsec = now.tv_usec * 1000;
        dve(timespec_valid(ts));
        return 0;
}
bool timespec_add_ms(struct timespec *ts, int ms)
{
        dve(timespec_valid(ts));

        int sec = ms / 1000;
        int nsec = MS_TO_NS(ms % 1000);

        ts->tv_sec += sec;
        ts->tv_nsec += nsec;

        const int base = SEC_TO_NS(1);
        if (0 > ts->tv_nsec) 
        {
                ts->tv_sec += ts->tv_nsec / base - 1;
                ts->tv_nsec = base + ts->tv_nsec % base;
        }
        else if (SEC_TO_NS(1) <= ts->tv_nsec) 
        {
                ts->tv_sec += ts->tv_nsec / base;
                ts->tv_nsec = ts->tv_nsec % base;
        }

        dve(timespec_valid(ts));
        return true;
}





cp_t time_now(time_t *t)
{
        dvb(NULL != t);
        ifbr(-1 != time(t));
        return 0;
}
void DateStr(char str[DATE_STR_SIZE], const time_t *t)
{
        dvb(NULL != str);
        dvb(NULL != t);

        struct tm tmp;
        localtime_r(t, &tmp);
        const int size_expect = snprintf(str, DATE_STR_SIZE, 
                        "%04d-%02d-%02d", 
                        tmp.tm_year + 1900, tmp.tm_mon + 1, tmp.tm_mday
                        );
        //UNUSED(size_expect);
        (void)(size_expect);
        dvb(DATE_STR_SIZE == size_expect + 1);
        return;
}
std::string DateStrNow(void)
{
	time_t t;
	time_now(&t);
	char str[DATE_STR_SIZE];
	DateStr(str, &t);
	return std::string(str);
}
void TimeStr(char str[TIME_STR_SIZE], const time_t *t)
{
        dvb(NULL != str);
        dvb(NULL != t);

        struct tm tmp;
        localtime_r(t, &tmp);
        const int size_expect = snprintf(str, TIME_STR_SIZE, 
                        "%02d:%02d:%02d", 
                        tmp.tm_hour, tmp.tm_min, tmp.tm_sec
                        );
        //UNUSED(size_expect);
        (void)(size_expect);
        dvb(TIME_STR_SIZE == size_expect + 1);
        return;
}
std::string TimeStrNow(void)
{
	time_t t;
	time_now(&t);
	char str[TIME_STR_SIZE];
	TimeStr(str, &t);
	return std::string(str);
}
void time_str(char str[DATE_TIME_STR_SIZE], const time_t *t)
{
        dvb(NULL != str);
        dvb(NULL != t);

        struct tm tmp;
        localtime_r(t, &tmp);
        const int size_expect = snprintf(str, DATE_TIME_STR_SIZE, 
                        "%04d-%02d-%02d %02d:%02d:%02d", 
                        tmp.tm_year + 1900, tmp.tm_mon + 1, tmp.tm_mday, 
                        tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
        //UNUSED(size_expect);
        (void)(size_expect);
        dvb(DATE_TIME_STR_SIZE == size_expect + 1);
        return;
}
std::string DateTimeStrNow(void)
{
	time_t t;
	time_now(&t);
	char str[DATE_TIME_STR_SIZE];
	time_str(str, &t);
	return std::string(str);
}



cp_t time_str_us(char buf[DATE_TIME_US_SIZE_MAX])
{       
        dvb(NULL != buf);

        time_t t;
        time(&t);
        struct tm *tmp = localtime(&t);

        struct timeval t1;
        ifbr(0 == gettimeofday(&t1, NULL));
        int len_expect = snprintf(buf, DATE_TIME_US_SIZE_MAX, 
                        "%04d-%02d-%02d %02d:%02d:%02d.%06ld", 
                        tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, 
                        tmp->tm_hour, tmp->tm_min, tmp->tm_sec, (long int)t1.tv_usec);
        //UNUSED(len_expect);
        (void)(len_expect);
        dvb(DATE_TIME_US_SIZE_MAX > len_expect);
        return 0;
}




cp_t qtime_get_datetime(qtime_datetime_t *dt)
{
        dvb(NULL != dt);

        time_t now;
        ifbr(0 <= time(&now));

        // get ms
        // ref: http://stackoverflow.com/questions/3756323/getting-the-current-time-in-milliseconds
        struct timespec spec;
        ifbr(0 == clock_gettime(CLOCK_REALTIME, &spec));

        struct tm tm = *localtime(&now);
        dt->year = tm.tm_year + 1900;
        dt->month = tm.tm_mon + 1; 
        dt->day = tm.tm_mday; 
        dt->hour = tm.tm_hour; 
        dt->minute = tm.tm_min; 
        dt->second = tm.tm_sec;

        //time_t s  = spec.tv_sec;        // second
        dt->ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
        dvb(dt->ms < 1000);

        return 0;
}



bool DecRestWhenArrive0(uint32_t &rest, uint32_t sub)
{
	if (0 == rest)
		return false;

	if (sub < rest)
	{
		rest -= sub;
		return false;
	}

	// 只有到达0这一次，返回true
	dvb(rest <= sub);
	rest = 0;
	return true;
}




#if 0
/***************************************************************************//**
 * \brief       Convert time struct to UTC seconds.
 * \param       year : [1900, ..., 2018, ...]
 * \param       mon  : [1, 12]
 * \param       day  : [1, 31]
 * \param       hour : [0, 23]
 * \param       min  : [0, 59]
 * \param       sec  : [0, 59]
 * \return      seconds from 1970-1-1 0:0:0
 * \author      glic
 * \note        timezone is ignored
 ******************************************************************************/
long long mktime64 (unsigned int year, unsigned int mon,
		unsigned int day, unsigned int hour,
		unsigned int min, unsigned int sec)
{
	if (0 >= (int) (mon -= 2)) {    /* 1..12 -> 11,12,1..10 */
		mon += 12;      /* Puts Feb last since it has leap day */
		year -= 1;
	}

	return (((
					(long long) (year/4 - year/100 + year/400 + 367*mon/12 + day) +
					year*365 - 719499
			 )*24 + hour /* now have hours */
			)*60 + min /* now have minutes */
		   )*60 + sec; /* finally seconds */
}

#define SECS_PER_HOUR        (60 * 60)
#define SECS_PER_DAY         (SECS_PER_HOUR * 24)
#define DIV(a, b)            ((a) / (b) - ((a) % (b) < 0))
#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))

#define __isleap(year) \
	((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

/***************************************************************************//**
 * \brief       Convert UTC seconds to tm struct.
 * \param       t : seconds from 1970-1-1 0:0:0
 * \param       tp : tm struct pointer
 * \return      0 : overflow error
 * \return      1 : success
 * \author      glic __offtime
 * \note        timezone is ignored
 *               struct tm
 *               {
 *					int tm_sec;            Seconds. [0-60] (1 leap second) 
 *					int tm_min;            Minutes. [0-59] 
 *				int tm_hour;           Hours.   [0-23] 
 *					int tm_mday;           Day.     [1-31] 
 *					int tm_mon;            Month.   [0-11] 
 *					int tm_year;           Year - 1900.  
 *				int tm_wday;           Day of week. [0-6] 
 *					int tm_yday;           Days in year.[0-365] 
 *               }
 ******************************************************************************/
int gmtime64 (const long long *t, struct tm *tp) 
{
	const unsigned short int __mon_yday[2][13] =
	{
		/* Normal years.  */
		{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
		/* Leap years.  */
		{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
	};
	long int days, rem, y;
	const unsigned short int *ip;

	days = *t / SECS_PER_DAY;
	rem = *t % SECS_PER_DAY;
	while (rem < 0)
	{
		rem += SECS_PER_DAY;
		--days;
	}
	while (rem >= SECS_PER_DAY)
	{
		rem -= SECS_PER_DAY;
		++days;
	}

	tp->tm_hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	tp->tm_min = rem / 60;
	tp->tm_sec = rem % 60;
	/* January 1, 1970 was a Thursday.  */
	tp->tm_wday = (4 + days) % 7;
	if (tp->tm_wday < 0)
		tp->tm_wday += 7;
	y = 1970;

	while (days < 0 || days >= (__isleap (y) ? 366 : 365))
	{
		/* Guess a corrected year, assuming 365 days per year.  */
		long int yg = y + days / 365 - (days % 365 < 0);
		/* Adjust DAYS and Y to match the guessed year.  */
		days -= ((yg - y) * 365
				+ LEAPS_THRU_END_OF (yg - 1)
				- LEAPS_THRU_END_OF (y - 1));
		y = yg;
	}

	tp->tm_year = y - 1900;
	if (tp->tm_year != y - 1900)
	{
		/* The year cannot be represented due to overflow.  */
		//__set_errno (EOVERFLOW);
		return 0;
	}

	tp->tm_yday = days;
	ip = __mon_yday[__isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	tp->tm_mon = y;
	tp->tm_mday = days + 1;
	return 1;
}
#endif






// ref:		https://github.com/TNick/aitown.git
/* DOCUMENTATION: See http://2038bug.com/pivotal_gmtime_doc.html */



static const int days[4][13] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366},
};

#define LEAP_CHECK(n)	((!(((n) + 1900) % 400) || (!(((n) + 1900) % 4) && (((n) + 1900) % 100))) != 0)
#define WRAP(a,b,m)	((a) = ((a) <  0  ) ? ((b)--, (a) + (m)) : (a))

time64_t pivot_time_t (const time_t * now, time64_t * _t)
{
    time64_t t;
    t = *_t;
    if (now && sizeof (time_t) == 4) {
        time_t _now;
        _now = *now;
        if (_now < 1231500000 /* Jan 2009 - date of writing */ )
            _now = 2147483647;
        if ((time64_t) t + ((time64_t) 1 << 31) < (time64_t) _now)
            t += (time64_t) 1 << 32;
    }
    return t;
}

static struct tm *_gmtime64_r (const time_t * now, time64_t * _t, struct tm *p)
{
    int v_tm_sec, v_tm_min, v_tm_hour, v_tm_mon, v_tm_wday, v_tm_tday;
    int leap;
    time64_t t;
    long m;
    t = pivot_time_t (now, _t);
    v_tm_sec = ((time64_t) t % (time64_t) 60);
    t /= 60;
    v_tm_min = ((time64_t) t % (time64_t) 60);
    t /= 60;
    v_tm_hour = ((time64_t) t % (time64_t) 24);
    t /= 24;
    v_tm_tday = t;
    WRAP (v_tm_sec, v_tm_min, 60);
    WRAP (v_tm_min, v_tm_hour, 60);
    WRAP (v_tm_hour, v_tm_tday, 24);
    if ((v_tm_wday = (v_tm_tday + 4) % 7) < 0)
        v_tm_wday += 7;
    m = (long) v_tm_tday;
    if (m >= 0) {
        p->tm_year = 70;
        leap = LEAP_CHECK (p->tm_year);
        while (m >= (long) days[leap + 2][12]) {
            m -= (long) days[leap + 2][12];
            p->tm_year++;
            leap = LEAP_CHECK (p->tm_year);
        }
        v_tm_mon = 0;
        while (m >= (long) days[leap][v_tm_mon]) {
            m -= (long) days[leap][v_tm_mon];
            v_tm_mon++;
        }
    } else {
        p->tm_year = 69;
        leap = LEAP_CHECK (p->tm_year);
        while (m < (long) -days[leap + 2][12]) {
            m += (long) days[leap + 2][12];
            p->tm_year--;
            leap = LEAP_CHECK (p->tm_year);
        }
        v_tm_mon = 11;
        while (m < (long) -days[leap][v_tm_mon]) {
            m += (long) days[leap][v_tm_mon];
            v_tm_mon--;
        }
        m += (long) days[leap][v_tm_mon];
    }
    p->tm_mday = (int) m + 1;
    p->tm_yday = days[leap + 2][v_tm_mon] + m;
    p->tm_sec = v_tm_sec, p->tm_min = v_tm_min, p->tm_hour = v_tm_hour, p->tm_mon = v_tm_mon, p->tm_wday = v_tm_wday;
    return p;
}

struct tm *gmtime64_r (const time64_t * _t, struct tm *p)
{
    time64_t t;
    t = *_t;
    return _gmtime64_r (NULL, &t, p);
}

struct tm *pivotal_gmtime_r (const time_t * now, const time_t * _t, struct tm *p)
{
    time64_t t;
    t = *_t;
    return _gmtime64_r (now, &t, p);
}

time64_t mktime64 (struct tm * t)
{
    int i, y;
    long day = 0;
    time64_t r;
    if (t->tm_year < 70) {
        y = 69;
        do {
            day -= 365 + LEAP_CHECK (y);
            y--;
        } while (y >= t->tm_year);
    } else {
        y = 70;
        while (y < t->tm_year) {
            day += 365 + LEAP_CHECK (y);
            y++;
        }
    }
    for (i = 0; i < t->tm_mon; i++)
        day += days[LEAP_CHECK (t->tm_year)][i];
    day += t->tm_mday - 1;
    t->tm_wday = (int) ((day + 4) % 7);
    r = (time64_t) day *86400;
    r += t->tm_hour * 3600;
    r += t->tm_min * 60;
    r += t->tm_sec;
    return r;
}

static struct tm *_localtime64_r (const time_t * now, time64_t * _t, struct tm *p)
{
    time64_t tl;
    time_t t;
    struct tm tm, tm_localtime, tm_gmtime;
    _gmtime64_r (now, _t, &tm);
    while (tm.tm_year > (2037 - 1900))
        tm.tm_year -= 28;
    t = mktime64 (&tm);
    localtime_r (&t, &tm_localtime);
    gmtime_r (&t, &tm_gmtime);
    tl = *_t;
    tl += (mktime64 (&tm_localtime) - mktime64 (&tm_gmtime));
    _gmtime64_r (now, &tl, p);
    p->tm_isdst = tm_localtime.tm_isdst;
    return p;
}

struct tm *pivotal_localtime_r (const time_t * now, const time_t * _t, struct tm *p)
{
    time64_t tl;
    tl = *_t;
    return _localtime64_r (now, &tl, p);
}

struct tm *localtime64_r (const time64_t * _t, struct tm *p)
{
    time64_t tl;
    tl = *_t;
    return _localtime64_r (NULL, &tl, p);
}


/* Added by Nicu Tofan, January 13, 2014 */

time64_t pivotal_timestamp ()
{
    time_t t_classic = time (NULL);
    time64_t ltime = 0;
    struct tm mytime = *gmtime (&t_classic);
    gmtime64_r(&ltime, &mytime);
    return ltime;
}



bool LeapYear(int year)
{
	if (year % 400 == 0) 
	{
		return true;
	} 
	else if (year % 100 == 0) 
	{
		return false;
	} 
	else if (year % 4 == 0) 
	{
		return true;
	} 

	return false;
}
int DaysInMonth(int year, int month)
{
	//dve(YymmddValid(yymmdd));
	if (month < 1 || month > 12)
		return 0;
	month--;

	const int leap = LeapYear(year) ? 1 : 0;
	//const int month_ix = yymmdd->month - 1;
	/*					J   F   M   A   M   J   J   A   S   O   N   D */
	const int days[2][12] = { {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
						{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

	return days[leap][month];
}
// 从1970.1.1开始的天数
// year >= 1970
// month: 1 ~ 12
// day: 1 ~ 31
// return: 从1970.1.1的天数
uint32_t DayFrom1970(uint16_t year, uint8_t month, uint8_t day)
{
	if (year < 1790)
	{
		dvb(false);
		return 0;
	}
	if (month < 1 || month > 12)
	{
		dvb(false);
		return 0;
	}
	if (day < 1 || day > 31)
	{
		dvb(false);
		return 0;
	}

	const DateTimeS dts = {year, month, day, 0, 0, 0};
	dve(DateTimeSValid(dts));
	struct tm tms;
	DateTimeS2Tm(tms, dts);
	
	// 注：mktime()在linux平台上 const DateTimeS dts = {1970, 1, 1, 8, 0, 0};时返回0
	// 注：mktime64()在linux平台上 const DateTimeS dts = {1970, 1, 1, 0, 0, 0};时返回0
	// 所以这里用mktime64()来转换，日期可以和SEC_PER_DAY整除
	const time64_t second_from_1970 = mktime64(&tms);
	if (-1 == second_from_1970)
	{
		// 参数填写出错
		dvb(false);
		return 0;
	}
	dvb(0 == second_from_1970 % SEC_PER_DAY);
	const uint32_t day_from_1970 = second_from_1970 / SEC_PER_DAY;
	return day_from_1970;
}



//{{{ DateTime
// 下面是从zy6013/control/dlt698.h中移植而来的函数
// ~/zy6013/control/libctlcomm/qtime.h
/*
struct DateTimeS
{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
};
*/
const DateTimeS kDateTimeSInvalid = {0xffff, 0xff, 0xff, 0xff, 0xff, 0xff};
const DateTimeS kDateTimeSValidExample = {2019,1,1,0,0,0};
cp_t DateTimeSYearMonthDayValid(const DateTimeS &dts)
{
	ifbr(0 <= dts.year);

	ifbr(1 <= dts.month);
	ifbr(dts.month <= 12);

	ifbr(1 <= dts.day);
	ifbr(dts.day <= 31);

	return 0;
}
cp_t DateTimeSHourMinuteSecondValid(const DateTimeS &dts)
{
	if (24 == dts.hour && 0 == dts.minute && 0 == dts.second)
		return 0;

	ifbr(0 <= dts.hour);
	ifbr(dts.hour <= 23);

	ifbr(0 <= dts.minute);
	ifbr(dts.minute <= 59);

	ifbr(0 <= dts.second);
	ifbr(dts.second <= 59);
	return 0;
}
cp_t DateTimeSValid(const DateTimeS &dts)
{
	ifer(DateTimeSYearMonthDayValid(dts));
	ifer(DateTimeSHourMinuteSecondValid(dts));
	return 0;
}
// ref: ../../libpublic/time/xtimeGet.cc xU32 xSysTimeGet(xTIMESTRUC *ts)
cp_t DateTimeSNow(DateTimeS &dts)
{
	/*
//日期时间数据类型date_time_s
struct DateTimeS
{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
};
	*/
	//dts = kDateTimeSInvalid;

	struct timeval tv;
	ifbr(0 == gettimeofday(&tv, NULL));

	struct tm * const tmptr = localtime(&tv.tv_sec);
	dts.year = tmptr->tm_year + 1900;
	dts.month = tmptr->tm_mon + 1;		/* tm.Month. [0-11] */
	dts.day = tmptr->tm_mday;
	dts.hour = tmptr->tm_hour;
	dts.minute = tmptr->tm_min;
	dts.second = tmptr->tm_sec;	
	
#if 0
	struct tm now_time;
	struct tm * const tmptr = localtime_r(&tv.tv_sec, &now_time);

	dvb(NULL != tmptr);
	ifbr(NULL != tmptr);
	// printf("当前的本地时间和日期：%s", asctime(info));
	dts.year = now_time.tm_year + 1900;
	dts.month = now_time.tm_mon + 1;		/* tm.Month. [0-11] */
	dts.day = now_time.tm_mday;
	dts.hour = now_time.tm_hour;
	dts.minute = now_time.tm_min;
	dts.second = now_time.tm_sec;
#endif	
	//ts->week = now_time.tm_wday;
	return 0;
}
cp_t DateTimeSPtrEqual(const void *lhs, const void *rhs, int size)
{
    ifbr(NULL != lhs);
    ifbr(NULL != rhs);
    ifbr(sizeof(DateTimeS) == size);
    const DateTimeS &x = *((const DateTimeS *)lhs);
    const DateTimeS &y = *((const DateTimeS *)rhs);
    return DateTimeSEqual(x, y);
}
cp_t DateTimeSEqual(const DateTimeS &lhs, const DateTimeS &rhs)
{
    ifbr(lhs.year == rhs.year);		
    ifbr(lhs.month == rhs.month);			
    ifbr(lhs.day == rhs.day);	
    ifbr(lhs.minute == rhs.minute);		
    ifbr(lhs.second == rhs.second);		
    return 0;
}
bool operator==(const DateTimeS &lhs, const DateTimeS &rhs)
{
    return (0 == DateTimeSEqual(lhs, rhs));
}
static std::ostream &DateTimeSPrintYear(std::ostream &os, unsigned int year)
{
	if (0xffff == year)
		return os << "xxxx";

	return os << year;
}
static std::ostream &DateTimeSPrintUint8(std::ostream &os, uint8_t x)
{
	if (0xff == x)
		return os << "xx";
	
	return os << (int)x;
}
cp_t DateTimeSPrint(std::ostream &os, const void *mem, int size)
{
    ifbr(NULL != mem);
    ifbr(size == sizeof(DateTimeS));
    const DateTimeS &r = *(const DateTimeS *)mem;
    os << std::dec << std::setfill(' ') << std::setw(4);
	DateTimeSPrintYear(os, r.year) << "-" << std::setfill(' ') << std::setw(2);
	DateTimeSPrintUint8(os, r.month) << "-" << std::setfill(' ') << std::setw(2);
	DateTimeSPrintUint8(os, r.day) << " " << std::setfill(' ') << std::setw(2);
	DateTimeSPrintUint8(os, r.hour) << ":" << std::setfill(' ') << std::setw(2);
	DateTimeSPrintUint8(os, r.minute) << ":" << std::setfill(' ') << std::setw(2);
	DateTimeSPrintUint8(os, r.second);
    return 0;
}
std::ostream & operator<<(std::ostream &os, const DateTimeS &dts)
{
	if (0 != DateTimeSPrint(os, &dts, sizeof(DateTimeS)))
		os << "Error in print DateTimeS.";
	return os;
}
std::string DateTimeSStr(const DateTimeS &dts)
{
    std::ostringstream ostr;
	DateTimeSPrint(ostr, &dts, sizeof(DateTimeS));
    return ostr.str();
}
void Tm2DateTimeS(DateTimeS &dts, const struct tm &tms)
{
	dts.year = tms.tm_year + 1900;	// Year - 1900.
	dts.month = tms.tm_mon + 1;		// tm.Month. [0-11] 
	dts.day = tms.tm_mday;
	dts.hour = tms.tm_hour;
	dts.minute = tms.tm_min;
	dts.second = tms.tm_sec;
	return;
}
void DateTimeS2Tm(struct tm &tms, const DateTimeS &dts)
{
	dve(DateTimeSValid(dts));
	// 没有下面这一句，会导致mktime()偶然返回-1，可能mktime()的计算需要下面其它的某些参数
	// In general, when using mktime() it's highly recommended to start with a valid tm structure in the first place (i.e. by calling localtime()) and then changing the fields.
	memset(&tms, 0, sizeof(struct tm));

	tms.tm_year = dts.year - 1900; // Year - 1900.
	dvb(0 < dts.month);
	tms.tm_mon = dts.month - 1;	// Month从0开始
	dvb(0 <= tms.tm_mon && tms.tm_mon < 12);
	tms.tm_mday = dts.day;
	dvb(1 <= tms.tm_mday && tms.tm_mday < 32);
	tms.tm_hour = dts.hour;

//!!!
	// 因为会出现24:0:0
	dvb(0 <= tms.tm_hour && tms.tm_hour <= 24);
	tms.tm_min = dts.minute;
	dvb(0 <= tms.tm_min && tms.tm_min < 60);
	tms.tm_sec = dts.second;
	dvb(0 <= tms.tm_sec && tms.tm_sec < 60);
	return;
}
cp_t DateTimeS2TimeT32(time_t &sec_from_1970, const DateTimeS &dts)
{
	ifer(DateTimeSValid(dts));
	// If we consider that time_t is a signed 32 bits integer, in the best scenario you can create date between ~ 13/12/1901 and 19/1/2038 
	// 年月日合起来比较难比较比较，这里只比较年
	ifbd(1901 < dts.year && dts.year < 2038);
	ifbr(1901 < dts.year && dts.year < 2038);

	sec_from_1970 = 0;
	struct tm tms;
	DateTimeS2Tm(tms, dts);
	sec_from_1970 = mktime(&tms);

	ifbr(-1 != sec_from_1970);
	return 0;
}
// 64位的时间差
cp_t DateTimeS2TimeT64(time64_t &sec_from_1970, const DateTimeS &dts)
{
	ifer(DateTimeSValid(dts));
	// If we consider that time_t is a signed 32 bits integer, in the best scenario you can create date between ~ 13/12/1901 and 19/1/2038 
	// 年月日合起来比较难比较比较，这里只比较年
	//ifbd(1901 < dts.year && dts.year < 2038);
	//ifbr(1901 < dts.year && dts.year < 2038);

	sec_from_1970 = 0;
	struct tm tms;
	DateTimeS2Tm(tms, dts);
	sec_from_1970 = mktime64(&tms);

	ifbr(0 <= sec_from_1970);
	return 0;
}
cp_t TimeT2DateTimeS32(DateTimeS &dts, const time_t sec_from_1970)
{
	struct tm tms;
	ifbr(NULL != localtime_r(&sec_from_1970, &tms));
	// printf("当前的本地时间和日期：%s", asctime(info));
	Tm2DateTimeS(dts, tms);
	return 0;
}
cp_t TimeT2DateTimeS64(DateTimeS &dts, const time64_t sec_from_1970)
{
	struct tm tms;
	ifbr(NULL != localtime64_r(&sec_from_1970, &tms));
	Tm2DateTimeS(dts, tms);
	return 0;
}
cp_t DateTimeSLhsLittleThanRhs(const DateTimeS &lhs, const DateTimeS &rhs)
{
	time64_t lhs_time = 0;
	ifer(DateTimeS2TimeT64(lhs_time, lhs));
	time64_t rhs_time = 0;
	ifer(DateTimeS2TimeT64(rhs_time, rhs));
	ifbr(lhs_time < rhs_time);
	return 0;
}
// 左闭右开
cp_t DateTimeSInRangeStartStop(const DateTimeS &dts, const DateTimeS &start, const DateTimeS &stop)
{
	dve(DateTimeSValid(dts));
	dve(DateTimeSValid(start));
	dve(DateTimeSValid(stop));
	
	time64_t dts_time = 0;
	ifer(DateTimeS2TimeT64(dts_time, dts));
	time64_t start_time = 0;
	ifer(DateTimeS2TimeT64(start_time, start));
	ifbr(start_time <= dts_time);

	time64_t stop_time = 0;
	ifer(DateTimeS2TimeT64(stop_time, stop));
	ifbr(dts_time < stop_time);

	return 0;
}
cp_t DateTimeSInRangeStartDelay(const DateTimeS &dts, const DateTimeS &start, int delay_time_sec)
{
	// 如果为0， 说明任何时间都不在范围内
	// 不用判断，下面的条件中会判断出不在范围内
	//ifbr(0 != delay_time_sec);

	time64_t dts_time = 0;
	ifer(DateTimeS2TimeT64(dts_time, dts));

	time64_t start_time = 0;
	ifer(DateTimeS2TimeT64(start_time, start));
//varx(start_time);
//varx(dts_time);
	ifbr(start_time <= dts_time);

	const time64_t stop_time = start_time + (time64_t)delay_time_sec;
	ifbr(dts_time < stop_time);
	return 0;
}
cp_t DateTimeSHHMMSSInRangeStartStop(const DateTimeS &dts, const DateTimeS &start, const DateTimeS &stop)
{
	// 把年月日统一到2000-1-1
	const DateTimeS dts_fix_yymmdd = DateTimeSFillYYMMDD20000101(dts);
	const DateTimeS start_fix_yymmdd = DateTimeSFillYYMMDD20000101(start);
	const DateTimeS stop_fix_yymmdd = DateTimeSFillYYMMDD20000101(stop);
	return DateTimeSInRangeStartStop(dts_fix_yymmdd, start_fix_yymmdd, stop_fix_yymmdd);
}
DateTimeS DateTimeSFillYearMonthDay(const DateTimeS &dts, unsigned int year, unsigned char month, unsigned char day)
{
	DateTimeS out = dts;
	out.year = year;
	out.month = month;
	out.day = day;
	return out;
}
DateTimeS DateTimeSFillYYMMDD20000101(const DateTimeS &dts)
{
	return DateTimeSFillYearMonthDay(dts, 2000, 1, 1);
}
DateTimeS DateTimeSFillHourMinSecZero(const DateTimeS &dts)
{
	DateTimeS out = dts;
	out.hour = 0;
	out.minute = 0;
	out.second = 0;
	return out;
}
cp_t DateTimeSOnTheHour(const DateTimeS &dts, unsigned int tolerance_sec)
{
	dvb(tolerance_sec < 60);
	ifbr(0 == dts.minute);
	ifbr(0 <= dts.second && dts.second < tolerance_sec);
	return 0;
}
cp_t WeekValid(int week)
{
	ifbr(0 < week);
	ifbr(week < 8);
	return 0;
}
int WeekOfDateTimeS(const DateTimeS &dts)
{
	struct tm tms;
	DateTimeS2Tm(tms, dts);
	if (-1 == mktime(&tms))
		return 0;

	if (tms.tm_wday < 0)
		return 0;
	if (6 < tms.tm_wday)
		return 0;

	// 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 
	// 星期天 对应7
	if (0 == tms.tm_wday)
		return 7;
	return tms.tm_wday;
}
int WeekOfDay(unsigned int year, unsigned char month, unsigned char day)
{
	const DateTimeS dts = {year, month, day, 0, 0, 0};
	return WeekOfDateTimeS(dts);
}
//}}}


#if 0
//{{{ Time
const Dlt698Time kTimeInvalid = {0xff, 0xff, 0xff};
cp_t Dlt698TimeValid(const Dlt698Time &time)
{
	if (0xff != time.hour)
	{
		ifbr(0 <= time.hour);
		ifbr(time.hour < 24);
	}
	if (0xff != time.minute)
	{
		ifbr(0 <= time.minute);
		ifbr(time.minute < 60);
	}
	if (0xff != time.second)
	{
		ifbr(0 <= time.second);
		ifbr(time.second < 60);
	}
	return 0;
}
std::ostream & operator<<(std::ostream &os, const Dlt698Time &time)
{
	os << std::setfill(' ') << std::setw(2) << time.hour
		<< ":" << time.minute
		<< ":" << time.second;
	return os;
}
Dlt698Time DateTimeS2Time(const DateTimeS &dts)
{
	const Dlt698Time time = {dts.hour, dts.minute, dts.second};
	return time;
}
// 返回距离当天0点的秒数
uint32_t Dlt698Time2Sec(const Dlt698Time &time)
{
	return (uint32_t)time.hour * 60 * 60 + (uint32_t)time.minute * 60 + (uint32_t)time.second;
}
cp_t Dlt698TimeInRange(const Dlt698Time &time, const Dlt698Time &start, const Dlt698Time &stop)
{
	dve(Dlt698TimeValid(time));
	dve(Dlt698TimeValid(start));
	dve(Dlt698TimeValid(stop));
	
	const uint32_t time_sec = Dlt698Time2Sec(time);
	const uint32_t start_sec = Dlt698Time2Sec(start);
	ifbr(start_sec <= time_sec);

	const uint32_t stop_sec = Dlt698Time2Sec(stop);
	ifbr(time_sec < stop_sec);
	return 0;
}


//}}}
#endif


//{{{ day
cp_t YymmddValid(const YYMMDD *yymmdd)
{
	ifbr(0 < yymmdd->month);
	ifbr(yymmdd->month <= 12);
	ifbr(0 < yymmdd->day);
	ifbr(yymmdd->day <= 31);
	return 0;
}
cp_t DayValid(uint32_t day)
{
	ifbr(INVALID_DAY != day);
	ifbr(0xffffffff != day);
	return 0;
}
uint32_t YymmddToDay(const YYMMDD *yymmdd)
{
	return DayFrom1970(yymmdd->year, yymmdd->month, yymmdd->day);
}
//}}}
