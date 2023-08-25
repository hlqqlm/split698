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
#include "p2_report_notification_list.h"
#include "p2_report_notification_list.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ReportNotificationListCutNum)



// {{{ piid-acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ReportNotificationListPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2ReportNotificationListPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_a_result_normal
#define LenSequenceOf		PcutItemLenBySub
static int OffsetSequenceOf(Pcut *cut, int ix, const char *whole) { return kP2ReportNotificationListSequenceOfOffset; }
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
	{ kP2ReportNotificationListNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2ReportNotificationListNameSequenceOf, LenSequenceOf, OffsetSequenceOf, ValidSequenceOf, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ReportNotificationListCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ReportNotificationListCutIxSequenceOf]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ReportNotificationListPcutOpen(P2ReportNotificationListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2AResultNormalPcutOpen(&m->a_result_normal));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of, &m->a_result_normal.base, kP2ReportNotificationListNameSequenceOf));
	PcutSubSet(&m->base, kP2ReportNotificationListCutIxSequenceOf, &m->sequence_of.base, NULL);
	return 0;
}
cp_t P2ReportNotificationListPcutClose(P2ReportNotificationListPcut *m)
{
	dve(P2ReportNotificationListPcutValid(m));

	PcutSubSet(&m->base, kP2ReportNotificationListCutIxSequenceOf, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of));
	ifer(P2AResultNormalPcutClose(&m->a_result_normal));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ReportNotificationListPcutValid(const P2ReportNotificationListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of));
	ifer(P2AResultNormalPcutValid(&m->a_result_normal));
	return 0;
}


cp_t P2ReportNotificationListPcutOpenBase(Pcut *base)
{
	P2ReportNotificationListPcut *m = (P2ReportNotificationListPcut*)base;
	return P2ReportNotificationListPcutOpen(m);
}
cp_t P2ReportNotificationListPcutCloseBase(Pcut *base)
{
	P2ReportNotificationListPcut *m = (P2ReportNotificationListPcut*)base;
	return P2ReportNotificationListPcutClose(m);
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
cp_t P2ReportNotificationListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReportNotificationListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
