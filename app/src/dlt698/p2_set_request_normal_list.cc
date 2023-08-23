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

用来描述dlt698_45中的set request normal list
*/
#include "p2_set_request_normal_list.h"
#include "p2_set_request_normal_list.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2SetRequestNormalListCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2SetRequestNormalListPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2SetRequestNormalListPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_oad_data
#define LenSequenceOfOadData		PcutItemLenBySub
static int OffsetSequenceOfOadData(Pcut *cut, int ix, const char *whole) { return kP2SetRequestNormalListSequenceOfOadDataOffset; }
#define ValidSequenceOfOadData		PcutItemValidBySub
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
	{ kP2SetRequestNormalListNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2SetRequestNormalListNameSequenceOfOadData, LenSequenceOfOadData, OffsetSequenceOfOadData, ValidSequenceOfOadData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetRequestNormalListCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetRequestNormalListCutIxSequenceOfOadData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SetRequestNormalListPcutOpen(P2SetRequestNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2OadDataPcutOpen(&m->oad_data_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_oad_data_cut, &m->oad_data_cut.base, kP2SetRequestNormalListNameSequenceOfOadData));
	PcutSubSet(&m->base, kP2SetRequestNormalListCutIxSequenceOfOadData, &m->sequence_of_oad_data_cut.base, NULL);
	return 0;
}
cp_t P2SetRequestNormalListPcutClose(P2SetRequestNormalListPcut *m)
{
	dve(P2SetRequestNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2SetRequestNormalListCutIxSequenceOfOadData, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_oad_data_cut));
	ifer(P2OadDataPcutClose(&m->oad_data_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2SetRequestNormalListPcutValid(const P2SetRequestNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_oad_data_cut));
	ifer(P2OadDataPcutValid(&m->oad_data_cut));
	return 0;
}


cp_t P2SetRequestNormalListPcutOpenBase(Pcut *base)
{
	P2SetRequestNormalListPcut *m = (P2SetRequestNormalListPcut*)base;
	return P2SetRequestNormalListPcutOpen(m);
}
cp_t P2SetRequestNormalListPcutCloseBase(Pcut *base)
{
	P2SetRequestNormalListPcut *m = (P2SetRequestNormalListPcut*)base;
	return P2SetRequestNormalListPcutClose(m);
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
cp_t P2SetRequestNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SetRequestNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
