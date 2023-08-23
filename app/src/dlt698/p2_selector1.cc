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

20230822-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的selector1
*/
#include <string.h>

#include "p2_selector1.h"
#include "p2_selector1.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2Selector1CutNum)


// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2Selector1OadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2Selector1OadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ data
#define LenData			PcutItemLenBySub
static int OffsetData(Pcut *cut, int ix, const char *whole) { return kP2Selector1DataOffset; }
#define ValidData			PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2Selector1NameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2Selector1NameData, LenData, OffsetData, ValidData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Selector1CutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2Selector1CutIxData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2Selector1PcutOpen(P2Selector1Pcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	PcutSubSet(&m->base, kP2Selector1CutIxData, &m->data_cut.choice.base, NULL);
	return 0;
}
cp_t P2Selector1PcutClose(P2Selector1Pcut *m)
{
	dve(P2Selector1PcutValid(m));

	PcutSubSet(&m->base, kP2Selector1CutIxData, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->data_cut));

	ifer(PcutClose(&m->base));

	return 0;
}
cp_t P2Selector1PcutValid(const P2Selector1Pcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	return 0;
}


cp_t P2Selector1PcutOpenBase(Pcut *base)
{
	P2Selector1Pcut *m = (P2Selector1Pcut*)base;
	return P2Selector1PcutOpen(m);
}
cp_t P2Selector1PcutCloseBase(Pcut *base)
{
	P2Selector1Pcut *m = (P2Selector1Pcut*)base;
	return P2Selector1PcutClose(m);
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
cp_t P2Selector1Test(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2Selector1Test(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


