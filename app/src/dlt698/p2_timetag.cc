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
DLT698_45 timetag 部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"
#include "p2_timetag.h"
#include "p2_timetag.xcp.h"
//#define this_file_id	0xd119bb4f		// echo -n dlt698_45_timetag.c | rhash --simple -


#define TEST_EN				(0)
#define TEST_RUN_TIMES		(10)	// 测试运行次数
#define kThisCutNum			(kP2TimetagCutNum)

// {{{ datetimes
static int LenDatetimes(Pcut *cut, int ix, const char *whole) { return P2_TIMETAG_DATETIMES_SIZE; }
static int OffsetDatetimes(Pcut *cut, int ix, const char *whole) { return P2_TIMETAG_DATETIMES_OFFSET; }
static cp_t ValidDatetimes(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = whole + P2_TIMETAG_DATETIMES_OFFSET;
	return Dlt698DatetimesValid(mem);
}
//}}}


// {{{ ti
static int LenTi(Pcut *cut, int ix, const char *whole) { return P2_TIMETAG_TI_SIZE; }
static int OffsetTi(Pcut *cut, int ix, const char *whole) { return P2_TIMETAG_TI_OFFSET; }
static cp_t ValidTi(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = whole + P2_TIMETAG_TI_OFFSET;	
	return Dlt698TiValid(mem);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "datetimes", LenDatetimes, OffsetDatetimes, ValidDatetimes, NULL },
	{ "ti", LenTi, OffsetTi, ValidTi, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TimetagCutIxDatetimes]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2TimetagCutIxTi]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2TimetagPcutOpen(P2TimetagPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2TimetagPcutClose(P2TimetagPcut *m)
{
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2TimetagPcutValid(const P2TimetagPcut *m)
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
cp_t P2TimetagTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2TimetagTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

