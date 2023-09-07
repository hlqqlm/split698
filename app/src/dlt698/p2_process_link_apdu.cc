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

20230709-----------------------------------------------------------------------
huanglin 创建.
执行dlt698协议 link apdu
*/

#include "qos/qstr.h"

// dlt698_45
#include "p2_config.h"

#include "p2_enum.h"

#include "p2_link_request.h"
#include "p2_link_apdu_choice.h"
#include "p2_link_apdu.h"

#include "p2_process_link_apdu.h"
#include "p2_process_link_apdu.xcp.h"


#define TEST_EN				(0)
#define PRINT_FILL_EN		(0)			// 是否打印填充帧过程
#define CHECK_MEM_LEAK_EN	(0)			// 是否检查内存泄漏



//{{{ link_request_login
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoLinkRequestLogin;
static cp_t PdoLinkRequestLoginProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2EnumCutIxEnum);

	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoLinkRequestLogin *derive = (PdoLinkRequestLogin*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在link_request_enum中, link_request_login是当前的enum
	P2EnumPcut *type_enum = (P2EnumPcut*)cut;

	const char * const type_mem = PcutIxPtrConst(&type_enum->base, ix, whole);
	const int type_mem_len = PcutIxLen(&type_enum->base, ix, whole);
	dvb(kP2EnumSize == type_mem_len);

	//qos_printf("grn_mem_len=%d\n", grn_mem_len);
	//printf_hex(grn_mem, grn_mem_len, " ");
	//qos_printf("\n");
	if (kPrintPartEn)
		printf_hex_ex("type_mem_len: ", "\n", type_mem, type_mem_len, "");

	// 解帧，得到type
	const uint8_t type = *type_mem;
	ifbr(kP2LinkRequestTypeEnumLogin == type);

	if (kPrintPartEn)
		qos_printf("link_request_login.type_value=%02xH\n", type);

	
	//qos_printf("todo: answer link_request_login\n");
	return 0;
}
//}}}


//{{{ link_request_heartbeat
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoLinkRequestHeartbeat;
static cp_t PdoLinkRequestHeartbeatProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	qos_printf("todo: answer link_request.heartbeat\n");
	return 0;
}
//}}}


//{{{ link_request_logout
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoLinkRequestLogout;
static cp_t PdoLinkRequestLogoutProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	qos_printf("todo: answer link_request.logout\n");
	return 0;
}
//}}}


//{{{ link_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoLinkRequest;
static cp_t PdoLinkRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoiceCutIxVar);

	PdoLinkRequest *derive = (PdoLinkRequest*)doa;

	// 可以确定，当前处在link_apdu_choice中, link_request是当前的choice
	P2LinkApduChoicePcut *lac = (P2LinkApduChoicePcut*)cut;
	P2LinkRequestPcut *lr = (P2LinkRequestPcut*)P2ChoicePcutVar(&lac->choice);
	dvb(lr == (void*)PcutFindSubRecursionDepth(&lac->choice.base, kP2LinkRequestName));

	const char * const link_request_mem = PcutIxPtrConst(&lac->choice.base, ix, whole);
	// const int link_request_mem_len = PcutIxLen(&lac->choice.base, ix, whole);

	// ../protocol/pcut.h
	//const char * const type_mem = P2LinkRequestTypeMem(link_request_mem);
	const char * const type_mem = PcutIxPtrConst(&lr->base, kP2LinkRequestCutIxType, link_request_mem);

	//printf_hex_ex("get_request_mem: ", "\n", get_request_mem, get_request_mem_len, "");
	// 再按link_request来解析+执行link_request_mem.


	PdoLinkRequestLogin do_link_request_login = { 
		PDO_INIT(PdoLinkRequestLoginProcess), derive->fill_repository_life };
	PdoLinkRequestHeartbeat do_link_request_heartbeat = { 
		PDO_INIT(PdoLinkRequestHeartbeatProcess), derive->fill_repository_life };
	PdoLinkRequestLogout do_link_request_logout = { 
		PDO_INIT(PdoLinkRequestLogoutProcess), derive->fill_repository_life };
	Pdo* const kDoTable[kP2LinkRequestTypeEnumNum] = {
		&do_link_request_login.doa,		// kP2LinkRequestTypeEnumLogin = 0,		// 登录 （0），
		&do_link_request_heartbeat.doa,	// kP2LinkRequestTypeEnumHeartbeat = 1,	// 心跳 （1），
		&do_link_request_logout.doa,	// kP2LinkRequestTypeEnumHeartLogout = 2,	// 退出登录 （2）
	};
	P2EnumDoTableSet(&lr->enum_type, kDoTable);
	const cp_t cp = PcutIxDo(&lr->enum_type.base, 0, 0, kPcutIxAll, type_mem);
	P2EnumDoTableSet(&lr->enum_type, NULL);
	return cp;
}
//}}}


//{{{ link_response
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoLinkResponse;
static cp_t PdoLinkResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	//qos_printf("todo: link_apdu.response process.");
	return 0;
}
//}}}


//{{{ link_apdu
cp_t P2ProcessLinkApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	P2LinkApduPcut la = kP2LinkApduPcutDef;

	ifer(P2LinkApduPcutOpen(&la));
	ifer(PcutIxValid(&la.base, kPcutIxAll, apdu));
	ifbr(apdu_size == PcutIxLen(&la.base, kPcutIxAll, apdu));

	printf_hex_ex("\nlink_apdu mem: ", "\n", apdu, apdu_size, "");
	PcutAllPrint(&la.base, 0, apdu);


	PdoLinkRequest do_link_request = { PDO_INIT(PdoLinkRequestProcess), fill_repository_life };
	PdoLinkResponse do_link_response = { PDO_INIT(PdoLinkResponseProcess), fill_repository_life };
	Pdo* const kDoTable[kP2LinkApduChoiceNum] = {
		&do_link_request.doa,	// kP2LinkApduChoiceRequest = 1,	// 预连接请求 [1] LINK-Request，
		&do_link_response.doa,			// kP2LinkApduChoiceResponse = 129,	// 预连接响应 [129] LINK-Response
	};
	P2ChoiceVarDoTableSet(&la.choice_cut.choice, kDoTable);
	const cp_t cp = PcutIxDo(&la.base, 0, 0, kPcutIxAll, apdu);
	P2ChoiceVarDoTableSet(&la.choice_cut.choice, NULL);

	ifer(P2LinkApduPcutClose(&la));
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
cp_t P2ProcessLinkApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ProcessLinkApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

