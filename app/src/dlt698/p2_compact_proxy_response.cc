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

20230828-----------------------------------------------------------------------
huanglin 创建.
用来描述dlt698_45中的CompactProxyResponse
*/
#include <string.h>

// var
#include "p2_compact_proxy_get_response_normal.h"

#include "p2_compact_proxy_response.h"
#include "p2_compact_proxy_response.xcp.h"


#define TEST_EN				(0)
#define kThisChoiceNum		(kP2CompactProxyResponseChoiceNum)

 
// {{{ choice
static const P2Choice kChoiceList[kThisChoiceNum] = {
	// choice	name
	{ kP2CompactProxyResponseChoiceNormal, "CompactProxyGetResponseNormal" }, // 代理精简读取服务器的若干个对象属性的响应 [1] CompactProxyGetResponseNormal，
	{ kP2CompactProxyResponseChoiceRecord, "CompactProxyGetResponseRecord" }, // 代理精简读取服务器的一个记录型对象属性的响应 [2] CompactProxyGetResponseRecord，
	{ kP2CompactProxyResponseChoiceSet, "CompactProxySetResponse" }, // 代理精简设置服务器的若干个对象属性的响应 [3] CompactProxySetResponse，
	{ kP2CompactProxyResponseChoiceSetThenGet, "CompactProxySetThenGetResponse" }, // 代理精简设置后读取服务器的若干个对象属性的响应 [4] CompactProxySetThenGetResponse，
	{ kP2CompactProxyResponseChoiceAction, "CompactProxyActionResponse" }, // 代理精简操作服务器的若干个对象方法的响应 [5] CompactProxyActionResponse，
	{ kP2CompactProxyResponseChoiceActionThenGet, "CompactProxyActionThenGetResponse" }, // 代理精简操作后读取服务器的若干个对象方法和属性的响应 [6] CompactProxyActionThenGetResponse
};
int P2CompactProxyResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kThisChoiceNum, choice);
}
uint8_t P2CompactProxyResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kThisChoiceNum, choice_ix);
}
cp_t P2CompactProxyResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kThisChoiceNum, choice_ix);
}
cp_t P2CompactProxyResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kThisChoiceNum, choice);
}

uint8_t P2CompactProxyResponseChoice(const char *whole)
{
	const char *mem = whole + kP2CompactProxyResponseChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2CompactProxyGetResponseNormalPcut kP2CompactProxyGetResponseNormalPcutDefVar = kP2CompactProxyGetResponseNormalPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kThisChoiceNum] = {
	{ kP2CompactProxyGetResponseNormalName, sizeof(P2CompactProxyGetResponseNormalPcut), &kP2CompactProxyGetResponseNormalPcutDefVar, P2CompactProxyGetResponseNormalPcutOpenBase, P2CompactProxyGetResponseNormalPcutCloseBase },	// 代理精简读取服务器的若干个对象属性的响应 [1] CompactProxyGetResponseNormal，
	kPcutFactoryInfoDef("CompactProxyGetResponseRecord"),	// 代理精简读取服务器的一个记录型对象属性的响应 [2] CompactProxyGetResponseRecord，
	kPcutFactoryInfoDef("CompactProxySetResponse"),	// 代理精简设置服务器的若干个对象属性的响应 [3] CompactProxySetResponse，
	kPcutFactoryInfoDef("CompactProxySetThenGetResponse"),	// 代理精简设置后读取服务器的若干个对象属性的响应 [4] CompactProxySetThenGetResponse，
	kPcutFactoryInfoDef("CompactProxyActionResponse"),	// 代理精简操作服务器的若干个对象方法的响应 [5] CompactProxyActionResponse，
	kPcutFactoryInfoDef("CompactProxyActionThenGetResponse"),	// 代理精简操作后读取服务器的若干个对象方法和属性的响应 [6] CompactProxyActionThenGetResponse
	// kPcutFactoryInfoDef("example"),	// 
};
cp_t P2CompactProxyResponsePcutOpen(P2CompactProxyResponsePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2CompactProxyResponseChoiceName, kChoiceList, kThisChoiceNum, kVarFactoryInfoList);
}
cp_t P2CompactProxyResponsePcutClose(P2CompactProxyResponsePcut *m)
{
	dve(P2CompactProxyResponsePcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2CompactProxyResponsePcutValid(const P2CompactProxyResponsePcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2CompactProxyResponsePcutOpenBase(Pcut *base)
{
	P2CompactProxyResponsePcut *m = (P2CompactProxyResponsePcut*)base;
	return P2CompactProxyResponsePcutOpen(m);
}
cp_t P2CompactProxyResponsePcutCloseBase(Pcut *base)
{
	P2CompactProxyResponsePcut *m = (P2CompactProxyResponsePcut*)base;
	return P2CompactProxyResponsePcutClose(m);
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
cp_t P2CompactProxyResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


