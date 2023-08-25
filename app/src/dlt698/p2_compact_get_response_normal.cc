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

20230825-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的CompactGetResponseNormal
*/
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"

#include "p2_compact_get_response_normal.h"
#include "p2_compact_get_response_normal.xcp.h"


#define TEST_EN				(0)
#define kThisCutNum			(kP2CompactGetResponseNormalCutNum)


// {{{ piid-acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactGetResponseNormalPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactGetResponseNormalPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ get_result
#define LenGetResult			PcutItemLenBySub
static int OffsetGetResult(Pcut *cut, int ix, const char *whole) { return kP2CompactGetResponseNormalGetResultOffset; }
//#define OffsetGetResult			PcutItemOffsetDef
#define ValidGetResult			PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactGetResponseNormalNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2CompactGetResponseNormalNameGetResult, LenGetResult, OffsetGetResult, ValidGetResult, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactGetResponseNormalCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactGetResponseNormalCutIxGetResult]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactGetResponseNormalPcutOpen(P2CompactGetResponseNormalPcut *m)
{

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2GetResultPcutOpen(&m->get_result_cut));
	PcutSubSet(&m->base, kP2CompactGetResponseNormalCutIxGetResult, &m->get_result_cut.choice.base, NULL);
	return 0;
}
cp_t P2CompactGetResponseNormalPcutClose(P2CompactGetResponseNormalPcut *m)
{
	dve(P2CompactGetResponseNormalPcutValid(m));

	PcutSubSet(&m->base, kP2CompactGetResponseNormalCutIxGetResult, NULL, NULL);
	ifer(P2GetResultPcutClose(&m->get_result_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactGetResponseNormalPcutValid(const P2CompactGetResponseNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2GetResultPcutValid(&m->get_result_cut));
	return 0;
}


cp_t P2CompactGetResponseNormalPcutOpenBase(Pcut *base)
{
	P2CompactGetResponseNormalPcut *m = (P2CompactGetResponseNormalPcut*)base;
	return P2CompactGetResponseNormalPcutOpen(m);
}
cp_t P2CompactGetResponseNormalPcutCloseBase(Pcut *base)
{
	P2CompactGetResponseNormalPcut *m = (P2CompactGetResponseNormalPcut*)base;
	return P2CompactGetResponseNormalPcutClose(m);
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
cp_t P2CompactGetResponseNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactGetResponseNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
