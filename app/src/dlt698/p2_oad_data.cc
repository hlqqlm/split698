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

20230821-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的Oad + Data
为了描述SetRequestNormalList中的sequence of，先描述oad + data
*/
#include <string.h>

#include "p2_oad_data.h"
#include "p2_oad_data.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2OadDataCutNum)


// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ data
#define LenData			PcutItemLenBySub
static int OffsetData(Pcut *cut, int ix, const char *whole) { return kP2OadDataDataOffset; }
#define ValidData			PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2OadDataNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2OadDataNameData, LenData, OffsetData, ValidData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadDataCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadDataCutIxData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2OadDataPcutOpen(P2OadDataPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	PcutSubSet(&m->base, kP2OadDataCutIxData, &m->data_cut.choice.base, NULL);
	return 0;
}
cp_t P2OadDataPcutClose(P2OadDataPcut *m)
{
	dve(P2OadDataPcutValid(m));

	PcutSubSet(&m->base, kP2OadDataCutIxData, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->data_cut));

	ifer(PcutClose(&m->base));

	return 0;
}
cp_t P2OadDataPcutValid(const P2OadDataPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	return 0;
}


cp_t P2OadDataPcutOpenBase(Pcut *base)
{
	P2OadDataPcut *m = (P2OadDataPcut*)base;
	return P2OadDataPcutOpen(m);
}
cp_t P2OadDataPcutCloseBase(Pcut *base)
{
	P2OadDataPcut *m = (P2OadDataPcut*)base;
	return P2OadDataPcutClose(m);
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
cp_t P2OadDataTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2OadDataTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


