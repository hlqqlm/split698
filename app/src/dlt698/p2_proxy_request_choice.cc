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
DLT698_45 proxy request choice 变化部分报文解析
*/

// var
#include "p2_proxy_request_trans_command_request.h"


#include "p2_proxy_request_choice.h"
#include "p2_proxy_request_choice.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)
#define kChoiceNum				(kP2ProxyRequestChoiceNum)


// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ProxyRequestChoiceGetRequestList, "ProxyGetRequestList" },		// 请求代理读取若干个服务器的若干个对象属性 [1] ProxyGetRequestList
	{ kP2ProxyRequestChoiceGetRequestRecord, "ProxyGetRequestRecord" },		// 请求代理读取一个服务器的一个记录型对象属性 [2] ProxyGetRequestRecord
	{ kP2ProxyRequestChoiceSetRequestList, "ProxySetRequestList" },		// 请求代理设置若干个服务器的若干个对象属性 [3] ProxySetRequestList
	{ kP2ProxyRequestChoiceSetThenGetRequestList, "ProxySetThenGetRequestList" },		// 请求代理设置后读取若干个服务器的若干个对象属性 [4] ProxySetThenGetRequestList
	{ kP2ProxyRequestChoiceActionRequestList, "ProxyActionRequestList" },		// 请求代理操作若干个服务器的若干个对象方法 [5] ProxyActionRequestList
	{ kP2ProxyRequestChoiceActionThenGetRequestList, "ProxyActionThenGetRequestList" },		// 请求代理操作后读取若干个服务器的若干个对象方法和属性 [6] ProxyActionThenGetRequestList
	{ kP2ProxyRequestChoiceTransCommandRequest, "ProxyTransCommandRequest" },		// 请求代理透明转发 命令 [7] ProxyTransCommandRequest
	{ kP2ProxyRequestChoiceInnerTransCommandRequest, "ProxyInnerTransCommandRequest" },		// 请求代理终端内部转发命令 [8] ProxyInnerTransCommandRequest
};
int P2ProxyRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ProxyRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ProxyRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ProxyRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ProxyRequestChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2ProxyRequestTransCommandRequestPcut kP2ProxyRequestTransCommandRequestPcutDefVar = kP2ProxyRequestTransCommandRequestPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	kPcutFactoryInfoDef("ProxyGetRequestList"),		// 请求代理读取若干个服务器的若干个对象属性 [1] ProxyGetRequestList
	kPcutFactoryInfoDef("ProxyGetRequestRecord"),		// 请求代理读取一个服务器的一个记录型对象属性 [2] ProxyGetRequestRecord
	kPcutFactoryInfoDef("ProxySetRequestList"),		// 请求代理设置若干个服务器的若干个对象属性 [3] ProxySetRequestList
	kPcutFactoryInfoDef("ProxySetThenGetRequestList"),		// 请求代理设置后读取若干个服务器的若干个对象属性 [4] ProxySetThenGetRequestList
	kPcutFactoryInfoDef("ProxyActionRequestList"),		// 请求代理操作若干个服务器的若干个对象方法 [5] ProxyActionRequestList
	kPcutFactoryInfoDef("ProxyActionThenGetRequestList"),		// 请求代理操作后读取若干个服务器的若干个对象方法和属性 [6] ProxyActionThenGetRequestList
	{ "ProxyTransCommandRequest", sizeof(P2ProxyRequestTransCommandRequestPcut), &kP2ProxyRequestTransCommandRequestPcutDefVar, P2ProxyRequestTransCommandRequestPcutOpenBase, P2ProxyRequestTransCommandRequestPcutCloseBase },	// 请求代理透明转发 命令 [7] ProxyTransCommandRequest
	kPcutFactoryInfoDef("ProxyInnerTransCommandRequest"),		// 请求代理终端内部转发命令 [8] ProxyInnerTransCommandRequest
	// kPcutFactoryInfoDef("example"),		// 
};
//}}}


//{{{ cut
cp_t P2ProxyRequestChoicePcutOpen(P2ProxyRequestChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ProxyRequestNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2ProxyRequestChoicePcutClose(P2ProxyRequestChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ProxyRequestChoicePcutValid(const P2ProxyRequestChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}


// 定义用子类base来open/close父类的函数
//PCUT_DEFINE_OPEN_BASE(P2ProxyRequestChoicePcut);
//PCUT_DEFINE_CLOSE_BASE(P2ProxyRequestChoicePcut);

cp_t P2ProxyRequestChoicePcutOpenBase(Pcut *base)
{
	P2ProxyRequestChoicePcut *m = (P2ProxyRequestChoicePcut*)base;
	return P2ProxyRequestChoicePcutOpen(m);
}
cp_t P2ProxyRequestChoicePcutCloseBase(Pcut *base)
{
	P2ProxyRequestChoicePcut *m = (P2ProxyRequestChoicePcut*)base;
	return P2ProxyRequestChoicePcutClose(m);
}
//}}}



//
cp_t P2ProxyRequestChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ProxyRequestNameChoice, PfillItemOffsetFix, kP2ProxyRequestChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ProxyRequestNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2ProxyRequestVarOffset, sub_var);
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
cp_t P2ProxyRequestChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ProxyRequestChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

