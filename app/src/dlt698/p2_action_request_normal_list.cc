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
#include "p2_action_request_normal_list.h"
#include "p2_action_request_normal_list.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ActionRequestNormalListCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalListPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalListPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_omd_data
#define LenSequenceOfOmdData		PcutItemLenBySub
static int OffsetSequenceOfOmdData(Pcut *cut, int ix, const char *whole) { return kP2ActionRequestNormalListSequenceOfOmdDataOffset; }
#define ValidSequenceOfOmdData		PcutItemValidBySub
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
	{ kP2ActionRequestNormalListNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2ActionRequestNormalListNameSequenceOfOmdData, LenSequenceOfOmdData, OffsetSequenceOfOmdData, ValidSequenceOfOmdData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionRequestNormalListCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ActionRequestNormalListCutIxSequenceOfOmdData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ActionRequestNormalListPcutOpen(P2ActionRequestNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2OmdDataPcutOpen(&m->omd_data_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_omd_data_cut, &m->omd_data_cut.base, kP2ActionRequestNormalListNameSequenceOfOmdData));
	PcutSubSet(&m->base, kP2ActionRequestNormalListCutIxSequenceOfOmdData, &m->sequence_of_omd_data_cut.base, NULL);
	return 0;
}
cp_t P2ActionRequestNormalListPcutClose(P2ActionRequestNormalListPcut *m)
{
	dve(P2ActionRequestNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2ActionRequestNormalListCutIxSequenceOfOmdData, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_omd_data_cut));
	ifer(P2OmdDataPcutClose(&m->omd_data_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ActionRequestNormalListPcutValid(const P2ActionRequestNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_omd_data_cut));
	ifer(P2OmdDataPcutValid(&m->omd_data_cut));
	return 0;
}


cp_t P2ActionRequestNormalListPcutOpenBase(Pcut *base)
{
	P2ActionRequestNormalListPcut *m = (P2ActionRequestNormalListPcut*)base;
	return P2ActionRequestNormalListPcutOpen(m);
}
cp_t P2ActionRequestNormalListPcutCloseBase(Pcut *base)
{
	P2ActionRequestNormalListPcut *m = (P2ActionRequestNormalListPcut*)base;
	return P2ActionRequestNormalListPcutClose(m);
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
cp_t P2ActionRequestNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionRequestNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
