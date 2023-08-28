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

20230815-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 proxy response choice 变化部分报文解析
*/

// var
#include "p2_proxy_response_trans_command_response.h"


#include "p2_proxy_response_choice.h"
#include "p2_proxy_response_choice.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)
#define kChoiceNum				(kP2ProxyResponseChoiceNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ProxyResponseChoiceGetResponseList, "ProxyGetResponseList" },		// 代理读取若干个服务器的若干个对象属性的响应 [1] ProxyGetResponseList，
	{ kP2ProxyResponseChoiceGetResponseRecord, "ProxyGetResponseRecord" },		// 代理读取一个服务器的一个记录型对象属性的响应 [2] ProxyGetResponseRecord，
	{ kP2ProxyResponseChoiceSetResponseList, "ProxySetResponseList" },		// 代理设置若干个服务器的若干个对象属性的响应 [3] ProxySetResponseList，
	{ kP2ProxyResponseChoiceSetThenGetResponseList, "ProxySetThenGetResponseList" },	// 代理设置后读取若干个服务器的若干个对象属性的响应 [4] ProxySetThenGetResponseList，
	{ kP2ProxyResponseChoiceActionResponseList, "ProxyActionResponseList" },	// 代理操作若干个服务器的若干个对象方法的响应 [5] ProxyActionResponseList，
	{ kP2ProxyResponseChoiceActionThenGetResponseList, "ProxyActionThenGetResponseList" },		// 代理操作后读取若干个服务器的若干个对象方法和属性的响应 [6] ProxyActionThenGetResponseList，
	{ kP2ProxyResponseChoiceTransCommandResponse, "ProxyTransCommandResponse" },	// 代理透明转发命令的响应 [7] ProxyTransCommandResponse
	{ kP2ProxyResponseChoiceInnerTransCommandResponse, "ProxyInnerTransCommandResponse" },		// 代理终端内部转发命令的响应 [8] ProxyInnerTransCommandResponse
};
int P2ProxyResponseChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ProxyResponseChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ProxyResponseChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ProxyResponseChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ProxyResponseChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2ProxyResponseTransCommandResponsePcut kP2ProxyResponseTransCommandResponsePcutDefVar = kP2ProxyResponseTransCommandResponsePcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	kPcutFactoryInfoDef("ProxyGetResponseList"),			// 代理读取若干个服务器的若干个对象属性的响应 [1] ProxyGetResponseList，
	kPcutFactoryInfoDef("ProxyGetResponseRecord"),			// 代理读取一个服务器的一个记录型对象属性的响应 [2] ProxyGetResponseRecord，
	kPcutFactoryInfoDef("ProxySetResponseList"),			// 代理设置若干个服务器的若干个对象属性的响应 [3] ProxySetResponseList，
	kPcutFactoryInfoDef("ProxySetThenGetResponseList"),		// 代理设置后读取若干个服务器的若干个对象属性的响应 [4] ProxySetThenGetResponseList，
	kPcutFactoryInfoDef("ProxyActionResponseList"),			// 代理操作若干个服务器的若干个对象方法的响应 [5] ProxyActionResponseList，
	kPcutFactoryInfoDef("ProxyActionThenGetResponseList"),	// 代理操作后读取若干个服务器的若干个对象方法和属性的响应 [6] ProxyActionThenGetResponseList，
	{ "ProxyTransCommandResponse", sizeof(P2ProxyResponseTransCommandResponsePcut), &kP2ProxyResponseTransCommandResponsePcutDefVar, P2ProxyResponseTransCommandResponsePcutOpenBase, P2ProxyResponseTransCommandResponsePcutCloseBase },	// 代理透明转发命令的响应 [7] ProxyTransCommandResponse
	kPcutFactoryInfoDef("ProxyInnerTransCommandResponse"),	// 代理终端内部转发命令的响应 [8] ProxyInnerTransCommandResponse
	// kPcutFactoryInfoDef("example"),			// 
};
//}}}


//{{{ cut
cp_t P2ProxyResponseChoicePcutOpen(P2ProxyResponseChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ProxyResponseNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2ProxyResponseChoicePcutClose(P2ProxyResponseChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ProxyResponseChoicePcutValid(const P2ProxyResponseChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}



cp_t P2ProxyResponseChoicePcutOpenBase(Pcut *base)
{
	P2ProxyResponseChoicePcut *m = (P2ProxyResponseChoicePcut*)base;
	return P2ProxyResponseChoicePcutOpen(m);
}
cp_t P2ProxyResponseChoicePcutCloseBase(Pcut *base)
{
	P2ProxyResponseChoicePcut *m = (P2ProxyResponseChoicePcut*)base;
	return P2ProxyResponseChoicePcutClose(m);
}
//}}}



//
cp_t P2ProxyResponseChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ProxyResponseNameChoice, PfillItemOffsetFix, kP2ProxyResponseChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ProxyResponseNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2ProxyResponseVarOffset, sub_var);
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
cp_t P2ProxyResponseChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ProxyResponseChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

