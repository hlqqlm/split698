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

20230407-----------------------------------------------------------------------
huanglin 创建.
用来描述dlt698_45中的GetResponse
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_get_response_normal.h"

#include "p2_get_response.h"
#include "p2_get_response.xcp.h"


#define TEST_EN				(0)

 
// {{{ choice
static const P2Choice kChoiceList[kP2GetResponseChoiceNum] = {
	// choice	name
	{ kP2GetResponseChoiceGetResponseNormal, "get_response_normal" }, // 读取一个对象属性的响应 [1] GetResponseNormal，
	{ kP2GetResponseChoiceGetResponseNormalList, "get_response_normal_list" }, // 读取若干个对象属性的响应 [2] GetResponseNormalList，
	{ kP2GetResponseChoiceGetResponseRecord, "get_response_record" }, // 读取一个记录型对象属性的响应 [3] GetResponseRecord，
	{ kP2GetResponseChoiceGetResponseRecordList, "get_response_record_list" }, // 读取若干个记录型对象属性的响应 [4] GetResponseRecordList，
	{ kP2GetResponseChoiceGetResponseNext, "get_response_next" }, // 读取分帧传输的下一帧的响应 [5] GetResponseNext，
	{ kP2GetResponseChoiceGetResponseMD5, "get_response_md5" }, // 读取一个对象属性的 MD5 值的响应 [6] GetResponseMD5，
	{ kP2GetResponseChoiceGetResponseSimplifyRecord, "get_response_simplify_record" }, // 读取一个精简记录型对象属性请求 [23] GetResponseSimplifyRecord
};
int P2GetResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kP2GetResponseChoiceNum, choice);
}
uint8_t P2GetResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kP2GetResponseChoiceNum, choice_ix);
}
cp_t P2GetResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kP2GetResponseChoiceNum, choice_ix);
}
cp_t P2GetResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kP2GetResponseChoiceNum, choice);
}

uint8_t P2GetResponseChoice(const char *whole)
{
	const char *mem = whole + kP2GetResponseChoiceOffset;
	return *mem;
}
//}}}


//{{{ pcut
static const P2GetResponseNormalPcut kP2GetResponseNormalPcutDefVar = kP2GetResponseNormalPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2GetResponseChoiceNum] = {
	{ kP2GetResponseNormalName, sizeof(P2GetResponseNormalPcut), &kP2GetResponseNormalPcutDefVar, P2GetResponseNormalPcutOpenBase, P2GetResponseNormalPcutCloseBase },	// 读取一个对象属性的响应 [1] GetResponseNormal，
	kPcutFactoryInfoDef("GetResponseNormalList"),	// 读取若干个对象属性的响应 [2] GetResponseNormalList，
	kPcutFactoryInfoDef("GetResponseRecord"),	// 读取一个记录型对象属性的响应 [3] GetResponseRecord，
	kPcutFactoryInfoDef("GetResponseRecordList"),	// 读取若干个记录型对象属性的响应 [4] GetResponseRecordList，
	kPcutFactoryInfoDef("GetResponseNext"),	// 读取分帧传输的下一帧的响应 [5] GetResponseNext，
	kPcutFactoryInfoDef("GetResponseMD5"),	// 读取一个对象属性的 MD5 值的响应 [6] GetResponseMD5，
	kPcutFactoryInfoDef("GetResponseSimplifyRecord"),	// 读取一个精简记录型对象属性请求 [23] GetResponseSimplifyRecord
};
cp_t P2GetResponsePcutOpen(P2GetResponsePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2GetResponseChoiceName, kChoiceList, kP2GetResponseChoiceNum, kVarFactoryInfoList);
}
cp_t P2GetResponsePcutClose(P2GetResponsePcut *m)
{
	dve(P2GetResponsePcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2GetResponsePcutValid(const P2GetResponsePcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2GetResponsePcutOpenBase(Pcut *base)
{
	P2GetResponsePcut *m = (P2GetResponsePcut*)base;
	return P2GetResponsePcutOpen(m);
}
cp_t P2GetResponsePcutCloseBase(Pcut *base)
{
	P2GetResponsePcut *m = (P2GetResponsePcut*)base;
	return P2GetResponsePcutClose(m);
}
//}}}


//{{{ fill
cp_t P2GetResponseFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2GetResponseChoiceName, PfillItemOffsetFix, kP2GetResponseChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2GetResponseName, PfillItemOffsetFix, PfillItemProcessBySub, kP2GetResponseVarOffset, sub_var);
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
cp_t P2GetResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


