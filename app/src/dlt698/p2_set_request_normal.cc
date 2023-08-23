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

20230815-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的set request normal
*/
#include "p2_set_request_normal.h"
#include "p2_set_request_normal.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2SetRequestNormalCutNum)



// {{{ piid
static int LenPiid(Pcut *part, int ix, const char *whole) { return kP2SetRequestNormalPiidSize; }
static int OffsetPiid(Pcut *part, int ix, const char *whole) { return kP2SetRequestNormalPiidOffset; }
static cp_t ValidPiid(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ oad
static int LenOad(Pcut *part, int ix, const char *whole) { return kP2SetRequestNormalOadSize; }
static int OffsetOad(Pcut *part, int ix, const char *whole) { return kP2SetRequestNormalOadOffset; }
static cp_t ValidOad(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ data
#define LenData		PcutItemLenBySub
#define OffsetData	PcutItemOffsetDef
#define ValidData	PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2SetRequestNormalNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2SetRequestNormalNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2SetRequestNormalNameData, LenData, OffsetData, ValidData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetRequestNormalCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetRequestNormalCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetRequestNormalCutIxData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SetRequestNormalPcutOpen(P2SetRequestNormalPcut *m)
{
	ifer(P2DataChoicePcutOpen(&m->data));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	PcutSubSet(&m->base, kP2SetRequestNormalCutIxData, &m->data.choice.base, NULL);
	return 0;
}
cp_t P2SetRequestNormalPcutClose(P2SetRequestNormalPcut *m)
{
	dve(P2SetRequestNormalPcutValid(m));

	PcutSubSet(&m->base, kP2SetRequestNormalCutIxData, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2DataChoicePcutClose(&m->data));
	return 0;
}
cp_t P2SetRequestNormalPcutValid(const P2SetRequestNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->data));
	return 0;
}


cp_t P2SetRequestNormalPcutOpenBase(Pcut *base)
{
	P2SetRequestNormalPcut *m = (P2SetRequestNormalPcut*)base;
	return P2SetRequestNormalPcutOpen(m);
}
cp_t P2SetRequestNormalPcutCloseBase(Pcut *base)
{
	P2SetRequestNormalPcut *m = (P2SetRequestNormalPcut*)base;
	return P2SetRequestNormalPcutClose(m);
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
cp_t P2SetRequestNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SetRequestNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
