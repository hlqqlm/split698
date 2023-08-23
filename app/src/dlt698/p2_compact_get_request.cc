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
用来描述dlt698_45中的CompactGetRequest
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_get_request_normal.h"
#include "p2_get_request_normal_list.h"
#include "p2_get_request_record.h"

#include "p2_compact_get_request.h"
#include "p2_compact_get_request.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2CompactGetRequestChoiceNum)
 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2CompactGetRequestChoiceNormal, "GetRequestNormal" }, // 请求紧凑读取一个对象属性 [1] GetRequestNormal，
	{ kP2CompactGetRequestChoiceNormalList, "GetRequestNormalList" }, // 请求紧凑读取多个对象属性 [2] GetRequestNormalList，
	{ kP2CompactGetRequestChoiceRecord, "GetRequestRecord" }, // 请求紧凑读取一个记录型对象属性 [3] GetRequestRecord
};
int P2CompactGetRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2CompactGetRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2CompactGetRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2CompactGetRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2CompactGetRequestChoice(const char *whole)
{
	const char *mem = whole + kP2CompactGetRequestChoiceOffset;
	return *mem;
}
//}}}


//{{{ pcut
static const P2GetRequestNormalPcut kP2GetRequestNormalPcutDefVar = kP2GetRequestNormalPcutDef;
static const P2GetRequestNormalListPcut kP2GetRequestNormalListPcutDefVar = kP2GetRequestNormalListPcutDef;
static const P2GetRequestRecordPcut kP2GetRequestRecordPcutDefVar = kP2GetRequestRecordPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2GetRequestNormalName, sizeof(P2GetRequestNormalPcut), &kP2GetRequestNormalPcutDefVar, P2GetRequestNormalPcutOpenBase, P2GetRequestNormalPcutCloseBase },	// 请求紧凑读取一个对象属性 [1] GetRequestNormal，
	{ kP2GetRequestNormalListName, sizeof(P2GetRequestNormalListPcut), &kP2GetRequestNormalListPcutDefVar, P2GetRequestNormalListPcutOpenBase, P2GetRequestNormalListPcutCloseBase },	// 请求紧凑读取多个对象属性 [2] GetRequestNormalList，
	{ kP2GetRequestRecordName, sizeof(P2GetRequestRecordPcut), &kP2GetRequestRecordPcutDefVar, P2GetRequestRecordPcutOpenBase, P2GetRequestRecordPcutCloseBase },	// 请求紧凑读取一个记录型对象属性 [3] GetRequestRecord
	//kPcutFactoryInfoDef("LINK-Response"),    //预连接响应 [129] LINK-Response 
};
cp_t P2CompactGetRequestPcutOpen(P2CompactGetRequestPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2CompactGetRequestChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2CompactGetRequestPcutClose(P2CompactGetRequestPcut *m)
{
	dve(P2CompactGetRequestPcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2CompactGetRequestPcutValid(const P2CompactGetRequestPcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2CompactGetRequestPcutOpenBase(Pcut *base)
{
	P2CompactGetRequestPcut *m = (P2CompactGetRequestPcut*)base;
	return P2CompactGetRequestPcutOpen(m);
}
cp_t P2CompactGetRequestPcutCloseBase(Pcut *base)
{
	P2CompactGetRequestPcut *m = (P2CompactGetRequestPcut*)base;
	return P2CompactGetRequestPcutClose(m);
}
//}}}


//{{{ fill
cp_t P2CompactGetRequestFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2CompactGetRequestChoiceName, PfillItemOffsetFix, kP2CompactGetRequestChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2CompactGetRequestName, PfillItemOffsetFix, PfillItemProcessBySub, kP2CompactGetRequestVarOffset, sub_var);
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
cp_t P2CompactGetRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactGetRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


