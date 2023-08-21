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
DLT698_45 action request choice 变化部分报文解析
*/

// var
#include "p2_action_request_normal.h"
#include "p2_action_request_normal_list.h"

#include "p2_action_request_choice.h"
#include "p2_action_request_choice.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)
#define kChoiceNum				(kP2ActionRequestChoiceNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ActionRequestChoiceNormal, "action_request_normal" },	// 请求操作一个对象方法 [1] ActionRequestNormal，
	{ kP2ActionRequestChoiceNormalList, "action_request_normal_list" },	// 请求操作若干个对象方法 [2] ActionRequestNormalList，
	{ kP2ActionRequestChoiceActionThenGetRequestNormalList, "action_then_get_request_normal_list" },	// 请求操作若干个对象方法后读取若干个对象属性 [3] ActionThenGetRequestNormalList
};
int P2ActionRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ActionRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ActionRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ActionRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ActionRequestChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2ActionRequestNormalPcut kP2ActionRequestNormalPcutDefVar = kP2ActionRequestNormalPcutDef;
static const P2ActionRequestNormalListPcut kP2ActionRequestNormalListPcutDefVar = kP2ActionRequestNormalListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2ActionRequestNormalName, sizeof(P2ActionRequestNormalPcut), &kP2ActionRequestNormalPcutDefVar, P2ActionRequestNormalPcutOpenBase, P2ActionRequestNormalPcutCloseBase },	// 请求操作一个对象方法 [1] ActionRequestNormal，
	{ kP2ActionRequestNormalListName, sizeof(P2ActionRequestNormalListPcut), &kP2ActionRequestNormalListPcutDefVar, P2ActionRequestNormalListPcutOpenBase, P2ActionRequestNormalListPcutCloseBase },	// 请求操作若干个对象方法 [2] ActionRequestNormalList，
	kPcutFactoryInfoDef("ActionThenGetRequestNormalList"),				// 请求操作若干个对象方法后读取若干个对象属性 [3] ActionThenGetRequestNormalList
};
//}}}


//{{{ pcut
cp_t P2ActionRequestChoicePcutOpen(P2ActionRequestChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ActionRequestNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2ActionRequestChoicePcutClose(P2ActionRequestChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ActionRequestChoicePcutValid(const P2ActionRequestChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}


// 定义用子类base来open/close父类的函数
//PCUT_DEFINE_OPEN_BASE(P2ActionRequestChoicePcut);
//PCUT_DEFINE_CLOSE_BASE(P2ActionRequestChoicePcut);

cp_t P2ActionRequestChoicePcutOpenBase(Pcut *base)
{
	P2ActionRequestChoicePcut *m = (P2ActionRequestChoicePcut*)base;
	return P2ActionRequestChoicePcutOpen(m);
}
cp_t P2ActionRequestChoicePcutCloseBase(Pcut *base)
{
	P2ActionRequestChoicePcut *m = (P2ActionRequestChoicePcut*)base;
	return P2ActionRequestChoicePcutClose(m);
}
//}}}



//
cp_t P2ActionRequestChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ActionRequestNameChoice, PfillItemOffsetFix, kP2ActionRequestChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ActionRequestNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2ActionRequestVarOffset, sub_var);
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
cp_t P2ActionRequestChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ActionRequestChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

