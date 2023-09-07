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

integer
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "p2_integer.h"
#include "p2_integer.xcp.h"


#define TEST_EN					(1)
#define kThisCutNum				(kP2IntegerCutNum)


//{{{ misc
static const P2IntegerPcut *ToDerive(const Pcut *cut)
{
	return (P2IntegerPcut*)(cut);
}
//}}}


//{{{ content
static int LenContent(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2IntegerContentSize;
}
static int OffsetContent(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2IntegerContentOffset;
}
static cp_t ValidContent(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainContent(Pcut *cut, int ix, const char *whole) 
{
	const int8_t value = P2IntegerToValue(whole);
	qos_printf("%dD", (int)value);
	return 0;
}
//}}}


//{{{ all
int8_t P2IntegerToValue(const char *whole)
{
	const int content_offset = kP2IntegerContentOffset;
	const char *src = whole + content_offset;
	const int8_t value = src[0];
	return value;
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "content", LenContent, OffsetContent, ValidContent, ExplainContent },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2IntegerCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2IntegerPcutOpen(P2IntegerPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2IntegerPcutClose(P2IntegerPcut *m)
{
	dve(P2IntegerPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2IntegerPcutValid(const P2IntegerPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2IntegerPcutOpenBase(Pcut *base)
{
	P2IntegerPcut *m = (P2IntegerPcut*)base;
	return P2IntegerPcutOpen(m);
}
cp_t P2IntegerPcutCloseBase(Pcut *base)
{
	P2IntegerPcut *m = (P2IntegerPcut*)base;
	return P2IntegerPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestValue(void)
{
	return 0;
}
static cp_t TestFill(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
	TestFill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2IntegerTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2IntegerTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


