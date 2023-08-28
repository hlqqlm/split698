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
用来描述dlt698_45中的SetResponse
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_set_response_normal.h"

#include "p2_set_response.h"
#include "p2_set_response.xcp.h"


#define TEST_EN				(0)

 
// {{{ choice
static const P2Choice kChoiceList[kP2SetResponseChoiceNum] = {
	// choice	name
	{ kP2SetResponseChoiceNormal, "SetResponseNormal" }, // 设置一个对象属性的响应 [1] SetResponseNormal，
	{ kP2SetResponseChoiceNormalList, "SetResponseNormalList" }, // 设置若干个对象属性的响应 [2] SetResponseNormalList，
	{ kP2SetResponseChoiceSetThenGetResponseNormalList, "SetThenGetResponseNormalList" }, // 设置的确认信息以及读取的响应 [3] SetThenGetResponseNormalList
};
int P2SetResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kP2SetResponseChoiceNum, choice);
}
uint8_t P2SetResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kP2SetResponseChoiceNum, choice_ix);
}
cp_t P2SetResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kP2SetResponseChoiceNum, choice_ix);
}
cp_t P2SetResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kP2SetResponseChoiceNum, choice);
}

uint8_t P2SetResponseChoice(const char *whole)
{
	const char *mem = whole + kP2SetResponseChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2SetResponseNormalPcut kP2SetResponseNormalPcutDefVar = kP2SetResponseNormalPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2SetResponseChoiceNum] = {
	{ kP2SetResponseNormalName, sizeof(P2SetResponseNormalPcut), &kP2SetResponseNormalPcutDefVar, P2SetResponseNormalPcutOpenBase, P2SetResponseNormalPcutCloseBase },	// 设置一个对象属性的响应 [1] SetResponseNormal，
	kPcutFactoryInfoDef("SetResponseNormalList"), // 设置若干个对象属性的响应 [2] SetResponseNormalList，
	kPcutFactoryInfoDef("SetThenGetResponseNormalList"), // 设置的确认信息以及读取的响应 [3] SetThenGetResponseNormalList
	// kPcutFactoryInfoDef("example"), // 
};
cp_t P2SetResponsePcutOpen(P2SetResponsePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2SetResponseChoiceName, kChoiceList, kP2SetResponseChoiceNum, kVarFactoryInfoList);
}
cp_t P2SetResponsePcutClose(P2SetResponsePcut *m)
{
	dve(P2SetResponsePcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2SetResponsePcutValid(const P2SetResponsePcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2SetResponsePcutOpenBase(Pcut *base)
{
	P2SetResponsePcut *m = (P2SetResponsePcut*)base;
	return P2SetResponsePcutOpen(m);
}
cp_t P2SetResponsePcutCloseBase(Pcut *base)
{
	P2SetResponsePcut *m = (P2SetResponsePcut*)base;
	return P2SetResponsePcutClose(m);
}
//}}}


//{{{ fill
cp_t P2SetResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2SetResponseChoiceName, PfillItemOffsetFix, kP2SetResponseChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2SetResponseName, PfillItemOffsetFix, PfillItemProcessBySub, kP2SetResponseVarOffset, sub_var);
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
static cp_t TestQpack(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2SetResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SetResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


