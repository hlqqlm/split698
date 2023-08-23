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

20230817-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的action response normal list
*/

// for test

#include "p2_action_response_normal_list.h"
#include "p2_action_response_normal_list.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2ActionResponseNormalListCutNum)


// {{{ piid_acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalListPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ActionResponseNormalListPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


// {{{ sequence_of_omd_data
#define LenSequenceOf		PcutItemLenBySub
static int OffsetSequenceOf(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2ActionResponseNormalListSequenceOfOffset;
}
#define ValidSequenceOf		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ActionResponseNormalListNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2ActionResponseNormalListNameSequenceOf, LenSequenceOf, OffsetSequenceOf, ValidSequenceOf, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionResponseNormalListCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionResponseNormalListCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ActionResponseNormalListPcutOpen(P2ActionResponseNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2OmdDarDataOptionalPcutOpen(&m->omd_dar_data_optional_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_cut, &m->omd_dar_data_optional_cut.base, kP2ActionResponseNormalListNameSequenceOf));
	PcutSubSet(&m->base, kP2ActionResponseNormalListCutIxSequenceOf, &m->sequence_of_cut.base, NULL);
	return 0;
}
cp_t P2ActionResponseNormalListPcutClose(P2ActionResponseNormalListPcut *m)
{
	dve(P2ActionResponseNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2ActionResponseNormalListCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_cut));
	ifer(P2OmdDarDataOptionalPcutClose(&m->omd_dar_data_optional_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ActionResponseNormalListPcutValid(const P2ActionResponseNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_cut));
	ifer(P2OmdDarDataOptionalPcutValid(&m->omd_dar_data_optional_cut));
	return 0;
}


cp_t P2ActionResponseNormalListPcutOpenBase(Pcut *base)
{
	P2ActionResponseNormalListPcut *m = (P2ActionResponseNormalListPcut*)base;
	return P2ActionResponseNormalListPcutOpen(m);
}
cp_t P2ActionResponseNormalListPcutCloseBase(Pcut *base)
{
	P2ActionResponseNormalListPcut *m = (P2ActionResponseNormalListPcut*)base;
	return P2ActionResponseNormalListPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0

static cp_t TestPfill(void)
{
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPfill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ActionResponseNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionResponseNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

