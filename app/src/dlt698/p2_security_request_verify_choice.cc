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

20230516-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 security request verify choice 变化部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/

// var
#include "p2_octet_string.h"

#include "p2_security_request_verify_choice.h"
#include "p2_security_request_verify_choice.xcp.h"
//#define this_file_id	0x77663aa4	// echo -n dlt698_45_security_request_verify_choice.c | rhash --simple -


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2SecurityRequestVerifyChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2SecurityRequestVerifyChoiceSidMac, "sid_mac" },	// 数据验证码 [0] SID_MAC，
	{ kP2SecurityRequestVerifyChoiceRn, "rn" },				// 随机数 [1] RN，
	{ kP2SecurityRequestVerifyChoiceRnMac, "rn_mac" },		// 随机数+MAC [2] RN_MAC，
	{ kP2SecurityRequestVerifyChoiceSid, "sid" },			// 安全标识 [3] SID
};
int P2SecurityRequestVerifyChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2SecurityRequestVerifyChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2SecurityRequestVerifyChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2SecurityRequestVerifyChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2OctetStringPcut kP2OctetStringPcutDefVar = kP2OctetStringPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2SecurityRequestVerifyChoiceNum] = {
	// name		size	init	derive_open		derive_close
	kPcutFactoryInfoDef("SID_MAC"),	// 数据验证码 [0] SID_MAC，
	{ "rn", sizeof(P2OctetStringPcut), &kP2OctetStringPcutDefVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// 随机数 [1] RN， RN就是octet-string
	kPcutFactoryInfoDef("RN_MAC"),	// 随机数+MAC [2] RN_MAC，
	kPcutFactoryInfoDef("SID"),	// 安全标识 [3] SID
	// kPcutFactoryInfoDef("example"),	// 
};
//}}}


//{{{ cut
cp_t P2SecurityRequestVerifyChoicePcutOpen(P2SecurityRequestVerifyChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, P2_SECURITY_REQUEST_VERIFY_CHOICE_NAME, kChoiceList, kP2SecurityRequestVerifyChoiceNum, kVarFactoryInfoList);
}
cp_t P2SecurityRequestVerifyChoicePcutClose(P2SecurityRequestVerifyChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2SecurityRequestVerifyChoicePcutValid(const P2SecurityRequestVerifyChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}
//}}}


#if 0
//{{{ qpack
cp_t P2SecurityRequestVerifyChoiceQpackOpen(P2SecurityRequestVerifyChoiceQpack *m)
{
	return P2ChoiceQpackOpen(&m->base, P2_SECURITY_REQUEST_VERIFY_CHOICE_NAME, 
			kChoiceList, kP2SecurityRequestVerifyChoiceNum);
}
cp_t P2SecurityRequestVerifyChoiceQpackClose(P2SecurityRequestVerifyChoiceQpack *m)
{
	return P2ChoiceQpackClose(&m->base);
}
cp_t P2SecurityRequestVerifyChoiceQpackValid(const P2SecurityRequestVerifyChoiceQpack *m)
{
	ifer(P2ChoiceQpackValid(&m->base));
	return 0;
}
//}}}
#endif

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
cp_t P2SecurityRequestVerifyChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityRequestVerifyChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

