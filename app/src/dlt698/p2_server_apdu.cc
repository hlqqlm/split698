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

20230407-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的Server-APDU
*/
#include <string.h>

#include "qos/qmem.h"
#include "p2_server_apdu.h"
#include "p2_server_apdu.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ServerApduCutNum)


// {{{ choice
uint8_t P2ServerApduChoice(const char *whole)
{
	return whole[kP2ServerApduChoiceOffset];
}

#define LenChoice	PcutItemLenBySub	
static int OffsetChoice(Pcut *cut ,int ix, const char *whole) { return kP2ServerApduChoiceOffset; }
#define ValidChoice	PcutItemValidBySub
//}}}


// {{{ optional_follow_report
#define LenOptionalFollowReport		PcutItemLenBySub
#define OffsetOptionalFollowReport	PcutItemOffsetDef
#define ValidOptionalFollowReport	PcutItemValidBySub
// 用sub来填写data
//}}}


// {{{ optional_timetag
#define LenOptionalTimetag		PcutItemLenBySub
#define OffsetOptionalTimetag	PcutItemOffsetDef
#define ValidOptionalTimetag	PcutItemValidBySub
// 用sub来填写data
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ServerApduNameChoice, LenChoice, OffsetChoice, ValidChoice, NULL },
	{ kP2ServerApduNameFollowReport, LenOptionalFollowReport, OffsetOptionalFollowReport, ValidOptionalFollowReport, NULL },
	{ kP2ServerApduNameTimetag, LenOptionalTimetag, OffsetOptionalTimetag, ValidOptionalTimetag, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ServerApduCutIxChoice]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ServerApduCutIxOptionalFollowReport]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ServerApduCutIxOptionalTimetag]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2ServerApduPcutValid(const P2ServerApduPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2ServerApduChoicePcutValid(&m->choice_cut));

	ifer(P2OptionalPcutValid(&m->optional_follow_report));
	ifer(P2FollowReportPcutValid(&m->follow_report_cut));

	ifer(P2OptionalPcutValid(&m->optional_timetag));
	ifer(P2TimetagPcutValid(&m->timetag_cut));
	return 0;
}
cp_t P2ServerApduPcutOpen(P2ServerApduPcut *m)
{
	ifer(P2ServerApduChoicePcutOpen(&m->choice_cut));

	ifer(P2FollowReportPcutOpen(&m->follow_report_cut));
	ifer(P2OptionalPcutOpen(&m->optional_follow_report, &m->follow_report_cut.base, kP2ServerApduNameFollowReport));

	ifer(P2TimetagPcutOpen(&m->timetag_cut));
	ifer(P2OptionalPcutOpen(&m->optional_timetag, &m->timetag_cut.base, kP2ServerApduNameTimetag));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	PcutSubSet(&m->base, kP2ServerApduCutIxChoice, &m->choice_cut.choice.base, NULL);
	PcutSubSet(&m->base, kP2ServerApduCutIxOptionalFollowReport, &m->optional_follow_report.base, NULL);
	PcutSubSet(&m->base, kP2ServerApduCutIxOptionalTimetag, &m->optional_timetag.base, NULL);
	return 0;
}
cp_t P2ServerApduPcutClose(P2ServerApduPcut *m)
{
	dve(P2ServerApduPcutValid(m));

	PcutSubSet(&m->base, kP2ServerApduCutIxChoice, NULL, NULL);
	PcutSubSet(&m->base, kP2ServerApduCutIxOptionalFollowReport, NULL, NULL);
	PcutSubSet(&m->base, kP2ServerApduCutIxOptionalTimetag, NULL, NULL);

	ifer(PcutClose(&m->base));
	ifer(P2ServerApduChoicePcutClose(&m->choice_cut));
	ifer(P2OptionalPcutClose(&m->optional_follow_report));
	ifer(P2FollowReportPcutClose(&m->follow_report_cut));
	ifer(P2OptionalPcutClose(&m->optional_timetag));
	ifer(P2TimetagPcutClose(&m->timetag_cut));
	return 0;
}
//}}}


//{{{ fill
cp_t P2ServerApduFillInit(Pfill *m, uint8_t choice, Pfill *sub_choice_var, Pfill *follow_report, Pfill *timetag)
{
	dvb(NULL != sub_choice_var);

	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ServerApduNameChoice, PfillItemOffsetFix, kP2ServerApduChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiChoiceVar = PFILL_ITEM(kP2ServerApduNameChoiceVar, PfillItemOffsetFix, PfillItemProcessBySub, 1, sub_choice_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoiceVar), &kFiChoiceVar));

	const uint8_t optional_follow_report = ((NULL == follow_report) ? 0 : 1);
	const P2OptionalFillItemOptional kFiOptionalFollowReport = kP2OptionalFillItemOptional(kP2ServerApduNameFollowReportOptional, PfillItemOffsetFollow, 0, optional_follow_report);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiOptionalFollowReport), &kFiOptionalFollowReport));

	if (NULL != follow_report)
	{
		const PfillItem kFiSub = PFILL_ITEM(kP2ServerApduNameFollowReport, PfillItemOffsetFollow, PfillItemProcessBySub, 0, follow_report);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiSub), &kFiSub));
	}

	const uint8_t optional_timetag = ((NULL == timetag) ? 0 : 1);
	const P2OptionalFillItemOptional kFiOptionalTimetag = kP2OptionalFillItemOptional(kP2ServerApduNameTimetagOptional, PfillItemOffsetFollow, 0, optional_timetag);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiOptionalTimetag), &kFiOptionalTimetag));

	if (NULL != timetag)
	{
		const PfillItem kFiSub = PFILL_ITEM(kP2ServerApduNameTimetag, PfillItemOffsetFollow, PfillItemProcessBySub, 0, timetag);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiSub), &kFiSub));
	}

	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
#include "dlt698_45_get_request.h"
#include "dlt698_45_get_request_normal.h"
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestQpack(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ServerApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ServerApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

