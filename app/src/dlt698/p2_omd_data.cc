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

用来描述dlt698_45中的Omd + Data
为了描述ActionRequestNormalList中的sequence of，先描述omd + data
*/
#include <string.h>

#include "p2_omd_data.h"
#include "p2_omd_data.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2OmdDataCutNum)


// {{{ omd
static int LenOmd(Pcut *cut, int ix, const char *whole) { return kP2OmdDataOmdSize; }
static int OffsetOmd(Pcut *cut, int ix, const char *whole) { return kP2OmdDataOmdOffset; }
static cp_t ValidOmd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ data
#define LenData			PcutItemLenBySub
static int OffsetData(Pcut *cut, int ix, const char *whole) { return kP2OmdDataDataOffset; }
#define ValidData			PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2OmdDataNameOmd, LenOmd, OffsetOmd, ValidOmd, NULL },
	{ kP2OmdDataNameData, LenData, OffsetData, ValidData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDataCutIxOmd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OmdDataCutIxData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2OmdDataPcutOpen(P2OmdDataPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	PcutSubSet(&m->base, kP2OmdDataCutIxData, &m->data_cut.choice.base, NULL);
	return 0;
}
cp_t P2OmdDataPcutClose(P2OmdDataPcut *m)
{
	dve(P2OmdDataPcutValid(m));

	PcutSubSet(&m->base, kP2OmdDataCutIxData, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->data_cut));

	ifer(PcutClose(&m->base));

	return 0;
}
cp_t P2OmdDataPcutValid(const P2OmdDataPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	return 0;
}


cp_t P2OmdDataPcutOpenBase(Pcut *base)
{
	P2OmdDataPcut *m = (P2OmdDataPcut*)base;
	return P2OmdDataPcutOpen(m);
}
cp_t P2OmdDataPcutCloseBase(Pcut *base)
{
	P2OmdDataPcut *m = (P2OmdDataPcut*)base;
	return P2OmdDataPcutClose(m);
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
cp_t P2OmdDataTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2OmdDataTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


