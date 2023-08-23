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
DLT698_45 set request choice 变化部分报文解析
*/

// var
#include "p2_set_request_normal.h"
#include "p2_set_request_normal_list.h"
#include "p2_set_then_get_request_normal_list.h"

#include "p2_set_request_choice.h"
#include "p2_set_request_choice.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2SetRequestChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2SetRequestChoiceNormal, "set_request_normal" },	// 请求操作一个对象方法 [1] SetRequestNormal，
	{ kP2SetRequestChoiceNormalList, "set_request_normal_list" },	// 请求操作若干个对象方法 [2] SetRequestNormalList，
	{ kP2SetRequestChoiceSetThenGetRequestNormalList, "set_then_get_request_normal_list" },	// 请求操作若干个对象方法后读取若干个对象属性 [3] SetThenGetRequestNormalList
};
int P2SetRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2SetRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2SetRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2SetRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2SetRequestChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2SetRequestNormalPcut kP2SetRequestNormalPcutDefVar = kP2SetRequestNormalPcutDef;
static const P2SetRequestNormalListPcut kP2SetRequestNormalListPcutDefVar = kP2SetRequestNormalListPcutDef;
static const P2SetThenGetRequestNormalListPcut kP2SetThenGetRequestNormalListPcutDefVar = kP2SetThenGetRequestNormalListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2SetRequestChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2SetRequestNormalName, sizeof(P2SetRequestNormalPcut), &kP2SetRequestNormalPcutDefVar, P2SetRequestNormalPcutOpenBase, P2SetRequestNormalPcutCloseBase },	// 请求操作一个对象方法 [1] SetRequestNormal，
	{ kP2SetRequestNormalListName, sizeof(P2SetRequestNormalListPcut), &kP2SetRequestNormalListPcutDefVar, P2SetRequestNormalListPcutOpenBase, P2SetRequestNormalListPcutCloseBase },	// 请求操作若干个对象方法 [2] SetRequestNormalList，
	{ kP2SetThenGetRequestNormalListName, sizeof(P2SetThenGetRequestNormalListPcut), &kP2SetThenGetRequestNormalListPcutDefVar, P2SetThenGetRequestNormalListPcutOpenBase, P2SetThenGetRequestNormalListPcutCloseBase },	// 请求操作若干个对象方法后读取若干个对象属性 [3] SetThenGetRequestNormalList
	// kPcutFactoryInfoDef("SetThenGetRequestNormalList"),				// 请求操作若干个对象方法后读取若干个对象属性 [3] SetThenGetRequestNormalList
};
//}}}


//{{{ cut
cp_t P2SetRequestChoicePcutOpen(P2SetRequestChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2SetRequestNameChoice, kChoiceList, kP2SetRequestChoiceNum, kVarFactoryInfoList);
}
cp_t P2SetRequestChoicePcutClose(P2SetRequestChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2SetRequestChoicePcutValid(const P2SetRequestChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}


// 定义用子类base来open/close父类的函数
//PCUT_DEFINE_OPEN_BASE(P2SetRequestChoicePcut);
//PCUT_DEFINE_CLOSE_BASE(P2SetRequestChoicePcut);
cp_t P2SetRequestChoicePcutOpenBase(Pcut *base)
{
	P2SetRequestChoicePcut *m = (P2SetRequestChoicePcut*)base;
	return P2SetRequestChoicePcutOpen(m);
}
cp_t P2SetRequestChoicePcutCloseBase(Pcut *base)
{
	P2SetRequestChoicePcut *m = (P2SetRequestChoicePcut*)base;
	return P2SetRequestChoicePcutClose(m);
}
//}}}



//
cp_t P2SetRequestChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2SetRequestNameChoice, PfillItemOffsetFix, kP2SetRequestChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2SetRequestNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2SetRequestVarOffset, sub_var);
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
cp_t P2SetRequestChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SetRequestChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

