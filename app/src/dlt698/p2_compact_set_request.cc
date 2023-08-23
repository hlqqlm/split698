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
用来描述dlt698_45中的CompactSetRequest
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_set_request_normal.h"
#include "p2_set_request_normal_list.h"

#include "p2_compact_set_request.h"
#include "p2_compact_set_request.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2CompactSetRequestChoiceNum)
 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2CompactSetRequestChoiceNormal, "SetRequestNormal" }, // 请求紧凑设置一个对象属性 [1] SetRequestNormal，
	{ kP2CompactSetRequestChoiceNormalList, "SetRequestNormalList" }, // 请求紧凑设置多个对象属性 [2] SetRequestNormalList
};
int P2CompactSetRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2CompactSetRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2CompactSetRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2CompactSetRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2CompactSetRequestChoice(const char *whole)
{
	const char *mem = whole + kP2CompactSetRequestChoiceOffset;
	return *mem;
}
//}}}


//{{{ pcut
static const P2SetRequestNormalPcut kP2SetRequestNormalPcutDefVar = kP2SetRequestNormalPcutDef;
static const P2SetRequestNormalListPcut kP2SetRequestNormalListPcutDefVar = kP2SetRequestNormalListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2SetRequestNormalName, sizeof(P2SetRequestNormalPcut), &kP2SetRequestNormalPcutDefVar, P2SetRequestNormalPcutOpenBase, P2SetRequestNormalPcutCloseBase },	// 请求紧凑设置一个对象属性 [1] SetRequestNormal，
	{ kP2SetRequestNormalListName, sizeof(P2SetRequestNormalListPcut), &kP2SetRequestNormalListPcutDefVar, P2SetRequestNormalListPcutOpenBase, P2SetRequestNormalListPcutCloseBase },	// 请求紧凑设置多个对象属性 [2] SetRequestNormalList
	//kPcutFactoryInfoDef("LINK-Response"),    //预连接响应 [129] LINK-Response 
};
cp_t P2CompactSetRequestPcutOpen(P2CompactSetRequestPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2CompactSetRequestChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2CompactSetRequestPcutClose(P2CompactSetRequestPcut *m)
{
	dve(P2CompactSetRequestPcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2CompactSetRequestPcutValid(const P2CompactSetRequestPcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2CompactSetRequestPcutOpenBase(Pcut *base)
{
	P2CompactSetRequestPcut *m = (P2CompactSetRequestPcut*)base;
	return P2CompactSetRequestPcutOpen(m);
}
cp_t P2CompactSetRequestPcutCloseBase(Pcut *base)
{
	P2CompactSetRequestPcut *m = (P2CompactSetRequestPcut*)base;
	return P2CompactSetRequestPcutClose(m);
}
//}}}


//{{{ fill
cp_t P2CompactSetRequestFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2CompactSetRequestChoiceName, PfillItemOffsetFix, kP2CompactSetRequestChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2CompactSetRequestName, PfillItemOffsetFix, PfillItemProcessBySub, kP2CompactSetRequestVarOffset, sub_var);
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
cp_t P2CompactSetRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactSetRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


