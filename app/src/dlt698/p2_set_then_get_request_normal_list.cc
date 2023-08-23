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

20230823-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的set request normal list
*/
#include "p2_set_then_get_request_normal_list.h"
#include "p2_set_then_get_request_normal_list.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2SetThenGetRequestNormalListCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2SetThenGetRequestNormalListPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2SetThenGetRequestNormalListPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_oad_data_oad_unsigned
#define LenSequenceOf		PcutItemLenBySub
static int OffsetSequenceOf(Pcut *cut, int ix, const char *whole) { return kP2SetThenGetRequestNormalListSequenceOfOffset; }
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
	{ kP2SetThenGetRequestNormalListNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2SetThenGetRequestNormalListNameSequenceOf, LenSequenceOf, OffsetSequenceOf, ValidSequenceOf, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetThenGetRequestNormalListCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SetThenGetRequestNormalListCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SetThenGetRequestNormalListPcutOpen(P2SetThenGetRequestNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2OadDataOadUnsignedPcutOpen(&m->oad_data_oad_unsigned_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_cut, &m->oad_data_oad_unsigned_cut.base, kP2SetThenGetRequestNormalListNameSequenceOf));
	PcutSubSet(&m->base, kP2SetThenGetRequestNormalListCutIxSequenceOf, &m->sequence_of_cut.base, NULL);
	return 0;
}
cp_t P2SetThenGetRequestNormalListPcutClose(P2SetThenGetRequestNormalListPcut *m)
{
	dve(P2SetThenGetRequestNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2SetThenGetRequestNormalListCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_cut));
	ifer(P2OadDataOadUnsignedPcutClose(&m->oad_data_oad_unsigned_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2SetThenGetRequestNormalListPcutValid(const P2SetThenGetRequestNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_cut));
	ifer(P2OadDataOadUnsignedPcutValid(&m->oad_data_oad_unsigned_cut));
	return 0;
}


cp_t P2SetThenGetRequestNormalListPcutOpenBase(Pcut *base)
{
	P2SetThenGetRequestNormalListPcut *m = (P2SetThenGetRequestNormalListPcut*)base;
	return P2SetThenGetRequestNormalListPcutOpen(m);
}
cp_t P2SetThenGetRequestNormalListPcutCloseBase(Pcut *base)
{
	P2SetThenGetRequestNormalListPcut *m = (P2SetThenGetRequestNormalListPcut*)base;
	return P2SetThenGetRequestNormalListPcutClose(m);
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
cp_t P2SetThenGetRequestNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SetThenGetRequestNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
