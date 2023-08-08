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
DLT698_45 security apdu choice 变化部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的SECURITY-APDU choice

security_apud就是security_apud_choice，不像client_apdu在client_apdu_choice附加了optional_timetag，因此，security_apud=security_apud_choice。
*/

#include "qos/qmem.h"

// var
#include "p2_security_request.h"

#include "p2_security_apdu_choice.h"
#include "p2_security_apdu_choice.xcp.h"
//#define this_file_id	0x87ea2786	// echo -n dlt698_45_security_apdu_choice.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(1)

// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2SecurityApduChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2SecurityApduChoiceRequest, "security_request" },	// 安全请求 [16] SECURITY-Request，
	{ kP2SecurityApduChoiceResponse, "security_response" },	// 安全响应 [144] SECURITY-Response
};
int P2SecurityApduChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2SecurityApduChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2SecurityApduChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2SecurityApduChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2SecurityApduChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2SecurityRequestPcut kP2SecurityRequestPcutDefVar = kP2SecurityRequestPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2SecurityApduChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2SecurityRequestName, sizeof(P2SecurityRequestPcut), &kP2SecurityRequestPcutDefVar, P2SecurityRequestPcutOpenBase, P2SecurityRequestPcutCloseBase },	// 安全请求 [16] SECURITY-Request，
	kPcutFactoryInfoDef("SECURITY-Response"),				// 安全响应 [144] SECURITY-Response
};
//}}}


//{{{ pcut
cp_t P2SecurityApduChoicePcutOpen(P2SecurityApduChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2SecurityApduNameChoice, kChoiceList, kP2SecurityApduChoiceNum, kVarFactoryInfoList);
}
cp_t P2SecurityApduChoicePcutClose(P2SecurityApduChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2SecurityApduChoicePcutValid(const P2SecurityApduChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}
//}}}


#if 0
//{{{ qpack
cp_t P2SecurityApduChoiceQpackOpen(P2SecurityApduChoiceQpack *m)
{
	return P2ChoiceQpackOpen(&m->base, P2_SECURITY_APDU_CHOICE_NAME, kChoiceList, kP2SecurityApduChoiceNum);
}
cp_t P2SecurityApduChoiceQpackClose(P2SecurityApduChoiceQpack *m)
{
	return P2ChoiceQpackClose(&m->base);
}
cp_t P2SecurityApduChoiceQpackValid(const P2SecurityApduChoiceQpack *m)
{
	ifer(P2ChoiceQpackValid(&m->base));
	return 0;
}
//}}}
#endif


cp_t P2SecurityApduChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2SecurityApduNameChoice, PfillItemOffsetFix, kP2SecurityApduChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2SecurityApduNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2SecurityApduVarOffset, sub_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVar), &kFiVar));

	return 0;
}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	// 10001105020003001002002000020020040200000110482f84cf401c9d15856ad5722d050e6e
	const char whole[] = "\x10\x00\x11\x05\x02\x00\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00\x00\x01\x10\x48\x2f\x84\xcf\x40\x1c\x9d\x15\x85\x6a\xd5\x72\x2d\x05\x0e\x6e";
	const int whole_size = sizeof(whole) - 1;
	const char data[] = "\x00\x11\x05\x02\x00\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00\x00";
	const int data_size = sizeof(data) - 1;
	const char verify[] = "\x01\x10\x48\x2f\x84\xcf\x40\x1c\x9d\x15\x85\x6a\xd5\x72\x2d\x05\x0e\x6e";
	const int verify_size = sizeof(verify) - 1;

	/*
security_request: 001105020003001002002000020020040200000110482f84cf401c9d15856ad5722d050e6e
	data: 00110502000300100200200002002004020000
		security_request_data_choice: 00
		plain: 110502000300100200200002002004020000
			datatype: 
			varlen: 11
			content: 0502000300100200200002002004020000
	verify: 0110482f84cf401c9d15856ad5722d050e6e
		security_request_verify_choice: 01
		rn: 10482f84cf401c9d15856ad5722d050e6e
			datatype: 
			varlen: 10
			content: 482f84cf401c9d15856ad5722d050e6e
	*/
	
	ifbr(0 == MallocCnt());
	P2SecurityApduChoicePcut sac = kP2SecurityApduChoicePcutDef;
	ifer(P2SecurityApduChoicePcutOpen(&sac));
	Pcut * const m = &sac.choice.base;

	//const uint8_t security_request_data_choice_ite = 

	if (PRINT_PART_IN_TEST_EN)
		PcutAllPrint(m, 0, whole);

	ifer(P2SecurityApduChoicePcutClose(&sac));
	ifbr(0 == MallocCnt());
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
cp_t P2SecurityApduChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityApduChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

