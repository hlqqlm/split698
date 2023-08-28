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

用来描述dlt698_45中的P2ARecordRow
*/
#include "p2_a_record_row.h"
#include "p2_a_record_row.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2ARecordRowCutNum)



// {{{ sequence_of_oad_data
#define LenSequenceOfData		PcutItemLenBySub
static int OffsetSequenceOfData(Pcut *cut, int ix, const char *whole) { return kP2ARecordRowSequenceOfDataOffset; }
#define ValidSequenceOfData		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ARecordRowNameSequenceOfData, LenSequenceOfData, OffsetSequenceOfData, ValidSequenceOfData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ARecordRowCutIxSequenceOfData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ARecordRowPcutOpen(P2ARecordRowPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DataChoicePcutOpen(&m->data));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_data, &m->data.choice.base, kP2ARecordRowNameSequenceOfData));
	PcutSubSet(&m->base, kP2ARecordRowCutIxSequenceOfData, &m->sequence_of_data.base, NULL);
	return 0;
}
cp_t P2ARecordRowPcutClose(P2ARecordRowPcut *m)
{
	dve(P2ARecordRowPcutValid(m));

	PcutSubSet(&m->base, kP2ARecordRowCutIxSequenceOfData, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_data));
	ifer(P2DataChoicePcutClose(&m->data));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ARecordRowPcutValid(const P2ARecordRowPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_data));
	ifer(P2DataChoicePcutValid(&m->data));
	return 0;
}


cp_t P2ARecordRowPcutOpenBase(Pcut *base)
{
	P2ARecordRowPcut *m = (P2ARecordRowPcut*)base;
	return P2ARecordRowPcutOpen(m);
}
cp_t P2ARecordRowPcutCloseBase(Pcut *base)
{
	P2ARecordRowPcut *m = (P2ARecordRowPcut*)base;
	return P2ARecordRowPcutClose(m);
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
cp_t P2ARecordRowTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ARecordRowTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
