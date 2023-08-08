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

20230730-----------------------------------------------------------------------
Huanglin创建.
*/
#include "gtest/gtest.h"
#include "fileid.h"
#include "qcp.h"
#include "qtime.h"
#include "qtime_ut.xcp.h"
// #define this_file_id   0x


// 有些测试非常耗时，因此用这个屏蔽之
#define UNIT_TEST_LONG_TIME_EN		(0)


static const FileidItem kFileidItems[] = {
#include "id_filename_table_uniq.h"
	    //#include "../libctlcomm/id_filename_table_uniq.h"
	    //#include "../libctl698/id_filename_table_uniq.h"
};
static const int kFileidNum = sizeof(kFileidItems) / sizeof(kFileidItems[0]);
static const FileidTable kFileidTable = {kFileidItems, kFileidNum};
const FileidTable *MainFileidTable() { return &kFileidTable; }


namespace {

// ../../app/src/qcp.h
// ../../app/src/qbuf/qbuf.h
// ../../app/src/qbuf/qtimeout.h
// ../../app/src/qtime.h



TEST(Qtime, loop_times) {
	EXPECT_EQ(1, LOOP_TIMES(1, 1));
	EXPECT_EQ(2, LOOP_TIMES(2, 1 ));

	EXPECT_EQ(1, LOOP_TIMES(1, 2));
	EXPECT_EQ(1, LOOP_TIMES(1, 99));

	EXPECT_EQ(5, LOOP_TIMES(10, 2));
	EXPECT_EQ(4, LOOP_TIMES(10, 3));
	return;
}






TEST(Qtime, mktime64_gmtime64)
{
// ../src/qos/qtime.cc
	const DateTimeS dts = {2000, 1, 1, 1, 1, 11};
	struct tm tms;
	DateTimeS2Tm(tms, dts);

	// mktime64 结果等于 mktime()
	time64_t tt64 = mktime64(&tms);
	time_t tt32 = mktime(&tms);
	// 因为tt64没有考虑时区，而PC机是有时区的
	EXPECT_EQ(tt64, tt32 + 8 * 60 * 60);

	// 将mktime64结果，再转换回struct tm，和转换前应该相同
	struct tm tm_out;
	gmtime64_r(&tt64, &tm_out);
	DateTimeS dts_out;
	Tm2DateTimeS(dts_out, tm_out);
	EXPECT_TRUE(dts_out == dts);
	//dlt698.h
}



// mktime64就是为了解决32位time_t的2038年的限值，因此，这里测试一下超过2038年会怎样
TEST(Qtime, mktime64_gmtime64_after_2038)
{
	// 2099- 9- 9 
	const DateTimeS dts = {2099, 12, 30, 23, 59, 59};
	struct tm tms;
	DateTimeS2Tm(tms, dts);

	// mktime64 超过2038年时，无法获取mktime()结果
	time64_t tt64 = mktime64(&tms);
	// 通过时间计算器算出 dts - 1970-1-1 0:0:0 的秒差
	EXPECT_EQ(4102358399, tt64);

	// 将mktime64结果，再转换回struct tm，和转换前应该相同
	struct tm tm_out;
	gmtime64_r(&tt64, &tm_out);
	DateTimeS dts_out;
	Tm2DateTimeS(dts_out, tm_out);
	EXPECT_TRUE(dts_out == dts);
	//dlt698.h
}


TEST(Qtime, DayFrom1970)
{
	EXPECT_EQ(0, DayFrom1970(1970, 1, 1));
	EXPECT_EQ(1, DayFrom1970(1970, 1, 2));
	EXPECT_EQ(19069, DayFrom1970(2022, 3, 18));
}

TEST(Qtime, elapse_ns)
{
        elapse_ns_t id;
        EXPECT_EQ(0, elapse_ns_open(&id));
        unsigned int ns = elapse_ns_close(&id);
        EXPECT_TRUE(0 <= ns);
        return;
}


TEST(Qtime, elapse_us)
{
        elapse_us_t id;
        EXPECT_EQ(0, elapse_us_open(&id));
        unsigned int us = elapse_us_close(&id);
        EXPECT_TRUE(0 <= us);
        return;
}


TEST(Qtime, elapse_ms)
{
        if (!UNIT_TEST_LONG_TIME_EN)
            return;

        elapse_ms_t id;
        EXPECT_EQ(0, elapse_ms_open(&id));
        unsigned int ms0 = elapse_ms(&id);
        sleep_ms(1);
        unsigned int ms1 = elapse_ms_close(&id);
        EXPECT_TRUE(ms1 > ms0);
        return;
}


TEST(Qtime, elapse_sec)
{
        if (!UNIT_TEST_LONG_TIME_EN)
            return;

        elapse_sec_t id;
        EXPECT_EQ(0, elapse_sec_open(&id));
        unsigned int sec0 = elapse_sec(&id);
        sleep_sec(1);
        unsigned int sec1 = elapse_sec_close(&id);
        EXPECT_TRUE(sec1 >= sec0);
        return;
}



TEST(Qtime, timeval_sub)
{
        {
                struct timeval t0 = {2, 999999};
                struct timeval t1 = {3, 1998};  // diff = 1000 + 999 us

                struct timeval sub;
                EXPECT_TRUE(timeval_sub(&sub, &t1, &t0));
                EXPECT_TRUE(0 == sub.tv_sec);
                EXPECT_TRUE(1999 == sub.tv_usec);

                EXPECT_TRUE(!timeval_sub(&sub, &t0, &t1));
                EXPECT_TRUE(-1 == sub.tv_sec);
                EXPECT_TRUE((1000000-1999) == sub.tv_usec);
        }
        {
                struct timeval t0 = {2, 0};
                struct timeval t1 = {2, 1};  // diff = 1 us

                struct timeval sub;
                EXPECT_TRUE(timeval_sub(&sub, &t1, &t0));
                EXPECT_TRUE(0 == sub.tv_sec);
                EXPECT_TRUE(1 == sub.tv_usec);

                EXPECT_TRUE(!timeval_sub(&sub, &t0, &t1));
                EXPECT_TRUE(-1 == sub.tv_sec);
                EXPECT_TRUE((1000000 - 1) == sub.tv_usec);
        }
        return;
}



TEST(Qtime, timeval_ms_in_range)
{
        {
                struct timeval t0 = {2, 999999};
                struct timeval t1 = {3, 1998};  // diff = 1000 + 999 us
                EXPECT_EQ(0, timeval_ms_in_range(2, &t1, &t0));
                EXPECT_NE(0, timeval_ms_in_range(1, &t1, &t0));
        }
        {
                struct timeval t0 = {2, 0};
                struct timeval t1 = {2, 1001};  // diff = 1001 us
                EXPECT_NE(0, timeval_ms_in_range(1, &t1, &t0));
                EXPECT_EQ(0, timeval_ms_in_range(2, &t1, &t0));
        }
        return;
}



TEST(Qtime, timeval_add_ms)
{
// ../src/qos/qtime.cc
        {
                struct timeval tv = {1, 0};
                const struct timeval expect = {1, 0};
                EXPECT_TRUE(0 == memcmp(timeval_add_ms(&tv, 0), &expect, sizeof(struct timeval)));
        }
        {
                struct timeval tv = {1, 0};
                const struct timeval expect = {1, 999 * 1000};
                EXPECT_TRUE(0 == memcmp(timeval_add_ms(&tv, 999), &expect, sizeof(struct timeval)));
        }
        {
                struct timeval tv = {1, 0};
                const struct timeval expect = {2, 1 * 1000};
                EXPECT_TRUE(0 == memcmp(timeval_add_ms(&tv, 1001), &expect, sizeof(struct timeval)));
        }
        {
                struct timeval tv = {1, 0};
                const struct timeval expect = {0, 1000 * 1000 - 1000};
                EXPECT_TRUE(0 == memcmp(timeval_add_ms(&tv, -1), &expect, sizeof(struct timeval)));
        }
        {
                struct timeval tv = {1, 0};
                const struct timeval expect = {-1, 1000 * 1000 - 1000};
                EXPECT_TRUE(0 == memcmp(timeval_add_ms(&tv, -1001), &expect, sizeof(struct timeval)));
        }

        return;
}


TEST(Qtime, timespec_add_ms)
{
        {
                struct timespec ts = {1, 0};
                EXPECT_TRUE(timespec_add_ms(&ts, 0));
                const struct timespec expect = {1, 0};
                EXPECT_TRUE(0 == memcmp(&ts, &expect, sizeof(struct timespec)));
        }
        {
                struct timespec ts = {1, 0};
                EXPECT_TRUE(timespec_add_ms(&ts, 999));
                const struct timespec expect = {1, 999 * 1000 * 1000};
                EXPECT_TRUE(0 == memcmp(&ts, &expect, sizeof(struct timespec)));
        }
        {
                struct timespec ts = {1, 0};
                EXPECT_TRUE(timespec_add_ms(&ts, 1001));
                const struct timespec expect = {2, 1 * 1000 * 1000};
                EXPECT_TRUE(0 == memcmp(&ts, &expect, sizeof(struct timespec)));
        }
        {
                struct timespec ts = {1, 0};
                EXPECT_TRUE(timespec_add_ms(&ts, -1));
                const struct timespec expect = {0, 1000 * 1000 * 1000 - 1000 * 1000};
                EXPECT_TRUE(0 == memcmp(&ts, &expect, sizeof(struct timespec)));
        }
        {
                struct timespec ts = {1, 0};
                EXPECT_TRUE(timespec_add_ms(&ts, -1001));
                const struct timespec expect = {-1, 1000 * 1000 * 1000 - 1000 * 1000};
                EXPECT_TRUE(0 == memcmp(&ts, &expect, sizeof(struct timespec)));
        }

        return;
}




static void *thread_change_var(void *arg)
{
        int *var = (int*)arg;

        sleep_ms(1000);
        *var = 1;
        return NULL;
}
TEST(Qtime, expect)
{
        if (!UNIT_TEST_LONG_TIME_EN)
                return;

        EXPECT_TRUE(expect(EQUAL, true, true, 10, 1, NULL));

        // should wait timeout
        {
                elapse_ms_t elapse;
                EXPECT_EQ(0, elapse_ms_open(&elapse));
                EXPECT_TRUE(!expect(EQUAL, 1, 2, 100, 10, NULL));
                EXPECT_TRUE(100 <= elapse_ms_close(&elapse));
        }

        // should not wait timeout_total_ms, because thread change the var let 
        // var == 1
        {
                pthread_t thread;
                volatile int var = 2;
                EXPECT_TRUE(0 == pthread_create(&thread, NULL, thread_change_var, (void*)&var));
                elapse_ms_t elapse;
                EXPECT_EQ(0, elapse_ms_open(&elapse));
                EXPECT_TRUE(expect(EQUAL, var, 1, 4000, 10, NULL));
                // elapse time should less than 4000ms
                const int ms = elapse_ms_close(&elapse);
                EXPECT_TRUE(1000 <= ms);
                EXPECT_TRUE(2000 >= ms);
                EXPECT_TRUE(0 == pthread_join(thread, NULL));
        }

        return;
}






TEST(Qtime, qtime_get_datetime)
{
        qtime_datetime_t dt = QTIME_DATETIME_V;
        EXPECT_EQ(0, qtime_get_datetime(&dt));
        EXPECT_TRUE(1900 <= dt.year);
        return;
}


}  // namespace
