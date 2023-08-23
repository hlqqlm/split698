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

20230822-----------------------------------------------------------------------
huanglin 创建.
用来描述dlt698_45中的RSD
*/

// var
#include "p2_null.h"
#include "p2_selector1.h"

#include "p2_rsd.h"
#include "p2_rsd.xcp.h"


#define TEST_EN				(0)
#define kChoiceNum			(kP2RsdNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2RsdNull, "null" },	// 不选择 [0] NULL，
	{ kP2RsdSelector1, "Selector1" },	// 选择方法 1 [1] Selector1，
	{ kP2RsdSelector2, "Selector2" },	// 选择方法 2 [2] Selector2，
	{ kP2RsdSelector3, "Selector3" },	// 选择方法 3 [3] Selector3，
	{ kP2RsdSelector4, "Selector4" },	// 选择方法 4 [4] Selector4，
	{ kP2RsdSelector5, "Selector5" },	// 选择方法 5 [5] Selector5，
	{ kP2RsdSelector6, "Selector6" },	// 选择方法 6 [6] Selector6，
	{ kP2RsdSelector7, "Selector7" },	// 选择方法 7 [7] Selector7，
	{ kP2RsdSelector8, "Selector8" },	// 选择方法 8 [8] Selector8，
	{ kP2RsdSelector9, "Selector9" },	// 选择方法 9 [9] Selector9，
	{ kP2RsdSelector10, "Selector10" },	// 选择方法 10 [10] Selector10，
	{ kP2RsdSelector11, "Selector11" },	// 选择方法 11 [11] Selector11，
};
int P2Rsd2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2RsdIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2RsdIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2RsdValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2RsdStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2NullPcut kP2NullPcutDefVar = kP2NullPcutDef;
static const P2Selector1Pcut kP2Selector1PcutDefVar = kP2Selector1PcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2NullName, sizeof(P2NullPcut), &kP2NullPcutDefVar, P2NullPcutOpenBase, P2NullPcutCloseBase },	// 不选择 [0] NULL，
	{ kP2Selector1Name, sizeof(P2Selector1Pcut), &kP2Selector1PcutDefVar, P2Selector1PcutOpenBase, P2Selector1PcutCloseBase },	// 选择方法 1 [1] Selector1，
	kPcutFactoryInfoDef("Selector2"),	// 选择方法 2 [2] Selector2，
	kPcutFactoryInfoDef("Selector3"),	// 选择方法 3 [3] Selector3，
	kPcutFactoryInfoDef("Selector4"),	// 选择方法 4 [4] Selector4，
	kPcutFactoryInfoDef("Selector5"),	// 选择方法 5 [5] Selector5，
	kPcutFactoryInfoDef("Selector6"),	// 选择方法 6 [6] Selector6，
	kPcutFactoryInfoDef("Selector7"),	// 选择方法 7 [7] Selector7，
	kPcutFactoryInfoDef("Selector8"),	// 选择方法 8 [8] Selector8，
	kPcutFactoryInfoDef("Selector9"),	// 选择方法 9 [9] Selector9，
	kPcutFactoryInfoDef("Selector10"),	// 选择方法 10 [10] Selector10，
	kPcutFactoryInfoDef("Selector11"),	// 选择方法 11 [11] Selector11
};
//}}}


//{{{ pcut
cp_t P2RsdPcutOpen(P2RsdPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2RsdNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2RsdPcutClose(P2RsdPcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2RsdPcutValid(const P2RsdPcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}
//}}}


//{{{ fill
cp_t P2RsdFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2RsdNameChoice, PfillItemOffsetFix, 0, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2RsdNameVar, PfillItemOffsetFix, PfillItemProcessBySub, 1, sub_var);
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
cp_t P2RsdTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2RsdTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

