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

20230825-----------------------------------------------------------------------
huanglin 创建.
用来描述dlt698_45中的CompactSetResponse
*/
#include <string.h>

// var
#include "p2_compact_set_response_normal.h"

#include "p2_compact_set_response.h"
#include "p2_compact_set_response.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2CompactSetResponseChoiceNum)

 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2CompactSetResponseChoiceNormal, "CompactSetResponseNormal" }, // 紧凑设置一个对象属性的响应 [1] CompactSetResponseNormal，
	{ kP2CompactSetResponseChoiceNormalList, "CompactSetResponseNormalList" }, // 紧凑设置多个对象属性的响应 [2] CompactSetResponseNormalList
};
int P2CompactSetResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2CompactSetResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2CompactSetResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2CompactSetResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2CompactSetResponseChoice(const char *whole)
{
	const char *mem = whole + kP2CompactSetResponseChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2CompactSetResponseNormalPcut kP2CompactSetResponseNormalPcutDefVar = kP2CompactSetResponseNormalPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2CompactSetResponseNormalName, sizeof(P2CompactSetResponseNormalPcut), &kP2CompactSetResponseNormalPcutDefVar, P2CompactSetResponseNormalPcutOpenBase, P2CompactSetResponseNormalPcutCloseBase },	// 紧凑设置一个对象属性的响应 [1] CompactSetResponseNormal，
	kPcutFactoryInfoDef("CompactSetResponseNormalList"),	// 紧凑设置多个对象属性的响应 [2] CompactSetResponseNormalList
};
cp_t P2CompactSetResponsePcutOpen(P2CompactSetResponsePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2CompactSetResponseChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2CompactSetResponsePcutClose(P2CompactSetResponsePcut *m)
{
	dve(P2CompactSetResponsePcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2CompactSetResponsePcutValid(const P2CompactSetResponsePcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2CompactSetResponsePcutOpenBase(Pcut *base)
{
	P2CompactSetResponsePcut *m = (P2CompactSetResponsePcut*)base;
	return P2CompactSetResponsePcutOpen(m);
}
cp_t P2CompactSetResponsePcutCloseBase(Pcut *base)
{
	P2CompactSetResponsePcut *m = (P2CompactSetResponsePcut*)base;
	return P2CompactSetResponsePcutClose(m);
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
cp_t P2CompactSetResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactSetResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


