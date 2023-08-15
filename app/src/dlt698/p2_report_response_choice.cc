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

20230815-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 report response choice 变化部分报文解析
*/

// var
#include "p2_report_response_record_list.h"

#include "p2_report_response_choice.h"
#include "p2_report_response_choice.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)
#define kChoiceNum				(kP2ReportResponseChoiceNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ReportResponseChoiceList, "ReportResponseList" },		// 上报若干个对象属性的响应 [1] ReportResponseList
	{ kP2ReportResponseChoiceRecordList, "ReportResponseRecordList" },	// 上报若干个记录型对象属性的响应 [2] ReportResponseRecordList
	{ kP2ReportResponseChoiceTransData, "ReportResponseTransData" },	// 上报透明数据的响应 [3] ReportResponseTransData
	{ kP2ReportResponseChoiceClientService, "ReportResponseClientService" },	// 请求客户机服务的响应 [4] ReportResponseClientService
	{ kP2ReportResponseChoiceSimplifyRecord, "ReportResponseSimplifyRecord" },	// 上报精简的记录型对象属性的响应 [6] ReportResponseSimplifyRecord
};
int P2ReportResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ReportResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ReportResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ReportResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ReportResponseChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2ReportResponseRecordListPcut kP2ReportResponseRecordListPcutDefVar = kP2ReportResponseRecordListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	kPcutFactoryInfoDef("ReportResponseList"),				// 上报若干个对象属性的响应 [1] ReportResponseList
	{ kP2ReportResponseRecordListName, sizeof(P2ReportResponseRecordListPcut), &kP2ReportResponseRecordListPcutDefVar, P2ReportResponseRecordListPcutOpenBase, P2ReportResponseRecordListPcutCloseBase },	// 上报若干个记录型对象属性的响应 [2] ReportResponseRecordList
	kPcutFactoryInfoDef("ReportResponseTransData"),				// 上报透明数据的响应 [3] ReportResponseTransData
	kPcutFactoryInfoDef("ReportResponseClientService"),			// 请求客户机服务的响应 [4] ReportResponseClientService
	kPcutFactoryInfoDef("ReportResponseSimplifyRecord"),		// 上报精简的记录型对象属性的响应 [6] ReportResponseSimplifyRecord
};
//}}}


//{{{ pcut
cp_t P2ReportResponseChoicePcutOpen(P2ReportResponseChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ReportResponseNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2ReportResponseChoicePcutClose(P2ReportResponseChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ReportResponseChoicePcutValid(const P2ReportResponseChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}


// 定义用子类base来open/close父类的函数
//PCUT_DEFINE_OPEN_BASE(P2ReportResponseChoicePcut);
//PCUT_DEFINE_CLOSE_BASE(P2ReportResponseChoicePcut);
cp_t P2ReportResponseChoicePcutOpenBase(Pcut *base)
{
	P2ReportResponseChoicePcut *m = (P2ReportResponseChoicePcut*)base;
	return P2ReportResponseChoicePcutOpen(m);
}
cp_t P2ReportResponseChoicePcutCloseBase(Pcut *base)
{
	P2ReportResponseChoicePcut *m = (P2ReportResponseChoicePcut*)base;
	return P2ReportResponseChoicePcutClose(m);
}
//}}}



//
cp_t P2ReportResponseChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ReportResponseNameChoice, PfillItemOffsetFix, kP2ReportResponseChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ReportResponseNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2ReportResponseVarOffset, sub_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVar), &kFiVar));

	return 0;
}


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
cp_t P2ReportResponseChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ReportResponseChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

