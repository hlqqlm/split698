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
用来描述dlt698_45中的ActionResponse
*/
#include <string.h>

// var
#include "p2_action_response_normal.h"
#include "p2_action_response_normal_list.h"
#include "p2_action_then_get_response_normal_list.h"

#include "p2_action_response.h"
#include "p2_action_response.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2ActionResponseChoiceNum)
 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2ActionResponseChoiceNormal, "ActionResponseNormal" }, // 操作一个对象方法的响应 [1] ActionResponseNormal，
	{ kP2ActionResponseChoiceNormalList, "ActionResponseNormalList" }, // 操作若干个对象方法的响应 [2] ActionResponseNormalList，
	{ kP2ActionResponseChoiceActionThenGetResponseNormalList, "ActionThenGetResponseNormalList" }, // 操作若干个对象方法后读取若干个属性的响应 [3] ActionThenGetResponseNormalList
};
int P2ActionResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2ActionResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2ActionResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2ActionResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2ActionResponseChoice(const char *whole)
{
	const char *mem = whole + kP2ActionResponseChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2ActionResponseNormalPcut kP2ActionResponseNormalPcutDefVar = kP2ActionResponseNormalPcutDef;
static const P2ActionResponseNormalListPcut kP2ActionResponseNormalListPcutDefVar = kP2ActionResponseNormalListPcutDef;
static const P2ActionThenGetResponseNormalListPcut kP2ActionThenGetResponseNormalListPcutDefVar = kP2ActionThenGetResponseNormalListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2ActionResponseNormalName
		, sizeof(P2ActionResponseNormalPcut), &kP2ActionResponseNormalPcutDefVar
		, P2ActionResponseNormalPcutOpenBase, P2ActionResponseNormalPcutCloseBase },	
	// 操作一个对象方法的响应 [1] ActionResponseNormal，

	{ kP2ActionResponseNormalListName
		, sizeof(P2ActionResponseNormalListPcut), &kP2ActionResponseNormalListPcutDefVar
		, P2ActionResponseNormalListPcutOpenBase, P2ActionResponseNormalListPcutCloseBase },	
	// 操作若干个对象方法的响应 [2] ActionResponseNormalList，

	{ kP2ActionThenGetResponseNormalListName
		, sizeof(P2ActionThenGetResponseNormalListPcut), &kP2ActionThenGetResponseNormalListPcutDefVar
		, P2ActionThenGetResponseNormalListPcutOpenBase, P2ActionThenGetResponseNormalListPcutCloseBase },	
	// 操作若干个对象方法后读取若干个属性的响应 [3] ActionThenGetResponseNormalList
};
cp_t P2ActionResponsePcutOpen(P2ActionResponsePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ActionResponseChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2ActionResponsePcutClose(P2ActionResponsePcut *m)
{
	dve(P2ActionResponsePcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2ActionResponsePcutValid(const P2ActionResponsePcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2ActionResponsePcutOpenBase(Pcut *base)
{
	P2ActionResponsePcut *m = (P2ActionResponsePcut*)base;
	return P2ActionResponsePcutOpen(m);
}
cp_t P2ActionResponsePcutCloseBase(Pcut *base)
{
	P2ActionResponsePcut *m = (P2ActionResponsePcut*)base;
	return P2ActionResponsePcutClose(m);
}
//}}}


//{{{ fill
cp_t P2ActionResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ActionResponseChoiceName, PfillItemOffsetFix, kP2ActionResponseChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ActionResponseName, PfillItemOffsetFix, PfillItemProcessBySub, kP2ActionResponseVarOffset, sub_var);
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
cp_t P2ActionResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


