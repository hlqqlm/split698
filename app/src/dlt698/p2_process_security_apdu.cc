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

20230710-----------------------------------------------------------------------
从p2_process拆分而来，执行security_apdu报文

20230511-----------------------------------------------------------------------
huanglin 创建.
执行dlt698协议
*/
#include <string.h>
#include <algorithm>

#include "qos/qstr.h"
#include "qos/qmem.h"

//#include "exAddrparameter.h"

// dlt698_45

#include "p2_config.h"

#include "p2_double_long_unsigned.h"
#include "p2_double_long.h"
#include "p2_long_unsigned.h"
#include "p2_octet_string.h"
#include "p2_sequence_of_var_len.h"
#include "p2_array.h"


#include "p2_security_apdu_choice.h"
#include "p2_security_request.h"
#include "p2_security_response.h"
#include "p2_security_response_data_choice.h"

#include "p2_process_client_apdu.h"

#include "p2_process_security_apdu.h"
#include "p2_process_security_apdu.xcp.h"
//#define this_file_id	0x  // echo -n p2_process.c | rhash --simple -


#define TEST_EN				(0)
#define PRINT_FILL_EN		(0)			// 是否打印填充帧过程
#define CHECK_MEM_LEAK_EN	(0)			// 是否检查内存泄漏


//{{{ security_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoSecurityRequestDataPlain;
static cp_t PdoSecurityRequestDataPlainProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	// qos_printf("plain\n");
	dvb(ix == kP2ChoicePartIxVar);
	PdoSecurityRequestDataPlain *derive = (PdoSecurityRequestDataPlain*)doa;
	PfillRepository *fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在security-request-data中, plain是当前的choice
	P2SecurityRequestDataChoicePcut *srdc = (P2SecurityRequestDataChoicePcut*)cut;
	P2OctetStringPcut *os = (P2OctetStringPcut*)P2ChoicePcutVar(&srdc->choice);
	dvb(os == (void*)PcutFindSubRecursionDepth(&srdc->choice.base, kP2SecurityRequestDataNamePlain));

	const char * const octet_string_mem = PcutIxPtrConst(&srdc->choice.base, ix, whole);
	//const int octet_string_len = PcutIxLen(&srdc->choice.base, ix, whole);

	const char * const content = PcutIxPtrConst(&os->base, kP2OctetStringCutIxContent, octet_string_mem);
	const int content_len = PcutIxLen(&os->base, kP2OctetStringCutIxContent, octet_string_mem);

	// printf_hex_ex("security request data plain content: ", "\n", content, content_len, "");
	// 再按client_apdu来解析+执行content。
	ifer(P2ProcessClientApdu(fill_repository_life, content, content_len));
	
	// fill仓库中得到最后一个fill指针，即最外层的
	const int num = PfillRepositoryNum(fill_repository_life);
	ifbr(0 < num);
	// apdu可以用名字"apdu"找，也可以默认最后一个是apdu。因为组帧，apdu一定是最后一个
	// 这样找有个好处，不用层层传递apdu指针。减轻传递变量的繁琐，也减少栈的占用，时间换空间
	const int last_ix = num - 1;		// 最后一个索引号
	dve(PfillRepositoryIxValid(fill_repository_life, last_ix));
	const PfillRepositoryItem *fr_item_last = PfillRepositoryIxItemConst(fill_repository_life, last_ix);
	Pfill *fill_client_apdu = fr_item_last->fill;
	ifbr(NULL != fill_client_apdu);

	// 按照SECURITY-Response应答
	Pfill *fill_octet_string = PfillRepositoryFactory(fill_repository_life, "plain_octet_string");
	ifbr(NULL != fill_octet_string);
	ifer(P2OctetStringFillInitByFill(fill_octet_string, false, fill_client_apdu));

	// data
	Pfill *fill_security_response_data = PfillRepositoryFactory(fill_repository_life, "security_response_data");
	ifbr(NULL != fill_security_response_data);
	ifer(P2SecurityResponseDataChoiceFillInit(fill_security_response_data, kP2SecurityResponseDataChoicePlain, kP2DarOtherReason, fill_octet_string));

	// mac
	Pfill *fill_security_response_verify_mac = PfillRepositoryFactory(fill_repository_life, "security_response_verify_mac");
	ifbr(NULL != fill_security_response_verify_mac);
	const char mac[] = "\x00\x00\x00\x00";
	const int mac_size = sizeof(mac) - 1;
	ifer(P2OctetStringFillInitByMem(fill_security_response_verify_mac, false, mac, mac_size));

	// verify
	Pfill *fill_security_response_verify = PfillRepositoryFactory(fill_repository_life, "security_response_verify");
	ifbr(NULL != fill_security_response_verify);
	ifer(P2SecurityResponseVerifyFillInit(fill_security_response_verify, fill_security_response_verify_mac));

	Pfill *fill_security_response = PfillRepositoryFactory(fill_repository_life, "security_response");
	ifbr(NULL != fill_security_response);
	ifer(P2SecurityResponseFillInit(fill_security_response, fill_security_response_data, fill_security_response_verify));

	Pfill *fill_security_apdu = PfillRepositoryFactory(fill_repository_life, kP2SecurityApduName);
	ifbr(NULL != fill_security_apdu);
	ifer(P2SecurityApduChoiceFillInit(fill_security_apdu, kP2SecurityApduChoiceResponse, fill_security_response));
	return 0;
}



