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
用来描述dlt698_45中的CompactGetResponse
*/
#include <string.h>

// var
#include "p2_compact_get_response_normal.h"
#include "p2_compact_get_response_normal_list.h"
#include "p2_compact_get_response_record.h"

#include "p2_compact_get_response.h"
#include "p2_compact_get_response.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2CompactGetResponseChoiceNum)

 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2CompactGetResponseChoiceNormal, "CompactGetResponseNormal" }, // 紧凑读取一个对象属性的响应 [1] CompactGetResponseNormal，
	{ kP2CompactGetResponseChoiceNormalList, "CompactGetResponseNormalList" }, // 紧凑读取多个对象属性的响应 [2] CompactGetResponseNormalList，
	{ kP2CompactGetResponseChoiceRecord, "CompactGetResponseRecord" }, // 紧凑读取一个记录型对象属性的响应 [3] CompactGetResponseRecord
};
int P2CompactGetResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2CompactGetResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2CompactGetResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2CompactGetResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2CompactGetResponseChoice(const char *whole)
{
	const char *mem = whole + kP2CompactGetResponseChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2CompactGetResponseNormalPcut kP2CompactGetResponseNormalPcutDefVar = kP2CompactGetResponseNormalPcutDef;
static const P2CompactGetResponseNormalListPcut kP2CompactGetResponseNormalListPcutDefVar = kP2CompactGetResponseNormalListPcutDef;
static const P2CompactGetResponseRecordPcut kP2CompactGetResponseRecordPcutDefVar = kP2CompactGetResponseRecordPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2CompactGetResponseNormalName, sizeof(P2CompactGetResponseNormalPcut), &kP2CompactGetResponseNormalPcutDefVar, P2CompactGetResponseNormalPcutOpenBase, P2CompactGetResponseNormalPcutCloseBase },	// 紧凑读取一个对象属性的响应 [1] CompactGetResponseNormal，
	{ kP2CompactGetResponseNormalListName, sizeof(P2CompactGetResponseNormalListPcut), &kP2CompactGetResponseNormalListPcutDefVar, P2CompactGetResponseNormalListPcutOpenBase, P2CompactGetResponseNormalListPcutCloseBase },	// 紧凑读取多个对象属性的响应 [2] CompactGetResponseNormalList，
	{ kP2CompactGetResponseRecordName, sizeof(P2CompactGetResponseRecordPcut), &kP2CompactGetResponseRecordPcutDefVar, P2CompactGetResponseRecordPcutOpenBase, P2CompactGetResponseRecordPcutCloseBase },	// 紧凑读取一个记录型对象属性的响应 [3] CompactGetResponseRecord
};
cp_t P2CompactGetResponsePcutOpen(P2CompactGetResponsePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2CompactGetResponseChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2CompactGetResponsePcutClose(P2CompactGetResponsePcut *m)
{
	dve(P2CompactGetResponsePcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2CompactGetResponsePcutValid(const P2CompactGetResponsePcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2CompactGetResponsePcutOpenBase(Pcut *base)
{
	P2CompactGetResponsePcut *m = (P2CompactGetResponsePcut*)base;
	return P2CompactGetResponsePcutOpen(m);
}
cp_t P2CompactGetResponsePcutCloseBase(Pcut *base)
{
	P2CompactGetResponsePcut *m = (P2CompactGetResponsePcut*)base;
	return P2CompactGetResponsePcutClose(m);
}
//}}}


//{{{ fill
cp_t P2CompactGetResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2CompactGetResponseChoiceName, PfillItemOffsetFix, kP2CompactGetResponseChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2CompactGetResponseName, PfillItemOffsetFix, PfillItemProcessBySub, kP2CompactGetResponseVarOffset, sub_var);
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
cp_t P2CompactGetResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactGetResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


