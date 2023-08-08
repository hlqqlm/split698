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
DLT698_45 security request data choice 变化部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/

// var
#include "p2_octet_string.h"

#include "p2_security_request_data_choice.h"
#include "p2_security_request_data_choice.xcp.h"
//#define this_file_id	0x8fe68a1a	// echo -n dlt698_45_security_request_data_choice.c | rhash --simple -


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2SecurityRequestDataChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2SecurityRequestDataChoicePlain, "plain" },	// 明文
	{ kP2SecurityRequestDataChoiceCipher, "cipher" },	// 密文
};
int P2SecurityRequestDataChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2SecurityRequestDataChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2SecurityRequestDataChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2SecurityRequestDataChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2OctetStringPcut kP2OctetStringPcutDefVar = kP2OctetStringPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2SecurityRequestDataChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2SecurityRequestDataNamePlain, sizeof(P2OctetStringPcut), &kP2OctetStringPcutDefVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// 明文
	{ kP2SecurityRequestDataNameCipher, sizeof(P2OctetStringPcut), &kP2OctetStringPcutDefVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// 明文
};
//}}}


//{{{ pcut
cp_t P2SecurityRequestDataChoicePcutOpen(P2SecurityRequestDataChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2SecurityRequestDataChoiceName, 
			kChoiceList, kP2SecurityRequestDataChoiceNum, kVarFactoryInfoList);
}
cp_t P2SecurityRequestDataChoicePcutClose(P2SecurityRequestDataChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2SecurityRequestDataChoicePcutValid(const P2SecurityRequestDataChoicePcut *m)
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
cp_t P2SecurityRequestDataChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityRequestDataChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

