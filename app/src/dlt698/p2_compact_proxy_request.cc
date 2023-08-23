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
用来描述dlt698_45中的CompactProxyRequest
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_compact_proxy_get_request_normal.h"

#include "p2_compact_proxy_request.h"
#include "p2_compact_proxy_request.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2CompactProxyRequestChoiceNum)
 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2CompactProxyRequestChoiceGetRequestNormal, "CompactProxyGetRequestNormal" }, // 请求紧凑代理读取服务器的若干个对象属性 [1] CompactProxyGetRequestNormal，
	{ kP2CompactProxyRequestChoiceGetRequestRecord, "CompactProxyGetRequestRecord" }, // 请求紧凑代理读取服务器的一个记录型对象属性 [2] CompactProxyGetRequestRecord，
	{ kP2CompactProxyRequestChoiceSetRequestNormal, "CompactProxySetRequestNormal" }, // 请求紧凑代理设置服务器的若干个对象属性 [3] CompactProxySetRequestNormal，
	{ kP2CompactProxyRequestChoiceSetThenGetRequest, "CompactProxySetThenGetRequest" }, // 请求紧凑代理设置后读取服务器的若干个对象属性 [4] CompactProxySetThenGetRequest，
	{ kP2CompactProxyRequestChoiceActionRequest, "CompactProxyActionRequest" }, // 请求紧凑代理操作服务器的若干个对象方法 [5] CompactProxyActionRequest，
	{ kP2CompactProxyRequestChoiceActionThenGetRequest, "CompactProxyActionThenGetRequest" }, // 请求紧凑代理操作后读取服务器的若干个对象方法和属性 [6] CompactProxyActionThenGetRequest，
};
int P2CompactProxyRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2CompactProxyRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2CompactProxyRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2CompactProxyRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2CompactProxyRequestChoice(const char *whole)
{
	const char *mem = whole + kP2CompactProxyRequestChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2CompactProxyGetRequestNormalPcut kP2CompactProxyGetRequestNormalPcutDefVar = kP2CompactProxyGetRequestNormalPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2CompactProxyGetRequestNormalName, sizeof(P2CompactProxyGetRequestNormalPcut), &kP2CompactProxyGetRequestNormalPcutDefVar, P2CompactProxyGetRequestNormalPcutOpenBase, P2CompactProxyGetRequestNormalPcutCloseBase },	// 请求紧凑代理读取服务器的若干个对象属性 [1] CompactProxyGetRequestNormal，
	kPcutFactoryInfoDef("CompactProxyGetRequestRecord"),    // 请求紧凑代理读取服务器的一个记录型对象属性 [2] CompactProxyGetRequestRecord，
	kPcutFactoryInfoDef("CompactProxySetRequestNormal"),    // 请求紧凑代理设置服务器的若干个对象属性 [3] CompactProxySetRequestNormal，
	kPcutFactoryInfoDef("CompactProxySetThenGetRequest"),    // 请求紧凑代理设置后读取服务器的若干个对象属性 [4] CompactProxySetThenGetRequest，
	kPcutFactoryInfoDef("CompactProxyActionRequest"),    // 请求紧凑代理操作服务器的若干个对象方法 [5] CompactProxyActionRequest，
	kPcutFactoryInfoDef("CompactProxyActionThenGetRequest"),    // 请求紧凑代理操作后读取服务器的若干个对象方法和属性 [6] CompactProxyActionThenGetRequest，
	//kPcutFactoryInfoDef("LINK-Response"),    //预连接响应 [129] LINK-Response 
};
cp_t P2CompactProxyRequestPcutOpen(P2CompactProxyRequestPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2CompactProxyRequestChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2CompactProxyRequestPcutClose(P2CompactProxyRequestPcut *m)
{
	dve(P2CompactProxyRequestPcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2CompactProxyRequestPcutValid(const P2CompactProxyRequestPcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2CompactProxyRequestPcutOpenBase(Pcut *base)
{
	P2CompactProxyRequestPcut *m = (P2CompactProxyRequestPcut*)base;
	return P2CompactProxyRequestPcutOpen(m);
}
cp_t P2CompactProxyRequestPcutCloseBase(Pcut *base)
{
	P2CompactProxyRequestPcut *m = (P2CompactProxyRequestPcut*)base;
	return P2CompactProxyRequestPcutClose(m);
}
//}}}


//{{{ fill
cp_t P2CompactProxyRequestFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2CompactProxyRequestChoiceName, PfillItemOffsetFix, kP2CompactProxyRequestChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2CompactProxyRequestName, PfillItemOffsetFix, PfillItemProcessBySub, kP2CompactProxyRequestVarOffset, sub_var);
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
cp_t P2CompactProxyRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


