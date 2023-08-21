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

20230814-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的action request normal
*/
#include "p2_action_request_normal.h"
#include "p2_action_request_normal.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ActionRequestNormalCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ omd
static int LenOmd(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalOmdSize; }
static int OffsetOmd(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalOmdOffset; }
static cp_t ValidOmd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ data
#define LenData		PcutItemLenBySub
#define OffsetData	PcutItemOffsetDef
#define ValidData	PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ActionRequestNormalNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2ActionRequestNormalNameOmd, LenOmd, OffsetOmd, ValidOmd, NULL },
	{ kP2ActionRequestNormalNameData, LenData, OffsetData, ValidData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionRequestNormalCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionRequestNormalCutIxOmd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionRequestNormalCutIxData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ActionRequestNormalPcutOpen(P2ActionRequestNormalPcut *m)
{
	ifer(P2DataChoicePcutOpen(&m->data));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	PcutSubSet(&m->base, kP2ActionRequestNormalCutIxData, &m->data.choice.base, NULL);
	return 0;
}
cp_t P2ActionRequestNormalPcutClose(P2ActionRequestNormalPcut *m)
{
	dve(P2ActionRequestNormalPcutValid(m));

	PcutSubSet(&m->base, kP2ActionRequestNormalCutIxData, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2DataChoicePcutClose(&m->data));
	return 0;
}
cp_t P2ActionRequestNormalPcutValid(const P2ActionRequestNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->data));
	return 0;
}


cp_t P2ActionRequestNormalPcutOpenBase(Pcut *base)
{
	P2ActionRequestNormalPcut *m = (P2ActionRequestNormalPcut*)base;
	return P2ActionRequestNormalPcutOpen(m);
}
cp_t P2ActionRequestNormalPcutCloseBase(Pcut *base)
{
	P2ActionRequestNormalPcut *m = (P2ActionRequestNormalPcut*)base;
	return P2ActionRequestNormalPcutClose(m);
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
cp_t P2ActionRequestNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionRequestNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
