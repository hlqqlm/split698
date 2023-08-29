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

用来描述dlt698_45中的action request normal list
*/
#include "p2_action_then_get_request_normal_list.h"
#include "p2_action_then_get_request_normal_list.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ActionThenGetRequestNormalListCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2ActionThenGetRequestNormalListPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2ActionThenGetRequestNormalListPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_omd_data_oad_unsigned
#define LenSequenceOfOmdDataOadUnsigned		PcutItemLenBySub
static int OffsetSequenceOfOmdDataOadUnsigned(Pcut *cut, int ix, const char *whole) { return kP2ActionThenGetRequestNormalListSequenceOfOmdDataOadUnsignedOffset; }
#define ValidSequenceOfOmdDataOadUnsigned		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ActionThenGetRequestNormalListNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2ActionThenGetRequestNormalListNameSequenceOfOmdDataOadUnsigned, LenSequenceOfOmdDataOadUnsigned, OffsetSequenceOfOmdDataOadUnsigned, ValidSequenceOfOmdDataOadUnsigned, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionThenGetRequestNormalListCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionThenGetRequestNormalListCutIxSequenceOfOmdDataOadUnsigned]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ActionThenGetRequestNormalListPcutOpen(P2ActionThenGetRequestNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2OmdDataOadUnsignedPcutOpen(&m->omd_data_oad_unsigned_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_omd_data_oad_unsigned_cut, &m->omd_data_oad_unsigned_cut.base, kP2ActionThenGetRequestNormalListNameSequenceOfOmdDataOadUnsigned));
	PcutSubSet(&m->base, kP2ActionThenGetRequestNormalListCutIxSequenceOfOmdDataOadUnsigned, &m->sequence_of_omd_data_oad_unsigned_cut.base, NULL);
	return 0;
}
cp_t P2ActionThenGetRequestNormalListPcutClose(P2ActionThenGetRequestNormalListPcut *m)
{
	dve(P2ActionThenGetRequestNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2ActionThenGetRequestNormalListCutIxSequenceOfOmdDataOadUnsigned, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_omd_data_oad_unsigned_cut));
	ifer(P2OmdDataOadUnsignedPcutClose(&m->omd_data_oad_unsigned_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ActionThenGetRequestNormalListPcutValid(const P2ActionThenGetRequestNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_omd_data_oad_unsigned_cut));
	ifer(P2OmdDataOadUnsignedPcutValid(&m->omd_data_oad_unsigned_cut));
	return 0;
}


cp_t P2ActionThenGetRequestNormalListPcutOpenBase(Pcut *base)
{
	P2ActionThenGetRequestNormalListPcut *m = (P2ActionThenGetRequestNormalListPcut*)base;
	return P2ActionThenGetRequestNormalListPcutOpen(m);
}
cp_t P2ActionThenGetRequestNormalListPcutCloseBase(Pcut *base)
{
	P2ActionThenGetRequestNormalListPcut *m = (P2ActionThenGetRequestNormalListPcut*)base;
	return P2ActionThenGetRequestNormalListPcutClose(m);
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
cp_t P2ActionThenGetRequestNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionThenGetRequestNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
