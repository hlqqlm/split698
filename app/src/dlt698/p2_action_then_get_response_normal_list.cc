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

用来描述dlt698_45中的action then get response normal list
*/
#include "p2_action_then_get_response_normal_list.h"
#include "p2_action_then_get_response_normal_list.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ActionThenGetResponseNormalListCutNum)



// {{{ piid
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ActionThenGetResponseNormalListPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ActionThenGetResponseNormalListPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of
#define LenSequenceOf		PcutItemLenBySub
static int OffsetSequenceOf(Pcut *cut, int ix, const char *whole) { return kP2ActionThenGetResponseNormalListSequenceOfOffset; }
#define ValidSequenceOf		PcutItemValidBySub
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
	{ kP2ActionThenGetResponseNormalListNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2ActionThenGetResponseNormalListNameSequenceOf, LenSequenceOf, OffsetSequenceOf, ValidSequenceOf, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionThenGetResponseNormalListCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionThenGetResponseNormalListCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ActionThenGetResponseNormalListPcutOpen(P2ActionThenGetResponseNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2OmdDarDataOptionalAresultnormalPcutOpen(&m->omd_dar_data_optional_aresultnormal_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_cut, &m->omd_dar_data_optional_aresultnormal_cut.base, kP2ActionThenGetResponseNormalListNameSequenceOf));
	PcutSubSet(&m->base, kP2ActionThenGetResponseNormalListCutIxSequenceOf, &m->sequence_of_cut.base, NULL);
	return 0;
}
cp_t P2ActionThenGetResponseNormalListPcutClose(P2ActionThenGetResponseNormalListPcut *m)
{
	dve(P2ActionThenGetResponseNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2ActionThenGetResponseNormalListCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_cut));
	ifer(P2OmdDarDataOptionalAresultnormalPcutClose(&m->omd_dar_data_optional_aresultnormal_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ActionThenGetResponseNormalListPcutValid(const P2ActionThenGetResponseNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_cut));
	ifer(P2OmdDarDataOptionalAresultnormalPcutValid(&m->omd_dar_data_optional_aresultnormal_cut));
	return 0;
}


cp_t P2ActionThenGetResponseNormalListPcutOpenBase(Pcut *base)
{
	P2ActionThenGetResponseNormalListPcut *m = (P2ActionThenGetResponseNormalListPcut*)base;
	return P2ActionThenGetResponseNormalListPcutOpen(m);
}
cp_t P2ActionThenGetResponseNormalListPcutCloseBase(Pcut *base)
{
	P2ActionThenGetResponseNormalListPcut *m = (P2ActionThenGetResponseNormalListPcut*)base;
	return P2ActionThenGetResponseNormalListPcutClose(m);
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
cp_t P2ActionThenGetResponseNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionThenGetResponseNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
