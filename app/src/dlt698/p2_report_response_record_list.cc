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

用来描述dlt698_45中的report response record_list
*/
#include <string.h>

#include "p2_report_response_record_list.h"
#include "p2_report_response_record_list.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ReportResponseRecordListCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2ReportResponseRecordListPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2ReportResponseRecordListPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_oad
#define LenSequenceOfOad		PcutItemLenBySub	
static int OffsetSequenceOfOad(Pcut *cut, int ix, const char *whole) { return kP2ReportResponseRecordListSequenceOfOadOffset; }
#define ValidSequenceOfOad		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ReportResponseRecordListNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2ReportResponseRecordListNameSequenceOfOad, LenSequenceOfOad, OffsetSequenceOfOad, ValidSequenceOfOad, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ReportResponseRecordListCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ReportResponseRecordListCutIxSequenceOfOad]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ReportResponseRecordListPcutOpen(P2ReportResponseRecordListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2SequenceOfPcutOpen(&m->sequence_of_oad, OAD_MEM_SIZE, "oad"));
	PcutSubSet(&m->base, kP2ReportResponseRecordListCutIxSequenceOfOad, &m->sequence_of_oad.base, "sequence_of_oad");
	return 0;
}
cp_t P2ReportResponseRecordListPcutClose(P2ReportResponseRecordListPcut *m)
{
	dve(P2ReportResponseRecordListPcutValid(m));

	PcutSubSet(&m->base, kP2ReportResponseRecordListCutIxSequenceOfOad, NULL, NULL);

	ifer(PcutClose(&m->base));
	ifer(P2SequenceOfPcutClose(&m->sequence_of_oad));
	return 0;
}
cp_t P2ReportResponseRecordListPcutValid(const P2ReportResponseRecordListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfPcutValid(&m->sequence_of_oad));
	return 0;
}


cp_t P2ReportResponseRecordListPcutOpenBase(Pcut *base)
{
	P2ReportResponseRecordListPcut *m = (P2ReportResponseRecordListPcut*)base;
	return P2ReportResponseRecordListPcutOpen(m);
}
cp_t P2ReportResponseRecordListPcutCloseBase(Pcut *base)
{
	P2ReportResponseRecordListPcut *m = (P2ReportResponseRecordListPcut*)base;
	return P2ReportResponseRecordListPcutClose(m);
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
cp_t P2ReportResponseRecordListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReportResponseRecordListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
