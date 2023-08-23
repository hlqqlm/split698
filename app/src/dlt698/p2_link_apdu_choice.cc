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
DLT698_45 link apdu choice 变化部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)
*/

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_link_request.h"
#include "p2_link_response.h"

#include "p2_link_apdu_choice.h"
//#define this_file_id	0xc03d4629	// echo -n dlt698_45_link_apdu_choice.c | rhash --simple -


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2LinkApduChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2LinkApduChoiceRequest,	"LINK-Request" },	// 预连接请求 [1] LINK-Request，
	{ kP2LinkApduChoiceResponse, "LINK-Response" },	// 预连接响应 [129] LINK-Response
};
int P2LinkApduChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2LinkApduChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2LinkApduChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2LinkApduChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2LinkApduChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2LinkRequestPcut kP2LinkRequestPcutDefVar = kP2LinkRequestPcutDef;
static const P2LinkResponsePcut kP2LinkResponsePcutDefVar = kP2LinkResponsePcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2LinkApduChoiceNum] = {
	// name		size	init	derive_open		derive_close

	{ kP2LinkRequestName, sizeof(P2LinkRequestPcut), &kP2LinkRequestPcutDefVar, P2LinkRequestPcutOpenBase, P2LinkRequestPcutCloseBase },	// 预连接请求 [1] LINK-Request，
	{ kP2LinkResponseName, sizeof(P2LinkResponsePcut), &kP2LinkResponsePcutDefVar, P2LinkResponsePcutOpenBase, P2LinkResponsePcutCloseBase },	// 预连接响应 [129] LINK-Response
	//kPcutFactoryInfoDef("LINK-Response"),    //预连接响应 [129] LINK-Response 
};
//}}}


//{{{ cut
cp_t P2LinkApduChoicePcutOpen(P2LinkApduChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2LinkApduChoiceName, kChoiceList, kP2LinkApduChoiceNum, kVarFactoryInfoList);
}
cp_t P2LinkApduChoicePcutClose(P2LinkApduChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2LinkApduChoicePcutValid(const P2LinkApduChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
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
cp_t P2LinkApduChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2LinkApduChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

