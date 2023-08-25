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
用来描述dlt698_45中的ReportNotification
*/
#include <string.h>

// var
#include "p2_report_notification_list.h"

#include "p2_report_notification.h"
#include "p2_report_notification.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2ReportNotificationChoiceNum)
 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2ReportNotificationChoiceList, "ReportNotificationList" }, // 通知上报若干个对象属性 [1] ReportNotificationList，
	{ kP2ReportNotificationChoiceRecordList, "ReportNotificationRecordList" }, // 通知上报若干个记录型对象属性 [2] ReportNotificationRecordList，
	{ kP2ReportNotificationChoiceTransData, "ReportNotificationTransData" }, // 通知上报透明数据 [3] ReportNotificationTransData，
	{ kP2ReportNotificationChoiceClientService, "ReportRequestClientService" }, // 请求客户机服务 [4] ReportRequestClientService，
	{ kP2ReportNotificationChoiceSimplifyRecord, "ReportNotificationSimplifyRecord" }, // 通知上报精简的记录型对象属性 [6] ReportNotificationSimplifyRecord
};
int P2ReportNotificationChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2ReportNotificationChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2ReportNotificationChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2ReportNotificationChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2ReportNotificationChoice(const char *whole)
{
	const char *mem = whole + kP2ReportNotificationChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2ReportNotificationListPcut kP2ReportNotificationListPcutDefVar = kP2ReportNotificationListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2ReportNotificationListName, sizeof(P2ReportNotificationListPcut), &kP2ReportNotificationListPcutDefVar, P2ReportNotificationListPcutOpenBase, P2ReportNotificationListPcutCloseBase },	// 通知上报若干个对象属性 [1] ReportNotificationList，
	kPcutFactoryInfoDef("ReportNotificationRecordList"),		// 通知上报若干个记录型对象属性 [2] ReportNotificationRecordList，
	kPcutFactoryInfoDef("ReportNotificationTransData"),		// 通知上报透明数据 [3] ReportNotificationTransData，
	kPcutFactoryInfoDef("ReportRequestClientService"),		// 请求客户机服务 [4] ReportRequestClientService，
	kPcutFactoryInfoDef("ReportNotificationSimplifyRecord"),// 通知上报精简的记录型对象属性 [6] ReportNotificationSimplifyRecord
	// kPcutFactoryInfoDef("example"),		// 
};
cp_t P2ReportNotificationPcutOpen(P2ReportNotificationPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ReportNotificationChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2ReportNotificationPcutClose(P2ReportNotificationPcut *m)
{
	dve(P2ReportNotificationPcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2ReportNotificationPcutValid(const P2ReportNotificationPcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2ReportNotificationPcutOpenBase(Pcut *base)
{
	P2ReportNotificationPcut *m = (P2ReportNotificationPcut*)base;
	return P2ReportNotificationPcutOpen(m);
}
cp_t P2ReportNotificationPcutCloseBase(Pcut *base)
{
	P2ReportNotificationPcut *m = (P2ReportNotificationPcut*)base;
	return P2ReportNotificationPcutClose(m);
}
//}}}


//{{{ fill
cp_t P2ReportNotificationFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ReportNotificationChoiceName, PfillItemOffsetFix, kP2ReportNotificationChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ReportNotificationName, PfillItemOffsetFix, PfillItemProcessBySub, kP2ReportNotificationVarOffset, sub_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVar), &kFiVar));

	return 0;
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
cp_t P2ReportNotificationTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReportNotificationTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


