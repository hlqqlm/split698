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

用来描述dlt698_45中的report_notification_list
*/
#include "p2_compact_proxy_get_response_normal.h"
#include "p2_compact_proxy_get_response_normal.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2CompactProxyGetResponseNormalCutNum)



// {{{ piid-acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetResponseNormalPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetResponseNormalPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_a_result_normal
#define LenSequenceOf		PcutItemLenBySub
static int OffsetSequenceOf(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetResponseNormalSequenceOfOffset; }
#define ValidSequenceOf		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactProxyGetResponseNormalNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2CompactProxyGetResponseNormalNameSequenceOf, LenSequenceOf, OffsetSequenceOf, ValidSequenceOf, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetResponseNormalCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetResponseNormalCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxyGetResponseNormalPcutOpen(P2CompactProxyGetResponseNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2GetResultPcutOpen(&m->get_result));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of, &m->get_result.choice.base, kP2CompactProxyGetResponseNormalNameSequenceOf));
	PcutSubSet(&m->base, kP2CompactProxyGetResponseNormalCutIxSequenceOf, &m->sequence_of.base, NULL);
	return 0;
}
cp_t P2CompactProxyGetResponseNormalPcutClose(P2CompactProxyGetResponseNormalPcut *m)
{
	dve(P2CompactProxyGetResponseNormalPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxyGetResponseNormalCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of));
	ifer(P2GetResultPcutClose(&m->get_result));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxyGetResponseNormalPcutValid(const P2CompactProxyGetResponseNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of));
	ifer(P2GetResultPcutValid(&m->get_result));
	return 0;
}


cp_t P2CompactProxyGetResponseNormalPcutOpenBase(Pcut *base)
{
	P2CompactProxyGetResponseNormalPcut *m = (P2CompactProxyGetResponseNormalPcut*)base;
	return P2CompactProxyGetResponseNormalPcutOpen(m);
}
cp_t P2CompactProxyGetResponseNormalPcutCloseBase(Pcut *base)
{
	P2CompactProxyGetResponseNormalPcut *m = (P2CompactProxyGetResponseNormalPcut*)base;
	return P2CompactProxyGetResponseNormalPcutClose(m);
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
cp_t P2CompactProxyGetResponseNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyGetResponseNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
