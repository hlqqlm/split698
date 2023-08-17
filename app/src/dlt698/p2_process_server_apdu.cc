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

20230816-----------------------------------------------------------------------
huanglin 创建.
执行dlt698协议 Server-APDU
*/

#include "qos/qstr.h"

// dlt698_45
#include "p2_config.h"

#include "p2_base.h"
#include "p2_double_long_unsigned.h"
#include "p2_double_long.h"
#include "p2_long_unsigned.h"
#include "p2_octet_string.h"
#include "p2_sequence_of_var_len.h"
#include "p2_array.h"

#include "p2_get_response_normal.h"
#include "p2_get_response.h"

#include "p2_proxy_response_trans_command_response.h"
#include "p2_proxy_response_choice.h"

#include "p2_server_apdu_choice.h"
#include "p2_server_apdu.h"

#include "p2_process_server_apdu.h"
#include "p2_process_server_apdu.xcp.h"


#define TEST_EN				(0)
#define PRINT_FILL_EN		(0)			// 是否打印填充帧过程
#define CHECK_MEM_LEAK_EN	(0)			// 是否检查内存泄漏



//{{{ get_response_normal
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetResponseNormal;
static cp_t PdoGetResponseNormalProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoGetResponseNormal *derive = (PdoGetResponseNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在proxy_response_choice中, trans_command_response是当前的choice
	P2GetResponsePcut *gr = (P2GetResponsePcut*)part;
	P2GetResponseNormalPcut *grn = (P2GetResponseNormalPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grn == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetResponseNormalName));

	const char * const grn_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grn_mem_len = PcutIxLen(&gr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("get_response_normal mem: ", "\r\n", grn_mem, grn_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ get_response
typedef struct {
	Pdo doa;
} PdoGetResponseFail;
static cp_t PdoGetResponseProcessFail(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	const uint8_t get_response_choice = (uint8_t)(*whole);
	qos_printf("This get_response_choice is to be implemented. get_response_choice=%02xH\r\n", get_response_choice);
	return cph;
}
#define kPdoGetResponseFailDef { PDO_INIT(PdoGetResponseProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetResponse;
static cp_t PdoGetResponseProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoGetResponse *derive = (PdoGetResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ServerApduChoicePcut *sac = (P2ServerApduChoicePcut*)part;
	P2GetResponsePcut *gr = (P2GetResponsePcut*)P2ChoicePcutVar(&sac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&sac->choice.base, kP2GetResponseName));

	const char * const get_response_mem = PcutIxPtrConst(&sac->choice.base, ix, whole);
	const int get_response_mem_len = PcutIxLen(&sac->choice.base, ix, whole);

	//printf_hex_ex("get_response_mem: ", "\r\n", get_response_mem, get_response_mem_len, "");
	// 再按get_response来解析+执行get_response_mem.


	PdoGetResponseNormal do_get_response_normal = { 
		PDO_INIT(PdoGetResponseNormalProcess), derive->fill_repository_life };
	PdoGetResponseFail do_fail = kPdoGetResponseFailDef;
	Pdo* const kDoTable[kP2GetResponseChoiceNum] = {
		&do_get_response_normal.doa,	// 读取一个对象属性的响应 [1] GetResponseNormal，
		&do_fail.doa,	// 读取若干个对象属性的响应 [2] GetResponseNormalList，
		&do_fail.doa,	// 读取一个记录型对象属性的响应 [3] GetResponseRecord，
		&do_fail.doa,	// 读取若干个记录型对象属性的响应 [4] GetResponseRecordList，
		&do_fail.doa,	// 读取分帧传输的下一帧的响应 [5] GetResponseNext，
		&do_fail.doa,	// 读取一个对象属性的 MD5 值的响应 [6] GetResponseMD5，
		&do_fail.doa,	// 读取一个精简记录型对象属性请求 [23] GetResponseSimplifyRecord
	};
	P2ChoiceVarDoTableSet(&gr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&gr->choice.base, 0, 0, kPcutIxAll, get_response_mem);
	P2ChoiceVarDoTableSet(&gr->choice, NULL);
	return cp;
}
//}}}


//{{{ proxy_response_trans_command_response
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoProxyResponseTransCommandResponse;
static cp_t PdoProxyResponseTransCommandResponseProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoProxyResponseTransCommandResponse *derive = (PdoProxyResponseTransCommandResponse*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在proxy_response_choice中, trans_command_response是当前的choice
	P2ProxyResponseChoicePcut *pr = (P2ProxyResponseChoicePcut*)part;
	P2ProxyResponseTransCommandResponsePcut *prtcr = (P2ProxyResponseTransCommandResponsePcut*)P2ChoicePcutVar(&pr->choice);
	dvb(prtcr == (void*)PcutFindSubRecursionDepth(&pr->choice.base, kP2ProxyResponseTransCommandResponseName));

	const char * const prtcr_mem = PcutIxPtrConst(&pr->choice.base, ix, whole);
	const int prtcr_mem_len = PcutIxLen(&pr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("proxy_response.trans_command_response_mem: ", "\r\n", prtcr_mem, prtcr_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ proxy_response
typedef struct {
	Pdo doa;
} PdoProxyResponseFail;
static cp_t PdoProxyResponseProcessFail(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	const uint8_t proxy_response_choice = (uint8_t)(*whole);
	qos_printf("This proxy_response_choice is to be implemented. proxy_response_choice=%02xH\r\n", proxy_response_choice);
	return cph;
}
#define kPdoProxyResponseFailDef { PDO_INIT(PdoProxyResponseProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoProxyResponse;
static cp_t PdoProxyResponseProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoProxyResponse *derive = (PdoProxyResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ServerApduChoicePcut *sac = (P2ServerApduChoicePcut*)part;
	P2ProxyResponseChoicePcut *pr = (P2ProxyResponseChoicePcut*)P2ChoicePcutVar(&sac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&sac->choice.base, kP2ProxyResponseName));

	const char * const proxy_response_mem = PcutIxPtrConst(&sac->choice.base, ix, whole);
	const int proxy_response_mem_len = PcutIxLen(&sac->choice.base, ix, whole);

	//printf_hex_ex("get_response_mem: ", "\r\n", get_response_mem, get_response_mem_len, "");
	// 再按get_response来解析+执行get_response_mem.


	PdoProxyResponseTransCommandResponse do_trans_command_response = { 
		PDO_INIT(PdoProxyResponseTransCommandResponseProcess), derive->fill_repository_life };
	PdoProxyResponseFail do_fail = kPdoProxyResponseFailDef;
	Pdo* const kDoTable[kP2ProxyResponseChoiceNum] = {

		&do_fail.doa,	// 请求代理读取若干个服务器的若干个对象属性 [1] ProxyGetRequestList
		&do_fail.doa,	// 请求代理读取一个服务器的一个记录型对象属性 [2] ProxyGetRequestRecord
		&do_fail.doa,	// 请求代理设置若干个服务器的若干个对象属性 [3] ProxySetRequestList
		&do_fail.doa,	// 请求代理设置后读取若干个服务器的若干个对象属性 [4] ProxySetThenGetRequestList
		&do_fail.doa,	// 请求代理操作若干个服务器的若干个对象方法 [5] ProxyActionRequestList
		&do_fail.doa,	// 请求代理操作后读取若干个服务器的若干个对象方法和属性 [6] ProxyActionThenGetRequestList
		&do_trans_command_response.doa,	// 请求代理透明转发 命令 [7] ProxyTransCommandRequest
		&do_fail.doa,	// 请求代理终端内部转发命令 [8] ProxyInnerTransCommandRequest
	};
	P2ChoiceVarDoTableSet(&pr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&pr->choice.base, 0, 0, kPcutIxAll, proxy_response_mem);
	P2ChoiceVarDoTableSet(&pr->choice, NULL);
	return cp;
}
//}}}


//{{{ server-apdu
// do-fail
typedef struct {
	Pdo doa;
} PdoServerApduFail;
static cp_t PdoServerApduProcessFail(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	const uint8_t server_apdu_choice = (uint8_t)(*whole);
	qos_printf("Server apdu: this choice is to be implemented. server_apdu_choice=%02xH=%dD(%s)\r\n"
			, server_apdu_choice, server_apdu_choice, P2ServerApduChoiceStr(server_apdu_choice));
	return cph;
}
#define kPdoServerApduFailDef { PDO_INIT(PdoServerApduProcessFail) }
cp_t P2ProcessServerApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	P2ServerApduPcut sa = kP2ServerApduPcutDef;

	ifer(P2ServerApduPcutOpen(&sa));
	printf_hex_ex("\r\nserver_apdu mem: ", "\r\n", apdu, apdu_size, "");
	PcutAllPrint(&sa.base, 0, apdu);

	PdoGetResponse do_get_response = { PDO_INIT(PdoGetResponseProcess), fill_repository_life };
	PdoProxyResponse do_proxy_response = { PDO_INIT(PdoProxyResponseProcess), fill_repository_life };
	PdoServerApduFail do_fail = kPdoServerApduFailDef;
	Pdo* const kDoTable[kP2ServerApduChoiceNum] = {
		&do_fail.doa,	// kP2ServerApduChoiceConnectResponse = 130,	// 建立应用连接响应 [130] CONNECT-Response，
		&do_fail.doa,	// kP2ServerApduChoiceReleaseResponse = 131,	// 断开应用连接响应 [131] RELEASE-Response，
		&do_fail.doa,	// kP2ServerApduChoiceReleaseNotification = 132,	// 断开应用连接通知 [132] RELEASE-Notification，
		&do_get_response.doa,	// kP2ServerApduChoiceGetResponse = 133,	// 读取响应 [133] GET-Response，
		&do_fail.doa,	// kP2ServerApduChoiceSetResponse = 134,	// 设置响应 [134] SET-Response，
		&do_fail.doa,	// kP2ServerApduChoiceActionResponse = 135,	// 操作响应 [135] ACTION-Response，
		&do_fail.doa,	// kP2ServerApduChoiceReportNotification = 136,	// 上报通知 [136] REPORT-Notification，
		&do_proxy_response.doa,	// kP2ServerApduChoiceProxyResponse = 137,	// 代理响应 [137] PROXY-Response，
		&do_fail.doa,	// kP2ServerApduChoiceCompactGetResponse = 197,	// 紧凑读取响应 [197] COMPACT-GET-Response，
		&do_fail.doa,	// kP2ServerApduChoiceCompactSetResponse = 198,	// 紧凑设置响应 [198] COMPACT-SET-Response，
		&do_fail.doa,	// kP2ServerApduChoiceCompactProxyResponse = 201,	// 紧凑代理响应 [201] COMPACT-PROXY-Response，
		&do_fail.doa,	// kP2ServerApduChoiceErrorResponse = 238,	// 异常响应 [238] ERROR-Response
	};
	P2ChoiceVarDoTableSet(&sa.choice_part.choice, kDoTable);
	const cp_t cp = PcutIxDo(&sa.base, 0, 0, kPcutIxAll, apdu);
	P2ChoiceVarDoTableSet(&sa.choice_part.choice, NULL);

	ifer(P2ServerApduPcutClose(&sa));
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
cp_t P2ProcessServerApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ProcessServerApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

