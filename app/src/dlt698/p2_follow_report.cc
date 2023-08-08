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

20230423-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 follow_report 部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/
#include <string.h>

#include "qdlt698.h"
#include "p2_follow_report.h"
#include "p2_follow_report.xcp.h"
//#define this_file_id	0xd37d9e20		// echo -n dlt698_45_follow_report.c | rhash --simple -


#define TEST_EN				(0)
#define TEST_RUN_TIMES		(10)	// 测试运行次数


// {{{ datetimes
static int LenDatetimes(Pcut *part, int ix, const char *whole) { return kP2FollowReportXXXSize; }
static int OffsetDatetimes(Pcut *part, int ix, const char *whole) { return kP2FollowReportXXXoffset; }
static cp_t ValidDatetimes(Pcut *part, int ix, const char *whole) 
{ 
	const char *mem = whole + kP2FollowReportXXXoffset;
	return Dlt698DatetimesValid(mem);
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2FollowReportPartNum] = {
	// name len offset valid
	{ "datetimes", LenDatetimes, OffsetDatetimes, ValidDatetimes },
};
	

static const PcutItem kPartItemsPattern[kP2FollowReportPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2FollowReportPartIxDatetimes]),
};
static void PcutItemsInit(PcutItem items[kP2FollowReportPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}


cp_t P2FollowReportPcutOpen(P2FollowReportPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2FollowReportPartNum));
	return 0;
}
cp_t P2FollowReportPcutClose(P2FollowReportPcut *m)
{
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2FollowReportPcutValid(const P2FollowReportPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2FollowReportTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2FollowReportTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

