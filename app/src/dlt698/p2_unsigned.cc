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

20230614-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

long unsigned
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//#include "qos/qmem.h"
#include "p2_unsigned.h"
#include "p2_unsigned.xcp.h"


#define TEST_EN					(1)
#define kThisCutNum				(kP2UnsignedCutNum)


//{{{ misc
static const P2UnsignedPcut *ToDerive(const Pcut *cut)
{
	return (P2UnsignedPcut*)(cut);
}
//}}}


//{{{ content
static int LenContent(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2UnsignedContentSize;
}
static int OffsetContent(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2UnsignedContentOffset;
}
static cp_t ValidContent(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
static cp_t ExplainContent(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t value = P2UnsignedToValue(whole);
	qos_printf("%uD", (unsigned int)value);
	return 0;
}
//}}}


//{{{ all
uint8_t P2UnsignedToValue(const char *whole)
{
	const int content_offset = kP2UnsignedContentOffset;
	const char *src = whole + content_offset;
	const uint8_t value = src[0];
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
	PCUT_ITEM_NO_SUB(&kCutFix[kP2UnsignedCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2UnsignedPcutOpen(P2UnsignedPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2UnsignedPcutClose(P2UnsignedPcut *m)
{
	dve(P2UnsignedPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2UnsignedPcutValid(const P2UnsignedPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
/*
void P2UnsignedPcutConfigDatatypeExist(P2UnsignedPcut *m, bool exist)
{
	dve(P2UnsignedPcutValid(m));
	m->datatype_exist = exist;
	return;
}
*/

cp_t P2UnsignedPcutOpenBase(Pcut *base)
{
	P2UnsignedPcut *m = (P2UnsignedPcut*)base;
	return P2UnsignedPcutOpen(m);
}
cp_t P2UnsignedPcutCloseBase(Pcut *base)
{
	P2UnsignedPcut *m = (P2UnsignedPcut*)base;
	return P2UnsignedPcutClose(m);
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
cp_t P2UnsignedTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2UnsignedTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