typedef struct {
	Pdo doa;
} PdoSecurityRequestDataCipher;
static cp_t PdoSecurityRequestDataCipherProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	return cph;
}
#define kPdoSecurityRequestDataCipherDef { PDO_INIT(PdoSecurityRequestDataCipherProcess) }
//}}}


//{{{ security-apdu
typedef struct PdoSecurityApduRequestS {
	Pdo doa;
	Pcut *cut;
	int lud_size;	
	PfillRepository *fill_repository_life;
} PdoSecurityApduRequest;
static cp_t PdoProcessSecurityApduRequest(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	struct PdoSecurityApduRequestS *derive = (struct PdoSecurityApduRequestS *)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;
	dve(PfillRepositoryValid(fill_repository_life));

	// 可以确定，当前处在security-apdu中，choice为request
	P2SecurityApduChoicePcut *sac = (P2SecurityApduChoicePcut*)cut;
	P2ChoicePcut *choice = (P2ChoicePcut*)cut;
	Pcut *sac_base = (Pcut*)cut;
	dvb(choice == &sac->choice);
	dvb(sac_base == &sac->choice.base);
	// 当前一定是选择了security-request
	// 且P2SecurityRequestPcut已经被动态生成了
	dvb(kP2SecurityApduChoiceRequest == P2ChoicePcutChoice(choice));

	P2SecurityRequestPcut *sr = (P2SecurityRequestPcut*)P2ChoicePcutVar(choice);
	dvb(NULL != sr);
	dvb(NULL == (void*)PcutFindSubRecursionDepth(sac_base, kP2SecurityApduNameChoice)); 
	dvb(sr == (void*)PcutFindSubRecursionDepth(sac_base, kP2SecurityRequestName));
	dvb(&sr->data.choice.base == (void*)PcutFindSubRecursionDepth(sac_base, kP2SecurityRequestNameData));
	dvb(&sr->verify.choice.base == (void*)PcutFindSubRecursionDepth(sac_base, kP2SecurityRequestNameVerify));
	const char * const request_mem = PcutIxPtrConst(sac_base, ix, whole);
	

	dvb(cut == derive->cut);
	const int whole_len = PcutIxLen(cut, kPcutIxAll, whole);
	ifbr(whole_len == derive->lud_size);
	const int invalid_ix = PcutInvalidIx(cut, whole);
	ifbr(-1 == invalid_ix);
	printf_hex_ex("\nsecurity_apdu_request mem: ", "\n", whole, whole_len, "");
	PcutAllPrint(cut, 0, whole);
	
	PdoSecurityRequestDataPlain do_plain = { PDO_INIT(PdoSecurityRequestDataPlainProcess), fill_repository_life };
	PdoSecurityRequestDataCipher do_cipher = kPdoSecurityRequestDataCipherDef;
	Pdo* const kDataDoTable[kP2SecurityRequestDataChoiceNum] = {
		&do_plain.doa,
		&do_cipher.doa,
	};
	
	P2ChoiceVarDoTableSet(&sr->data.choice, kDataDoTable);
	const cp_t cp = PcutIxDo(&sr->base, 0, 0, kPcutIxAll, request_mem);
	return cp;
}


// security-response
typedef struct {
	Pdo doa;
} PdoSecurityApduResponse;
static cp_t PdoProcessSecurityApduResponse(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	return cph;
}
#define kPdoSecurityApduResponseDef { PDO_INIT(PdoProcessSecurityApduResponse) }
cp_t P2ProcessSecurityApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	// security_apud就是security_apud_choice，不像client_apdu在client_apdu_choice附加了optional_timetag，因此，security_apud=security_apud_choice。
	P2SecurityApduChoicePcut sac = kP2SecurityApduChoicePcutDef;
	ifer(P2SecurityApduChoicePcutOpen(&sac));

	PdoSecurityApduRequest do_request = { PDO_INIT(PdoProcessSecurityApduRequest)
		, &sac.choice.base, apdu_size, fill_repository_life };
	PdoSecurityApduResponse do_response = kPdoSecurityApduResponseDef;
	Pdo* const kDoTable[kP2SecurityApduChoiceNum] = {
		&do_request.doa,
		&do_response.doa,
	};
	P2ChoiceVarDoTableSet(&sac.choice, kDoTable);
	const cp_t cp = PcutIxDo(&sac.choice.base, 0, 0, kPcutIxAll, apdu);

	ifer(P2SecurityApduChoicePcutClose(&sac));
	return cp;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestNormal(void)
{
	return 0;
}


 
static const QtestItem kTestItem[] = {
	TestNormal,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ProcessSecurityApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ProcessSecurityApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

