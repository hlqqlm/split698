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
#include "p2_get_response_normal_list.h"
#include "p2_get_response.h"

#include "p2_set_response_normal.h"
#include "p2_set_response.h"

#include "p2_action_response_normal.h"
#include "p2_action_response_normal_list.h"
#include "p2_action_then_get_response_normal_list.h"
#include "p2_action_response.h"

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
static cp_t PdoGetResponseNormalProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoGetResponseNormal *derive = (PdoGetResponseNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在proxy_response_choice中, trans_command_response是当前的choice
	P2GetResponsePcut *gr = (P2GetResponsePcut*)cut;
	P2GetResponseNormalPcut *srn = (P2GetResponseNormalPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grn == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetResponseNormalName));

	const char * const grn_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grn_mem_len = PcutIxLen(&gr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("get_response_normal mem: ", "\n", grn_mem, grn_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ get_response_normal_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetResponseNormalList;
static cp_t PdoGetResponseNormalListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoGetResponseNormalList *derive = (PdoGetResponseNormalList*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在proxy_response_choice中, trans_command_response是当前的choice
	P2GetResponsePcut *gr = (P2GetResponsePcut*)cut;
	P2GetResponseNormalListPcut *grnl = (P2GetResponseNormalListPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grnl == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetResponseNormalListName));

	const char * const grnl_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grnl_mem_len = PcutIxLen(&gr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("get_response_normal_list mem: ", "\n", grnl_mem, grnl_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ get_response
typedef struct {
	Pdo doa;
} PdoGetResponseFail;
static cp_t PdoGetResponseProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t get_response_choice = (uint8_t)(*whole);
	qos_printf("This get_response_choice is to be implemented. get_response_choice=%02xH\n", get_response_choice);
	return cph;
}
#define kPdoGetResponseFailDef { PDO_INIT(PdoGetResponseProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetResponse;
static cp_t PdoGetResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoGetResponse *derive = (PdoGetResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ServerApduChoicePcut *sac = (P2ServerApduChoicePcut*)cut;
	P2GetResponsePcut *gr = (P2GetResponsePcut*)P2ChoicePcutVar(&sac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&sac->choice.base, kP2GetResponseName));

	const char * const get_response_mem = PcutIxPtrConst(&sac->choice.base, ix, whole);
	const int get_response_mem_len = PcutIxLen(&sac->choice.base, ix, whole);

	//printf_hex_ex("get_response_mem: ", "\n", get_response_mem, get_response_mem_len, "");
	// 再按get_response来解析+执行get_response_mem.


	PdoGetResponseNormal do_get_response_normal = { 
		PDO_INIT(PdoGetResponseNormalProcess), derive->fill_repository_life };
	PdoGetResponseNormalList do_get_response_normal_list = { 
		PDO_INIT(PdoGetResponseNormalListProcess), derive->fill_repository_life };
	PdoGetResponseFail do_fail = kPdoGetResponseFailDef;
	Pdo* const kDoTable[kP2GetResponseChoiceNum] = {
		&do_get_response_normal.doa,	// 读取一个对象属性的响应 [1] GetResponseNormal，
		&do_get_response_normal_list.doa,	// 读取若干个对象属性的响应 [2] GetResponseNormalList，
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


//{{{ set_response_normal
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoSetResponseNormal;
static cp_t PdoSetResponseNormalProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoSetResponseNormal *derive = (PdoSetResponseNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在set_response_choice中, set_response_normal是当前的choice
	P2SetResponsePcut *sr = (P2SetResponsePcut*)cut;
	P2SetResponseNormalPcut *srn = (P2SetResponseNormalPcut*)P2ChoicePcutVar(&sr->choice);
	dvb(srn == (void*)PcutFindSubRecursionDepth(&sr->choice.base, kP2SetResponseNormalName));

	const char * const srn_mem = PcutIxPtrConst(&sr->choice.base, ix, whole);
	const int srn_mem_len = PcutIxLen(&sr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("set_response_normal mem: ", "\n", srn_mem, srn_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ set_response
typedef struct {
	Pdo doa;
} PdoSetResponseFail;
static cp_t PdoSetResponseProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t set_response_choice = (uint8_t)(*whole);
	qos_printf("This set_response_choice is to be implemented. set_response_choice=%02xH\n", set_response_choice);
	return cph;
}
#define kPdoSetResponseFailDef { PDO_INIT(PdoSetResponseProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoSetResponse;
static cp_t PdoSetResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoSetResponse *derive = (PdoSetResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ServerApduChoicePcut *sac = (P2ServerApduChoicePcut*)cut;
	P2SetResponsePcut *sr = (P2SetResponsePcut*)P2ChoicePcutVar(&sac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&sac->choice.base, kP2SetResponseName));

	const char * const set_response_mem = PcutIxPtrConst(&sac->choice.base, ix, whole);
	const int set_response_mem_len = PcutIxLen(&sac->choice.base, ix, whole);

	//printf_hex_ex("get_response_mem: ", "\n", get_response_mem, get_response_mem_len, "");
	// 再按get_response来解析+执行get_response_mem.


	PdoSetResponseNormal do_set_response_normal = { 
		PDO_INIT(PdoSetResponseNormalProcess), derive->fill_repository_life };
	PdoSetResponseFail do_fail = kPdoSetResponseFailDef;
	Pdo* const kDoTable[kP2SetResponseChoiceNum] = {
		&do_set_response_normal.doa,	// 设置一个对象属性的响应 [1] SetResponseNormal，
		&do_fail.doa,	// 设置若干个对象属性的响应 [2] SetResponseNormalList，
		&do_fail.doa,	// 设置的确认信息以及读取的响应 [3] SetThenGetResponseNormalList
	};
	P2ChoiceVarDoTableSet(&sr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&sr->choice.base, 0, 0, kPcutIxAll, set_response_mem);
	P2ChoiceVarDoTableSet(&sr->choice, NULL);
	return cp;
}
//}}}


//{{{ action_response_normal
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionResponseNormal;
static cp_t PdoActionResponseNormalProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoActionResponseNormal *derive = (PdoActionResponseNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在set_response_choice中, set_response_normal是当前的choice
	P2ActionResponsePcut *ar = (P2ActionResponsePcut*)cut;
	P2ActionResponseNormalPcut *arn = (P2ActionResponseNormalPcut*)P2ChoicePcutVar(&ar->choice);
	dvb(arn == (void*)PcutFindSubRecursionDepth(&ar->choice.base, kP2ActionResponseNormalName));

	const char * const arn_mem = PcutIxPtrConst(&ar->choice.base, ix, whole);
	const int arn_mem_len = PcutIxLen(&ar->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("action_response_normal mem: ", "\n", arn_mem, arn_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ action_response_normal_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionResponseNormalList;
static cp_t PdoActionResponseNormalListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoActionResponseNormalList *derive = (PdoActionResponseNormalList*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在set_response_choice中, set_response_normal是当前的choice
	P2ActionResponsePcut *ar = (P2ActionResponsePcut*)cut;
	P2ActionResponseNormalListPcut *arnl = (P2ActionResponseNormalListPcut*)P2ChoicePcutVar(&ar->choice);
	dvb(arnl == (void*)PcutFindSubRecursionDepth(&ar->choice.base, kP2ActionResponseNormalListName));

	const char * const arnl_mem = PcutIxPtrConst(&ar->choice.base, ix, whole);
	const int arnl_mem_len = PcutIxLen(&ar->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("action_response_normal_list mem: ", "\n", arnl_mem, arnl_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ action_then_get_response_normal_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionThenGetResponseNormalList;
static cp_t PdoActionThenGetResponseNormalListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoActionThenGetResponseNormalList *derive = (PdoActionThenGetResponseNormalList*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在set_response_choice中, action_then_get_response_normal_list是当前的choice
	P2ActionResponsePcut *ar = (P2ActionResponsePcut*)cut;
	P2ActionThenGetResponseNormalListPcut *atgrnl = (P2ActionThenGetResponseNormalListPcut*)P2ChoicePcutVar(&ar->choice);
	dvb(atgrnl == (void*)PcutFindSubRecursionDepth(&ar->choice.base, kP2ActionThenGetResponseNormalListName));

	const char * const atgrnl_mem = PcutIxPtrConst(&ar->choice.base, ix, whole);
	const int atgrnl_mem_len = PcutIxLen(&ar->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("action_then_get_response_normal_list mem: ", "\n", atgrnl_mem, atgrnl_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ action_response
typedef struct {
	Pdo doa;
} PdoActionResponseFail;
static cp_t PdoActionResponseProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t action_response_choice = (uint8_t)(*whole);
	qos_printf("This action_response_choice is to be implemented. action_response_choice=%02xH\n", action_response_choice);
	return cph;
}
#define kPdoActionResponseFailDef { PDO_INIT(PdoActionResponseProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionResponse;
static cp_t PdoActionResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoActionResponse *derive = (PdoActionResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ServerApduChoicePcut *sac = (P2ServerApduChoicePcut*)cut;
	P2ActionResponsePcut *ar = (P2ActionResponsePcut*)P2ChoicePcutVar(&sac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&sac->choice.base, kP2ActionResponseName));

	const char * const action_response_mem = PcutIxPtrConst(&sac->choice.base, ix, whole);
	const int action_response_mem_len = PcutIxLen(&sac->choice.base, ix, whole);

	//printf_hex_ex("get_response_mem: ", "\n", get_response_mem, get_response_mem_len, "");
	// 再按get_response来解析+执行get_response_mem.


	PdoActionResponseNormal do_action_response_normal = { 
		PDO_INIT(PdoActionResponseNormalProcess), derive->fill_repository_life };
	PdoActionResponseNormalList do_action_response_normal_list = { 
		PDO_INIT(PdoActionResponseNormalListProcess), derive->fill_repository_life };
	PdoActionThenGetResponseNormalList do_action_then_get_response_normal_list = { 
		PDO_INIT(PdoActionThenGetResponseNormalListProcess), derive->fill_repository_life };
	// PdoActionResponseFail do_fail = kPdoActionResponseFailDef;
	Pdo* const kDoTable[kP2ActionResponseChoiceNum] = {
		&do_action_response_normal.doa,	// 操作一个对象方法的响应 [1] ActionResponseNormal，
		&do_action_response_normal_list.doa,	// 操作若干个对象方法的响应 [2] ActionResponseNormalList，
		&do_action_then_get_response_normal_list.doa,	// 操作若干个对象方法后读取若干个属性的响应 [3] ActionThenGetResponseNormalList
	};
	P2ChoiceVarDoTableSet(&ar->choice, kDoTable);
	const cp_t cp = PcutIxDo(&ar->choice.base, 0, 0, kPcutIxAll, action_response_mem);
	P2ChoiceVarDoTableSet(&ar->choice, NULL);
	return cp;
}
//}}}


//{{{ proxy_response_trans_command_response
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoProxyResponseTransCommandResponse;
static cp_t PdoProxyResponseTransCommandResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoProxyResponseTransCommandResponse *derive = (PdoProxyResponseTransCommandResponse*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在proxy_response_choice中, trans_command_response是当前的choice
	P2ProxyResponseChoicePcut *pr = (P2ProxyResponseChoicePcut*)cut;
	P2ProxyResponseTransCommandResponsePcut *prtcr = (P2ProxyResponseTransCommandResponsePcut*)P2ChoicePcutVar(&pr->choice);
	dvb(prtcr == (void*)PcutFindSubRecursionDepth(&pr->choice.base, kP2ProxyResponseTransCommandResponseName));

	const char * const prtcr_mem = PcutIxPtrConst(&pr->choice.base, ix, whole);
	const int prtcr_mem_len = PcutIxLen(&pr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("proxy_response.trans_command_response_mem: ", "\n", prtcr_mem, prtcr_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ proxy_response
typedef struct {
	Pdo doa;
} PdoProxyResponseFail;
static cp_t PdoProxyResponseProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t proxy_response_choice = (uint8_t)(*whole);
	qos_printf("This proxy_response_choice is to be implemented. proxy_response_choice=%02xH\n", proxy_response_choice);
	return cph;
}
#define kPdoProxyResponseFailDef { PDO_INIT(PdoProxyResponseProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoProxyResponse;
static cp_t PdoProxyResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoProxyResponse *derive = (PdoProxyResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ServerApduChoicePcut *sac = (P2ServerApduChoicePcut*)cut;
	P2ProxyResponseChoicePcut *pr = (P2ProxyResponseChoicePcut*)P2ChoicePcutVar(&sac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&sac->choice.base, kP2ProxyResponseName));

	const char * const proxy_response_mem = PcutIxPtrConst(&sac->choice.base, ix, whole);
	const int proxy_response_mem_len = PcutIxLen(&sac->choice.base, ix, whole);

	//printf_hex_ex("get_response_mem: ", "\n", get_response_mem, get_response_mem_len, "");
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
static cp_t PdoServerApduProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t server_apdu_choice = (uint8_t)(*whole);
	qos_printf("Server apdu: this choice is to be implemented. server_apdu_choice=%02xH=%dD(%s)\n"
			, server_apdu_choice, server_apdu_choice, P2ServerApduChoiceStr(server_apdu_choice));
	return cph;
}
#define kPdoServerApduFailDef { PDO_INIT(PdoServerApduProcessFail) }
cp_t P2ProcessServerApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	P2ServerApduPcut sa = kP2ServerApduPcutDef;

	ifer(P2ServerApduPcutOpen(&sa));
	printf_hex_ex("\nserver_apdu mem: ", "\n", apdu, apdu_size, "");
	PcutAllPrint(&sa.base, 0, apdu);

	PdoGetResponse do_get_response = { PDO_INIT(PdoGetResponseProcess), fill_repository_life };
	PdoSetResponse do_set_response = { PDO_INIT(PdoSetResponseProcess), fill_repository_life };
	PdoActionResponse do_action_response = { PDO_INIT(PdoActionResponseProcess), fill_repository_life };
	PdoProxyResponse do_proxy_response = { PDO_INIT(PdoProxyResponseProcess), fill_repository_life };
	PdoServerApduFail do_fail = kPdoServerApduFailDef;
	Pdo* const kDoTable[kP2ServerApduChoiceNum] = {
		&do_fail.doa,	// kP2ServerApduChoiceConnectResponse = 130,	// 建立应用连接响应 [130] CONNECT-Response，
		&do_fail.doa,	// kP2ServerApduChoiceReleaseResponse = 131,	// 断开应用连接响应 [131] RELEASE-Response，
		&do_fail.doa,	// kP2ServerApduChoiceReleaseNotification = 132,	// 断开应用连接通知 [132] RELEASE-Notification，
		&do_get_response.doa,	// kP2ServerApduChoiceGetResponse = 133,	// 读取响应 [133] GET-Response，
		&do_set_response.doa,	// kP2ServerApduChoiceSetResponse = 134,	// 设置响应 [134] SET-Response，
		&do_action_response.doa,	// kP2ServerApduChoiceActionResponse = 135,	// 操作响应 [135] ACTION-Response，
		&do_fail.doa,	// kP2ServerApduChoiceReportNotification = 136,	// 上报通知 [136] REPORT-Notification，
		&do_proxy_response.doa,	// kP2ServerApduChoiceProxyResponse = 137,	// 代理响应 [137] PROXY-Response，
		&do_fail.doa,	// kP2ServerApduChoiceCompactGetResponse = 197,	// 紧凑读取响应 [197] COMPACT-GET-Response，
		&do_fail.doa,	// kP2ServerApduChoiceCompactSetResponse = 198,	// 紧凑设置响应 [198] COMPACT-SET-Response，
		&do_fail.doa,	// kP2ServerApduChoiceCompactProxyResponse = 201,	// 紧凑代理响应 [201] COMPACT-PROXY-Response，
		&do_fail.doa,	// kP2ServerApduChoiceErrorResponse = 238,	// 异常响应 [238] ERROR-Response
	};
	P2ChoiceVarDoTableSet(&sa.choice_cut.choice, kDoTable);
	const cp_t cp = PcutIxDo(&sa.base, 0, 0, kPcutIxAll, apdu);
	P2ChoiceVarDoTableSet(&sa.choice_cut.choice, NULL);

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

