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

20230420-----------------------------------------------------------------------
huanglin 创建.
实现简单的单元测试。

原则：用在嵌入式系统上，最简答
*/
#include "qtest.h"
#include "qtest.xcp.h"
//#define this_file_id	0xe3dc453b		// echo -n qtest.c | rhash --simple -


#define TEST_EN				(0)
#define TEST_RUN_TIMES		(1)	// 测试运行次数


//{{{ test_fun
static cp_t QtestCp(const char *name, const QtestItem *item_list, int num, int verbose)
{
	for (int i = 0; i < num; ++i)
	{
		if (verbose > 2)
			qos_printf( "Test item will start. ix=%d/%d name=%s." LINEND, i + 1, num, name);
		const QtestItem item = item_list[i];
		const cp_t cp = item();
		if (verbose > 2)
			qos_printf( "Test item is end. ix=%d/%d name=%s" LINEND, i + 1, num, name);
		ifer(cp);
	}
	return 0;
}
static cp_t QtestCpNTime(const char *name, int run_times, const QtestItem *item_list, int num, int verbose)
{
	for (int i = 0; i < run_times; ++i)
	{
		if (verbose > 1)
			qos_printf("Testing. times=%d/%d name=%s." LINEND, i+1, run_times, name);
		ifer(QtestCp(name, item_list, num, verbose));
	}
	return 0;
}
cp_t QtestTest(const char *name, int run_times, const QtestItem *item_list, int num, int verbose)
{
	if (verbose > 0)
		qos_printf("Unittest is loading. name=%s run_times=%d item_num=%d" LINEND, name, run_times, num);
	const cp_t cp = QtestCpNTime(name, run_times, item_list, num, verbose);
	if (0 != cp)
	{
		char cp_str[CPSTR_SIZE];
		qos_printf("Test FAILED. name=%s cp=%s" LINEND, name, CpStr(cp_str, cp));
	}
	else
		if (verbose > 0)
			qos_printf("Test ok. name=%s" LINEND, name);
	return cp;
}
//}}}


//{{{ test
#if TEST_EN > 0
#include "a_xdr.h"
#include "qarray.h"
#include "qmem.h"
#include "qtime.h"
#include "qfill.h"

#include "qfill_repository.h"
#include "dlt698_45_double_long_unsigned.h"
#include "dlt698_45_double_long.h"
#include "dlt698_45_long_unsigned.h"
#include "dlt698_45_octet_string.h"
#include "dlt698_45_optional.h"
#include "dlt698_45_data_choice.h"
#include "dlt698_45_nfix.h"
#include "dlt698_45_sequence_of.h"
#include "dlt698_45_array.h"
#include "dlt698_45_addr.h"
#include "dlt698_45_base.h"
#include "dlt698_45_link.h"

// client-apdu
#include "dlt698_45_get_request_normal.h"
#include "dlt698_45_get_request_normal_list.h"
#include "dlt698_45_get_request.h"
#include "dlt698_45_client_apdu_choice.h"
#include "dlt698_45_client_apdu.h"
#include "dlt698_45_get_response_normal.h"
#include "dlt698_45_get_response_normal_list.h"

// security_apdu
#include "dlt698_45_security_apdu_choice.h"

#define kRuntimes	(10)
#define kVerbose	(1)	// 2
static cp_t TestAll(void)
{
	// arg: run_times verbose
	ifer(QmemtableTest(kRuntimes, kVerbose));

	ifer(AXdrTest(kRuntimes, kVerbose));
	ifer(QarrayTest(kRuntimes, kVerbose));
	ifer(QtimeTest(kRuntimes, kVerbose));
	ifer(QfillTest(kRuntimes, kVerbose));
	ifer(QfillRepositoryTest(kRuntimes, kVerbose));
	ifer(Dlt69845DoubleLongUnsignedTest(kRuntimes, kVerbose));
	ifer(Dlt69845DoubleLongTest(kRuntimes, kVerbose));
	ifer(Dlt69845LongUnsignedTest(kRuntimes, kVerbose));
	ifer(Dlt69845OctetStringTest(kRuntimes, kVerbose));
	ifer(Dlt69845OptionalTest(kRuntimes, kVerbose));
	ifer(Dlt69845DataChoiceTest(kRuntimes, kVerbose));
	ifer(Dlt69845ArrayTest(kRuntimes, kVerbose));
	ifer(Dlt69845NfixTest(kRuntimes, kVerbose));
	ifer(Dlt69845SequenceOfTest(kRuntimes, kVerbose));
	ifer(Dlt69845AddrTest(kRuntimes, kVerbose));
	ifer(Dlt69845BaseTest(kRuntimes, kVerbose));
	ifer(Dlt69845LinkTest(kRuntimes, kVerbose));

	// client-apdu
	ifer(Dlt69845GetRequestNormalTest(kRuntimes, kVerbose));
	ifer(Dlt69845GetRequestNormalListTest(kRuntimes, kVerbose));
	ifer(Dlt69845GetRequestTest(kRuntimes, kVerbose));
	ifer(Dlt69845ClientApduChoiceTest(kRuntimes, kVerbose));
	ifer(Dlt69845ClientApduTest(kRuntimes, kVerbose));
	ifer(Dlt69845GetResponseNormalTest(kRuntimes, kVerbose));
	ifer(Dlt69845GetResponseNormalListTest(kRuntimes, kVerbose));

	// security-apdu
	ifer(Dlt69845SecurityApduChoiceTest(kRuntimes, kVerbose));
	return 0;
}


// 所有测试，都在QtestEntry()里调用
static const QtestItem kTestItem[] = {
	TestAll
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
	
cp_t QtestEntry(QTEST_ARG)
{
	const int malloc_cnt_start = MallocCnt();
	char a = 10;
	const cp_t cp = QtestTest("qtest_entry", run_times, kTestItem, kTestItemNum, verbose);
	char b = 20;
	const int malloc_cnt_end = MallocCnt();

	// 看堆栈情况
	qos_printf("a=%d b=%d &b-&a=%d" LINEND, a, b, &b - &a);
	qos_printf("malloc_cnt start=%d end=%d" LINEND, malloc_cnt_start, malloc_cnt_end);
	qos_printf(LINEND LINEND LINEND LINEND "QtestEntry exit ***************************************" LINEND LINEND LINEND LINEND);
	rt_thread_mdelay(1000);  
	return cp;
}
#else
cp_t QtestEntry(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

