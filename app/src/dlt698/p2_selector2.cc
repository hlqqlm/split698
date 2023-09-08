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

20230907-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的selector2
*/
#include <string.h>

#include "qstr.h"

#include "p2_selector2.h"
#include "p2_selector2.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2Selector2CutNum)


// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2Selector2OadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2Selector2OadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ start
#define LenStart			PcutItemLenBySub
/*
static int LenStart(Pcut *cut, int ix, const char *whole) 
{ 
	const int value = PcutItemLenBySub(cut, ix, whole);
	qos_printf("len_start=%d\n", value);
	printf_hex_ex("len_start_mem:", "\n", whole, 10, "");
	return value;
}
*/
static int OffsetStart(Pcut *cut, int ix, const char *whole) { return kP2Selector2StartOffset; }
#define ValidStart			PcutItemValidBySub
//}}}


// {{{ end
#define LenEnd			PcutItemLenBySub
#define OffsetEnd		PcutItemOffsetDef
#define ValidEnd		PcutItemValidBySub
//}}}


// {{{ interval
#define LenInterval		PcutItemLenBySub
/*
static int LenInterval(Pcut *cut, int ix, const char *whole) 
{ 
	here(11);
	const int value = PcutItemLenBySub(cut, ix, whole);
	qos_printf("len_interval=%d\n", value);
	printf_hex_ex("len_interval_mem:", "\n", whole, 10, "");
	return value;
}
*/
#define OffsetInterval	PcutItemOffsetDef
#define ValidInterval	PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2Selector2NameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2Selector2NameStart, LenStart, OffsetStart, ValidStart, NULL },
	{ kP2Selector2NameEnd, LenEnd, OffsetEnd, ValidEnd, NULL },
	{ kP2Selector2NameInterval, LenInterval, OffsetInterval, ValidInterval, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Selector2CutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Selector2CutIxStart]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Selector2CutIxEnd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Selector2CutIxInterval]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2Selector2PcutOpen(P2Selector2Pcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DataChoicePcutOpen(&m->start));
	PcutSubSet(&m->base, kP2Selector2CutIxStart, &m->start.choice.base, kP2Selector2NameStart);

	ifer(P2DataChoicePcutOpen(&m->end));
	PcutSubSet(&m->base, kP2Selector2CutIxEnd, &m->end.choice.base, kP2Selector2NameEnd);

	ifer(P2DataChoicePcutOpen(&m->interval));
	PcutSubSet(&m->base, kP2Selector2CutIxInterval, &m->interval.choice.base, kP2Selector2NameInterval);
	return 0;
}
cp_t P2Selector2PcutClose(P2Selector2Pcut *m)
{
	dve(P2Selector2PcutValid(m));

	PcutSubSet(&m->base, kP2Selector2CutIxStart, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->start));

	PcutSubSet(&m->base, kP2Selector2CutIxEnd, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->end));

	PcutSubSet(&m->base, kP2Selector2CutIxInterval, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->interval));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2Selector2PcutValid(const P2Selector2Pcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->start));
	ifer(P2DataChoicePcutValid(&m->end));
	ifer(P2DataChoicePcutValid(&m->interval));
	return 0;
}


cp_t P2Selector2PcutOpenBase(Pcut *base)
{
	P2Selector2Pcut *m = (P2Selector2Pcut*)base;
	return P2Selector2PcutOpen(m);
}
cp_t P2Selector2PcutCloseBase(Pcut *base)
{
	P2Selector2Pcut *m = (P2Selector2Pcut*)base;
	return P2Selector2PcutClose(m);
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
cp_t P2Selector2Test(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2Selector2Test(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


