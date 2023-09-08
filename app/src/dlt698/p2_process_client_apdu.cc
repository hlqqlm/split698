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
执行dlt698协议
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

#include "p2_connect_request.h"

#include "p2_get_request_normal.h"
#include "p2_get_request_normal_list.h"
#include "p2_get_request.h"

#include "p2_set_request_normal.h"
#include "p2_set_request_choice.h"

#include "p2_action_request_normal.h"
#include "p2_action_request_normal_list.h"
#include "p2_action_then_get_request_normal_list.h"
#include "p2_action_request_choice.h"

#include "p2_report_response_record_list.h"
#include "p2_report_response_choice.h"

#include "p2_proxy_request_trans_command_request.h"
#include "p2_proxy_request_choice.h"

#include "p2_client_apdu_choice.h"
#include "p2_client_apdu.h"

#include "p2_get_result.h"
#include "p2_a_result_normal.h"
#include "p2_get_response.h"
#include "p2_get_response_normal.h"
#include "p2_get_response_normal_list.h"
#include "p2_server_apdu_choice.h"
#include "p2_server_apdu.h"
/*
#include "p2_get_response.h"
#include "p2_get_response_normal.h"
*/

#include "p2_process_client_apdu.h"
#include "p2_process_client_apdu.xcp.h"
//#define this_file_id	0x  // echo -n p2_process.c | rhash --simple -


#define TEST_EN				(0)
#define PRINT_FILL_EN		(0)			// 是否打印填充帧过程
#define CHECK_MEM_LEAK_EN	(0)			// 是否检查内存泄漏


//{{{ tx_info
typedef struct
{
	char *tx_buf; 
	int tx_buf_size; 
	int tx_size;
} TxInfo;
#define kTxInfo(_tx_buf, _tx_buf_size, _tx_size) {_tx_buf, _tx_buf_size, _tx_size}

// 为了简化tx信息传递，这里用了全局变量
static TxInfo m_tx_info = kTxInfo(NULL, 0, 0);
//}}}


//{{{ oad
static cp_t FillOadOctetString(Pfill *fill_get_result, Pfill *fill_data, const char *mem, int size)
{
	ifer(P2OctetStringFillInitByMem(fill_data, true, mem, size));
	ifer(P2GetResultFillInit(fill_get_result, kP2DarSuccess, fill_data));
	return 0;
}
// |-OAD	40 01 02 00	* OI=0x4001:通信地址 属性2:参见文档 特征0 索引0 	
static cp_t ResponseOad40010200(Pfill *fill_get_result, Pfill *fill_data)
{
	char addr_mem[6] = { 0 };
	const int addr_size = sizeof(addr_mem);
	//memcpy(addr_mem, AddrParameter.MrCommAddress, addr_size);
	memcpy(addr_mem, kAddrParameterMrCommAddress, addr_size);

	//ifer(P2OctetStringFillInit(fill_data, true, (const char *)AddrParameter.MrCommAddress, addr_size));
	ifer(P2OctetStringFillInitByMem(fill_data, true, addr_mem, addr_size));
	ifer(P2GetResultFillInit(fill_get_result, kP2DarSuccess, fill_data));
	// 如果出现错误，则不填写fill_data, Dar填写对应的错误值
	// ifer(P2GetResultFillInit(fill_get_result, kP2DarXxx, NULL));
	
	return 0;
}

static cp_t ResponseOad00100200OneEnergy(PfillRepository *fill_repository_life, Pfill *fill_array, double energy)
{
	const uint32_t value = energy * 100;

	Pfill *fill_double_long_unsigned = PfillRepositoryFactory(fill_repository_life, "double_long_unsigned");
	ifbr(NULL != fill_double_long_unsigned);
	ifer(P2DoubleLongUnsignedFillInit(fill_double_long_unsigned, true, value));
	ifer(P2ArrayFillInitThenAdd(fill_array, fill_double_long_unsigned));
	return 0;
}
static cp_t ResponseOad00100200(PfillRepository *fill_repository_life, Pfill *fill_get_result, Pfill *fill_data)
{
	Pfill *fill_array = fill_data;
	ifbr(NULL != fill_array);


	double energy[PRICE_NUM] = { 123.45 };
	// 只有一个费率
	for (int i = 0; i < PRICE_NUM; ++i)
		ifer(ResponseOad00100200OneEnergy(fill_repository_life, fill_array, energy[i]));

	ifer(P2GetResultFillInit(fill_get_result, kP2DarSuccess, fill_data));
	return 0;
}



static cp_t ResponseOad20000200OneVoltage(PfillRepository *fill_repository_life, Pfill *fill_array, double voltage)
{
	const uint16_t value = voltage * 10;

	Pfill *fill_long_unsigned = PfillRepositoryFactory(fill_repository_life, "long_unsigned");
	ifbr(NULL != fill_long_unsigned);
	ifer(P2LongUnsignedFillInit(fill_long_unsigned, true, value));
	ifer(P2ArrayFillInitThenAdd(fill_array, fill_long_unsigned));
	return 0;
}
static cp_t ResponseOad20000200(PfillRepository *fill_repository_life, Pfill *fill_get_result, Pfill *fill_data)
{
	Pfill *fill_array = fill_data;
	ifbr(NULL != fill_array);

	// ABC三相
	double voltage[3] = { 1.2, 2.2, 3.2 };
	for (int i = 0; i < 3; ++i)
	{
		//ifer(ResponseOad20000200OneVoltage(fill_repository_life, fill_array, MrData.Volt[i]));
		ifer(ResponseOad20000200OneVoltage(fill_repository_life, fill_array, voltage[i]));
	}

	ifer(P2GetResultFillInit(fill_get_result, kP2DarSuccess, fill_data));
	return 0;
}



static cp_t ResponseOad20040200OnePower(PfillRepository *fill_repository_life, Pfill *fill_array, float power)
{
	const int32_t value = power * 10000;
	Pfill *fill_double_long = PfillRepositoryFactory(fill_repository_life, "double_long");
	ifbr(NULL != fill_double_long);
	ifer(P2DoubleLongFillInit(fill_double_long, true, value));
	ifer(P2ArrayFillInitThenAdd(fill_array, fill_double_long));
	return 0;
}
static cp_t ResponseOad20040200(PfillRepository *fill_repository_life, Pfill *fill_get_result, Pfill *fill_data)
{
	Pfill *fill_array = fill_data;
	ifbr(NULL != fill_array);

	// ABC总
	float power[4] = { 1.1, 2.2, 3.3, 4.4 };

	// 总ABC
	ifer(ResponseOad20040200OnePower(fill_repository_life, fill_array, power[3]));
	ifer(ResponseOad20040200OnePower(fill_repository_life, fill_array, power[0]));
	ifer(ResponseOad20040200OnePower(fill_repository_life, fill_array, power[1]));
	ifer(ResponseOad20040200OnePower(fill_repository_life, fill_array, power[2]));

	ifer(P2GetResultFillInit(fill_get_result, kP2DarSuccess, fill_data));
	return 0;
}



static cp_t ResponseOad(PfillRepository *fill_repository_life
		, Pfill *fill_get_result, Pfill *fill_data, OadT oad)
{
    return 0;

	// qos_printf("oad=%08x\n", oad);
	switch (oad)
	{
		case 0x40010200:  // |-OAD	40 01 02 00	* OI=0x4001:通信地址 属性2:参见文档 特征0 索引0 	
			return ResponseOad40010200(fill_get_result, fill_data);
		case 0x00100200:	// 正向有功电能
			return ResponseOad00100200(fill_repository_life, fill_get_result, fill_data);
		case 0x20000200:	// 电压
			return ResponseOad20000200(fill_repository_life, fill_get_result, fill_data);
		case 0x20040200:	// 有功功率
			return ResponseOad20040200(fill_repository_life, fill_get_result, fill_data);
			//return FillOadOctetString(fill_get_result, fill_data, "456", 3);
		default:
			qos_printf("I can NOT response this oad in ResponseOad(). oad=%08xH\n", oad);
			break;
	}

	return cph;
}
// 应答这个oad，填充到fill_a_result_normal中
// 中间生成的Pfill对象，托管到fill_repository(fill仓库，负责Pfill的销毁)
// fill_repository_a_result_normal		专门记录a_result_normal类型的fill
// fill_repository_life					存放所有fill，负责其生命周期，即负责close + qfree
static cp_t FillAResultNormalOad(
		PfillRepository *fill_repository_a_result_normal
		, PfillRepository *fill_repository_life
		, OadT oad)
{
	// 逆序释放内存
	Pfill *fill_data = PfillRepositoryFactory(fill_repository_life, "data");
	ifbr(NULL != fill_data);
	Pfill *fill_get_result = PfillRepositoryFactory(fill_repository_life, "get_result");
	ifbr(NULL != fill_get_result);
	Pfill *fill_a_result_normal = PfillRepositoryFactory(fill_repository_life, "a_result_normal");
	ifbr(NULL != fill_a_result_normal);
	// 登记所有fill_a_result_normal类型。注意：fill_repository_a_result_normal不负责close+qfree，只登记
	ifer(PfillRepositoryRegister(fill_repository_a_result_normal, "a_result_normal", fill_a_result_normal, PfillCloseEmpty, false));

	ifer(ResponseOad(fill_repository_life, fill_get_result, fill_data, oad));
	ifer(P2AResultNormalFillInit(fill_a_result_normal, oad, fill_get_result));
	return 0;
}

//}}}


//{{{ get_request_normal
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetRequestNormal;
static cp_t PdoGetRequestNormalProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoGetRequestNormal *derive = (PdoGetRequestNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在get_request_choice中, get_request_normal是当前的choice
	P2GetRequestPcut *gr = (P2GetRequestPcut*)cut;
	P2GetRequestNormalPcut *grn = (P2GetRequestNormalPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grn == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetRequestNormalName));

	const char * const grn_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grn_mem_len = PcutIxLen(&gr->choice.base, 0, ix, whole);

	//qos_printf("grn_mem_len=%d\n", grn_mem_len);
	//printf_hex(grn_mem, grn_mem_len, " ");
	//qos_printf("\n");
	if (kPrintPartEn)
		printf_hex_ex("grn_mem_len: ", "\n", grn_mem, grn_mem_len, "");

	// 解帧，得到piid + oad
	P2GetRequestNormalValue grn_value = kP2GetRequestNormalValueDef;
	ifer(P2GetRequestNormalPcutValue(&grn_value, grn_mem));
	//P2Piid piid = kP2PiidDef;
	//P2PiidPart(&piid, grn_value.piid);

	if (kPrintPartEn)
		qos_printf("get_request_normal_value.[oad=%08xH piid=%02xH]\n", grn_value.oad, grn_value.piid);

	// 《698面向对象的用电信息数据交换协议（20210910）_有标签.pdf》附录H3有oad40010200的应答报文
	// 主站问地址，报文示例：
	// => 68 17 00 43 45 AA AA AA AA AA AA 00 5B 4F 05 01 25 40 01 02 00 00 0A 92 16
	// <= 68 21 00 C3 05 33 09 20 39 00 00 00 4A 55 85 01 25 40 01 02 00 01 09 06 00 00 39 20 09 33 00 00 B2 D8 16


	//Pfill fill_data = kPfillDef;
	//ifer(PfillOpen(&fill_data));
	Pfill *fill_data = PfillRepositoryFactory(fill_repository_life, "data");
	ifbr(NULL != fill_data);

	//Pfill fill_get_result = kPfillDef;
	//ifer(PfillOpen(&fill_get_result));
	Pfill *fill_get_result = PfillRepositoryFactory(fill_repository_life, "get_result");
	ifbr(NULL != fill_get_result);
	const cp_t response_cp = ResponseOad(fill_repository_life, fill_get_result, fill_data, grn_value.oad);
	// 如果应答oad错误，则关闭前面的Pfill，避免内存泄漏
	if (0 != response_cp)
	{
		//ifer(PfillClose(&fill_data));
		//ifer(PfillClose(&fill_get_result));
		return response_cp;
	}

	//Pfill fill_a_result_normal = kPfillDef;
	//ifer(PfillOpen(&fill_a_result_normal));
	Pfill *fill_a_result_normal = PfillRepositoryFactory(fill_repository_life, "a_result_normal");
	ifbr(NULL != fill_a_result_normal);
	ifer(P2AResultNormalFillInit(fill_a_result_normal, grn_value.oad, fill_get_result));

	//Pfill fill_get_response_normal = kPfillDef;
	//ifer(PfillOpen(&fill_get_response_normal));
	Pfill *fill_get_response_normal = PfillRepositoryFactory(fill_repository_life, "get_response_normal");
	ifbr(NULL != fill_get_response_normal);

	P2Piid piid_split = kP2PiidDef;
	P2PiidPart(&piid_split, grn_value.piid);
	const P2PiidAcd piid_acd_value = {piid_split.service_priority, 0, piid_split.service_code};
	ifer(P2GetResponseNormalFillInit(fill_get_response_normal, &piid_acd_value, fill_a_result_normal));

	//Pfill fill_get_response = kPfillDef;
	//ifer(PfillOpen(&fill_get_response));
	Pfill *fill_get_response = PfillRepositoryFactory(fill_repository_life, "get_response");
	ifbr(NULL != fill_get_response);
	ifer(P2GetResponseFillInit(fill_get_response, kP2GetResponseChoiceGetResponseNormal, fill_get_response_normal));

	// fill_server_apdu就是Lud/Apdu
	//Pfill fill_server_apdu = kPfillDef;
	//ifer(PfillOpen(&fill_server_apdu));
	Pfill *fill_apdu = PfillRepositoryFactory(fill_repository_life, kP2NameApdu);
	ifbr(NULL != fill_apdu);
	ifer(P2ServerApduFillInit(fill_apdu, kP2ServerApduChoiceGetResponse, fill_get_response, NULL, NULL));

#if 0
	P2Pfill fill_frame = kP2PfillDef(&fill_server_apdu);
	const P2Control control = { kP2DirPrmServerResponse, 0, 0, kP2FunCodeUserData };
	const int addr_size = 6;
	const P2AddrValue addr = { kP2AddrSaTypeSingle
		, {
			AddrParameter.MrCommAddress[0],
			AddrParameter.MrCommAddress[1],  
			AddrParameter.MrCommAddress[2],  
			AddrParameter.MrCommAddress[3],  
			AddrParameter.MrCommAddress[4],  
			AddrParameter.MrCommAddress[5]
		}
		, addr_size, 0, 0 };
	ifer(P2PfillOpen(&fill_frame, &control, &addr));
	ifer(P2PfillValid(&fill_frame));

	if (PRINT_FILL_EN)
		PfillSetPrintFillWithSubs(&fill_frame.base, PfillPrintFillNormal);

	dvb(NULL != m_tx_info.tx_buf);
	dvb(0 < m_tx_info.tx_buf_size);
	ifer(PfillDo(&fill_frame.base, 0, m_tx_info.tx_buf, m_tx_info.tx_buf_size));
	const int fill_size = PfillIxLen(&fill_frame.base, kPfillIxAll);
	ifbr(fill_size <= m_tx_info.tx_buf_size);
	m_tx_info.tx_size = fill_size; 

	if (PRINT_FILL_EN)
		printf_hex_ex("tx_buf: ", "\n", m_tx_info.tx_buf, fill_size, "");

	ifer(P2PfillClose(&fill_frame));
	ifer(PfillClose(&fill_server_apdu));
	ifer(PfillClose(&fill_get_response));
	ifer(PfillClose(&fill_get_response_normal));
	ifer(PfillClose(&fill_a_result_normal));
	ifer(PfillClose(&fill_get_result));
	ifer(PfillClose(&fill_data));
#endif
	return 0;
}
//}}}


//{{{ get_request_normal_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetRequestNormalList;
static cp_t PdoGetRequestNormalListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程

	PdoGetRequestNormalList *derive = (PdoGetRequestNormalList*)doa;
	// fill仓库，用来管理Pfill指针
	// 创建了两个仓库，一个用来保存所有fill指针，负责fill的生命周期，即调用fill的close+qfree
	// 一个用来登记fill_a_result_normal对象列表
	// 为什么所有fill的生命周期，要用一个fill_repository来管理，因为其默认记录了fill的生成顺序，也即释放顺序。
	// 如果分成多个fill_repository，则这个顺序就不好维护了。
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在get_request_choice中, get_request_normal是当前的choice
	P2GetRequestPcut *gr = (P2GetRequestPcut*)cut;
	P2GetRequestNormalListPcut *grnl = (P2GetRequestNormalListPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grnl == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetRequestNormalListName));

	const char * const grnl_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grnl_mem_len = PcutIxLen(&gr->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("get_request_normal_list_mem: ", "\n", grnl_mem, grnl_mem_len, "");

	// 得到传入的请求数据
	P2GetRequestNormalListValue grnl_value = kP2GetRequestNormalListValueDef;
	ifer(P2GetRequestNormalListPcutValue(&grnl_value, grnl_mem));


	// 解帧，得到piid + sequence of oad 
	// fill仓库，用来管理Pfill指针
	// 创建了两个仓库，一个用来保存所有fill指针，负责fill的生命周期，即调用fill的close+qfree
	// 一个用来登记fill_a_result_normal对象列表
	// 为什么所有fill的生命周期，要用一个fill_repository来管理，因为其默认记录了fill的生成顺序，也即释放顺序。
	// 如果分成多个fill_repository，则这个顺序就不好维护了。
	//PfillRepository fill_repository_life = kPfillRepositoryDef;	// 用来调用fill的close和qfree
	//ifer(PfillRepositoryOpen(&fill_repository_life));

	// a_result_normal fill列表，传入sequence of中
	PfillRepository fill_repository_a_result_normal = kPfillRepositoryDef;	
	ifer(PfillRepositoryOpen(&fill_repository_a_result_normal));
	// 生成n个fill_a_result_normal对象，托管给fill_repository
	for (int i = 0; i < grnl_value.oad.num; ++i)
	{
		const cp_t cp = FillAResultNormalOad(&fill_repository_a_result_normal, fill_repository_life, grnl_value.oad.list[i]);
		// 出现错误，关闭所有对象，释放所有申请的内存
		if (0 != cp)
		{
			ifer(PfillRepositoryClose(&fill_repository_a_result_normal));
			return cp;
		}
	}

	// 下面所有的Pfill，要用fill_repository_life管理生命周期，
	// Pfill fill_sequence_of_a_result_normal = kPfillDef;
	// ifer(PfillOpen(&fill_sequence_of_a_result_normal));
	Pfill *fill_sequence_of_a_result_normal = PfillRepositoryFactory(fill_repository_life, "sequence_of_a_result_normal");
	ifbr(NULL != fill_sequence_of_a_result_normal);
	// QarraySetPrint(&fill_sequence_of_a_result_normal.array, QarrayPrintFull);
	//qos_printf("oad=%08x\n", grn_value.oad);
	ifer(P2SequenceOfVarLenFillInit(fill_sequence_of_a_result_normal, &fill_repository_a_result_normal));

	P2Piid piid_split = kP2PiidDef;
	P2PiidPart(&piid_split, grnl_value.piid);
	const P2PiidAcd piid_acd_value = {piid_split.service_priority, 0, piid_split.service_code};
	//Pfill fill_get_response_normal_list = kPfillDef;
	//ifer(PfillOpen(&fill_get_response_normal_list));
	Pfill *fill_get_response_normal_list = PfillRepositoryFactory(fill_repository_life, "get_response_normal_list");
	ifbr(NULL != fill_get_response_normal_list);
	ifer(P2GetResponseNormalListFillInit(fill_get_response_normal_list, &piid_acd_value, fill_sequence_of_a_result_normal));

	//Pfill fill_get_response = kPfillDef;
	//ifer(PfillOpen(&fill_get_response));
	Pfill *fill_get_response = PfillRepositoryFactory(fill_repository_life, "get_response");
	ifbr(NULL != fill_get_response);
	ifer(P2GetResponseFillInit(fill_get_response, kP2GetResponseChoiceGetResponseNormalList, fill_get_response_normal_list));

	// fill_server_apdu就是Lud/Apdu
	//Pfill fill_server_apdu = kPfillDef;
	//ifer(PfillOpen(&fill_server_apdu));
	Pfill *fill_apdu = PfillRepositoryFactory(fill_repository_life, kP2NameApdu);
	ifbr(NULL != fill_apdu);
	ifer(P2ServerApduFillInit(fill_apdu, kP2ServerApduChoiceGetResponse, fill_get_response, NULL, NULL));

	// 只需要关闭下面仓库，其它fill都已经托管给fill_repository_life.
	ifer(PfillRepositoryClose(&fill_repository_a_result_normal));
	return 0;
}
//}}}


//{{{ do_connect_request
// client_apdu_choice_connect_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoConnectRequest;
static cp_t PdoConnectRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const int kPrintPartEn = 0;		// 打印解帧过程
	dvb(ix == kP2ChoicePartIxVar);

	PdoConnectRequest *derive = (PdoConnectRequest*)doa;

	// 可以确定，当前处在client_apdu_choice中, connect_request是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)cut;
	P2ConnectRequestPcut *cr = (P2ConnectRequestPcut*)P2ChoicePcutVar(&cac->choice);
	dvb(cr == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2ConnectRequestName));

	const char * const connect_request_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int connect_request_mem_len = PcutIxLen(&cac->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("connect_request_mem: ", "\n", connect_request_mem, connect_request_mem_len, "");
	// 再按connect_request来解析+执行connect_request_mem.

	// todo: execute frame
	return 0;
}
//}}}


//{{{ get_request
typedef struct {
	Pdo doa;
} PdoGetRequestFail;
static cp_t PdoGetRequestProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t get_request_choice = (uint8_t)(*whole);
	qos_printf("This get_request_choice is to be implemented. get_request_choice=%02xH\n", get_request_choice);
	return cph;
}
#define kPdoGetRequestFailDef { PDO_INIT(PdoGetRequestProcessFail) }




// client_apdu_choice_get_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoGetRequest;
static cp_t PdoGetRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoGetRequest *derive = (PdoGetRequest*)doa;

	// 可以确定，当前处在client_apdu_choice中, get_request是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)cut;
	P2GetRequestPcut *gr = (P2GetRequestPcut*)P2ChoicePcutVar(&cac->choice);
	dvb(gr == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2GetRequestName));

	const char * const get_request_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int get_request_mem_len = PcutIxLen(&cac->choice.base, 0, ix, whole);

	//printf_hex_ex("get_request_mem: ", "\n", get_request_mem, get_request_mem_len, "");
	// 再按get_request来解析+执行get_request_mem.


	PdoGetRequestNormal do_get_request_normal = { 
		PDO_INIT(PdoGetRequestNormalProcess), derive->fill_repository_life };
	PdoGetRequestNormalList do_get_request_normal_list = { 
		PDO_INIT(PdoGetRequestNormalListProcess), derive->fill_repository_life };
	PdoGetRequestFail do_fail = kPdoGetRequestFailDef;
	Pdo* const kDoTable[kP2GetRequestChoiceNum] = {
		&do_get_request_normal.doa,	// kP2GetRequestChoiceGetRequestNormal = 1,		// 请求读取一个对象属性 [1] GetRequestNormal，
		&do_get_request_normal_list.doa,	// kP2GetRequestChoiceGetRequestNormalList = 2,	// 请求读取若干个对象属性 [2] GetRequestNormalList，
		&do_fail.doa,	// kP2GetRequestChoiceGetRequestRecord = 3,		// 请求读取一个记录型对象属性 [3] GetRequestRecord，
		&do_fail.doa,	// kP2GetRequestChoiceGetRequestRecordList = 4,	// 请求读取若干个记录型对象属性 [4] GetRequestRecordList，
		&do_fail.doa,	// kP2GetRequestChoiceGetRequestNext = 5,			// 请求读取分帧传输的下一帧 [5] GetRequestNext，
		&do_fail.doa,	// kP2GetRequestChoiceGetRequestMD5 = 6,			// 请求读取一个对象属性的 MD5 值 [6] GetRequestMD5，
		&do_fail.doa,	// kP2GetRequestChoiceGetRequestSimplifyRecord = 23,	// 读取一个精简的记录型对象属性请求 [23] GetRequestSimplifyRecord
	};
	P2ChoiceVarDoTableSet(&gr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&gr->choice.base, 0, 0, kPcutIxAll, get_request_mem);
	P2ChoiceVarDoTableSet(&gr->choice, NULL);
	return cp;
}
//}}}


//{{{ set_request_normal
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoSetRequestNormal;
static cp_t PdoSetRequestNormalProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoSetRequestNormal *derive = (PdoSetRequestNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在action_request_choice中, action_request_normal是当前的choice
	P2SetRequestChoicePcut *sr = (P2SetRequestChoicePcut*)cut;
	P2SetRequestNormalPcut *srn = (P2SetRequestNormalPcut*)P2ChoicePcutVar(&sr->choice);
	dvb(srn == (void*)PcutFindSubRecursionDepth(&sr->choice.base, kP2SetRequestNormalName));

	const char * const srn_mem = PcutIxPtrConst(&sr->choice.base, ix, whole);
	const int srn_mem_len = PcutIxLen(&sr->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("set_request_normal_mem: ", "\n", srn_mem, srn_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ set_request
typedef struct {
	Pdo doa;
} PdoSetRequestFail;
static cp_t PdoSetRequestProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t set_request_choice = (uint8_t)(*whole);
	qos_printf("This set_request_choice is to be implemented. set_request_choice=%02xH\n", set_request_choice);
	return cph;
}
#define kPdoSetRequestFailDef { PDO_INIT(PdoSetRequestProcessFail) }




// client_apdu_choice_set_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoSetRequest;
static cp_t PdoSetRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoSetRequest *derive = (PdoSetRequest*)doa;

	// 可以确定，当前处在client_apdu_choice中, action_request是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)cut;
	P2SetRequestChoicePcut *sr = (P2SetRequestChoicePcut*)P2ChoicePcutVar(&cac->choice);
	dvb(sr == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2SetRequestName));

	const char * const set_request_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int set_request_mem_len = PcutIxLen(&cac->choice.base, 0, ix, whole);

	//printf_hex_ex("get_request_mem: ", "\n", get_request_mem, get_request_mem_len, "");
	// 再按get_request来解析+执行get_request_mem.


	PdoSetRequestNormal do_action_request_normal = { 
		PDO_INIT(PdoSetRequestNormalProcess), derive->fill_repository_life };
	PdoSetRequestFail do_fail = kPdoSetRequestFailDef;
	Pdo* const kDoTable[kP2SetRequestChoiceNum] = {
		&do_action_request_normal.doa,	// 请求操作一个对象方法 [1] SetRequestNormal
		&do_fail.doa,	// 请求操作若干个对象方法 [2] SetRequestNormalList
		&do_fail.doa,	// 请求操作若干个对象方法后读取若干个对象属性 [3] SetThenGetRequestNormalLi st
	};
	P2ChoiceVarDoTableSet(&sr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&sr->choice.base, 0, 0, kPcutIxAll, set_request_mem);
	P2ChoiceVarDoTableSet(&sr->choice, NULL);
	return cp;
}
//}}}


//{{{ action_request_normal
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionRequestNormal;
static cp_t PdoActionRequestNormalProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoActionRequestNormal *derive = (PdoActionRequestNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在action_request_choice中, action_request_normal是当前的choice
	P2ActionRequestChoicePcut *ar = (P2ActionRequestChoicePcut*)cut;
	P2ActionRequestNormalPcut *arn = (P2ActionRequestNormalPcut*)P2ChoicePcutVar(&ar->choice);
	dvb(arn == (void*)PcutFindSubRecursionDepth(&ar->choice.base, kP2ActionRequestNormalName));

	const char * const arn_mem = PcutIxPtrConst(&ar->choice.base, ix, whole);
	const int arn_mem_len = PcutIxLen(&ar->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("action_request_normal_mem: ", "\n", arn_mem, arn_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ action_request_normal_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionRequestNormalList;
static cp_t PdoActionRequestNormalListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoActionRequestNormalList *derive = (PdoActionRequestNormalList*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在action_request_choice中, action_request_normal_list是当前的choice
	P2ActionRequestChoicePcut *ar = (P2ActionRequestChoicePcut*)cut;
	P2ActionRequestNormalListPcut *arnl = (P2ActionRequestNormalListPcut*)P2ChoicePcutVar(&ar->choice);
	dvb(arnl == (void*)PcutFindSubRecursionDepth(&ar->choice.base, kP2ActionRequestNormalListName));

	const char * const arnl_mem = PcutIxPtrConst(&ar->choice.base, ix, whole);
	const int arnl_mem_len = PcutIxLen(&ar->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("action_request_normal_list_mem: ", "\n", arnl_mem, arnl_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ action_then_get_request_normal_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionThenGetRequestNormalList;
static cp_t PdoActionThenGetRequestNormalListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoActionThenGetRequestNormalList *derive = (PdoActionThenGetRequestNormalList*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在action_request_choice中, action_then_get_request_normal_list是当前的choice
	P2ActionRequestChoicePcut *ar = (P2ActionRequestChoicePcut*)cut;
	P2ActionThenGetRequestNormalListPcut *atgrnl = (P2ActionThenGetRequestNormalListPcut*)P2ChoicePcutVar(&ar->choice);
	dvb(atgrnl == (void*)PcutFindSubRecursionDepth(&ar->choice.base, kP2ActionThenGetRequestNormalListName));

	const char * const atgrnl_mem = PcutIxPtrConst(&ar->choice.base, ix, whole);
	const int atgrnl_mem_len = PcutIxLen(&ar->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("action_then_get_request_normal_list_mem: ", "\n", atgrnl_mem, atgrnl_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ action_request
typedef struct {
	Pdo doa;
} PdoActionRequestFail;
static cp_t PdoActionRequestProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t action_request_choice = (uint8_t)(*whole);
	qos_printf("This action_request_choice is to be implemented. action_request_choice=%02xH\n", action_request_choice);
	return cph;
}
#define kPdoActionRequestFailDef { PDO_INIT(PdoActionRequestProcessFail) }




// client_apdu_choice_action_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoActionRequest;
static cp_t PdoActionRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoActionRequest *derive = (PdoActionRequest*)doa;

	// 可以确定，当前处在client_apdu_choice中, action_request是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)cut;
	P2ActionRequestChoicePcut *ar = (P2ActionRequestChoicePcut*)P2ChoicePcutVar(&cac->choice);
	dvb(ar == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2ActionRequestName));

	const char * const action_request_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int action_request_mem_len = PcutIxLen(&cac->choice.base, 0, ix, whole);

	//printf_hex_ex("get_request_mem: ", "\n", get_request_mem, get_request_mem_len, "");
	// 再按get_request来解析+执行get_request_mem.


	PdoActionRequestNormal do_action_request_normal = { 
		PDO_INIT(PdoActionRequestNormalProcess), derive->fill_repository_life };
	PdoActionRequestNormalList do_action_request_normal_list = { 
		PDO_INIT(PdoActionRequestNormalListProcess), derive->fill_repository_life };
	PdoActionThenGetRequestNormalList do_action_then_get_request_normal_list = { 
		PDO_INIT(PdoActionThenGetRequestNormalListProcess), derive->fill_repository_life };
	// PdoActionRequestFail do_fail = kPdoActionRequestFailDef;
	Pdo* const kDoTable[kP2ActionRequestChoiceNum] = {
		&do_action_request_normal.doa,	// 请求操作一个对象方法 [1] ActionRequestNormal
		&do_action_request_normal_list.doa,	// 请求操作若干个对象方法 [2] ActionRequestNormalList
		&do_action_then_get_request_normal_list.doa,	// 请求操作若干个对象方法后读取若干个对象属性 [3] ActionThenGetRequestNormalLi st
	};
	P2ChoiceVarDoTableSet(&ar->choice, kDoTable);
	const cp_t cp = PcutIxDo(&ar->choice.base, 0, 0, kPcutIxAll, action_request_mem);
	P2ChoiceVarDoTableSet(&ar->choice, NULL);
	return cp;
}
//}}}


//{{{ report_response_record_list
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoReportResponseRecordList;
static cp_t PdoReportResponseRecordListProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoReportResponseRecordList *derive = (PdoReportResponseRecordList*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在report_response_choice中, report_response.record_list是当前的choice
	P2ReportResponseChoicePcut *rr = (P2ReportResponseChoicePcut*)cut;
	P2ReportResponseRecordListPcut *rrrl = (P2ReportResponseRecordListPcut*)P2ChoicePcutVar(&rr->choice);
	dvb(rrrl == (void*)PcutFindSubRecursionDepth(&rr->choice.base, kP2ReportResponseRecordListName));

	const char * const rrrl_mem = PcutIxPtrConst(&rr->choice.base, ix, whole);
	const int rrrl_mem_len = PcutIxLen(&rr->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("report_response.record_list mem: ", "\n", rrrl_mem, rrrl_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ report_response
typedef struct {
	Pdo doa;
} PdoReportResponseFail;
static cp_t PdoReportResponseProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t report_response_choice = (uint8_t)(*whole);
	qos_printf("This report_response_choice is to be implemented. report_response_choice=%02xH\n", report_response_choice);
	return cph;
}
#define kPdoReportResponseFailDef { PDO_INIT(PdoReportResponseProcessFail) }




// client_apdu_choice_report_response
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoReportResponse;
static cp_t PdoReportResponseProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoReportResponse *derive = (PdoReportResponse*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)cut;
	P2ReportResponseChoicePcut *rr = (P2ReportResponseChoicePcut*)P2ChoicePcutVar(&cac->choice);
	dvb(rr == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2ReportResponseName));

	const char * const report_response_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int report_response_mem_len = PcutIxLen(&cac->choice.base, 0, ix, whole);

	//printf_hex_ex("get_request_mem: ", "\n", get_request_mem, get_request_mem_len, "");
	// 再按get_request来解析+执行get_request_mem.


	PdoReportResponseRecordList do_report_response_record_list = { 
		PDO_INIT(PdoReportResponseRecordListProcess), derive->fill_repository_life };
	PdoReportResponseFail do_fail = kPdoReportResponseFailDef;
	Pdo* const kDoTable[kP2ReportResponseChoiceNum] = {
		&do_fail.doa,	// 上报若干个对象属性的响应 [1] ReportResponseList
		&do_report_response_record_list.doa,	// 上报若干个记录型对象属性的响应 [2] ReportResponseRecordList
		&do_fail.doa,	// 上报透明数据的响应 [3] ReportResponseTransData
		&do_fail.doa,	// 请求客户机服务的响应 [4] ReportResponseClientService
		&do_fail.doa,	// 上报精简的记录型对象属性的响应 [6] ReportResponseSimplifyRecord
	};
	P2ChoiceVarDoTableSet(&rr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&rr->choice.base, 0, 0, kPcutIxAll, report_response_mem);
	P2ChoiceVarDoTableSet(&rr->choice, NULL);
	return cp;
}
//}}}


//{{{ proxy_request_trans_command_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoProxyRequestTransCommandRequest;
static cp_t PdoProxyRequestTransCommandRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoProxyRequestTransCommandRequest *derive = (PdoProxyRequestTransCommandRequest*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在proxy_request_choice中, trans_command_request是当前的choice
	P2ProxyRequestChoicePcut *pr = (P2ProxyRequestChoicePcut*)cut;
	P2ProxyRequestTransCommandRequestPcut *prtcr = (P2ProxyRequestTransCommandRequestPcut*)P2ChoicePcutVar(&pr->choice);
	dvb(prtcr == (void*)PcutFindSubRecursionDepth(&pr->choice.base, kP2ProxyRequestTransCommandRequestName));

	const char * const prtcr_mem = PcutIxPtrConst(&pr->choice.base, ix, whole);
	const int prtcr_mem_len = PcutIxLen(&pr->choice.base, 0, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("proxy_request.trans_command_request_mem: ", "\n", prtcr_mem, prtcr_mem_len, "");

	// 解帧，得到piid + omd + data
	// todo: 解帧，执行
	return 0;
}
//}}}


//{{{ proxy_request
typedef struct {
	Pdo doa;
} PdoProxyRequestFail;
static cp_t PdoProxyRequestProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t proxy_request_choice = (uint8_t)(*whole);
	qos_printf("This proxy_request_choice is to be implemented. proxy_request_choice=%02xH\n", proxy_request_choice);
	return cph;
}
#define kPdoProxyRequestFailDef { PDO_INIT(PdoProxyRequestProcessFail) }




typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoProxyRequest;
static cp_t PdoProxyRequestProcess(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoProxyRequest *derive = (PdoProxyRequest*)doa;

	// 可以确定，当前处在client_apdu_choice中, report_response是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)cut;
	P2ProxyRequestChoicePcut *pr = (P2ProxyRequestChoicePcut*)P2ChoicePcutVar(&cac->choice);
	dvb(pr == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2ProxyRequestName));

	const char * const proxy_request_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int proxy_request_mem_len = PcutIxLen(&cac->choice.base, 0, ix, whole);

	//printf_hex_ex("get_request_mem: ", "\n", get_request_mem, get_request_mem_len, "");
	// 再按get_request来解析+执行get_request_mem.


	PdoProxyRequestTransCommandRequest do_trans_command_request = { 
		PDO_INIT(PdoProxyRequestTransCommandRequestProcess), derive->fill_repository_life };
	PdoProxyRequestFail do_fail = kPdoProxyRequestFailDef;
	Pdo* const kDoTable[kP2ProxyRequestChoiceNum] = {
		&do_fail.doa,	// 请求代理读取若干个服务器的若干个对象属性 [1] ProxyGetRequestList
		&do_fail.doa,	// 请求代理读取一个服务器的一个记录型对象属性 [2] ProxyGetRequestRecord
		&do_fail.doa,	// 请求代理设置若干个服务器的若干个对象属性 [3] ProxySetRequestList
		&do_fail.doa,	// 请求代理设置后读取若干个服务器的若干个对象属性 [4] ProxySetThenGetRequestList
		&do_fail.doa,	// 请求代理操作若干个服务器的若干个对象方法 [5] ProxyActionRequestList
		&do_fail.doa,	// 请求代理操作后读取若干个服务器的若干个对象方法和属性 [6] ProxyActionThenGetRequestList
		&do_trans_command_request.doa,	// 请求代理透明转发 命令 [7] ProxyTransCommandRequest
		&do_fail.doa,	// 请求代理终端内部转发命令 [8] ProxyInnerTransCommandRequest
	};
	P2ChoiceVarDoTableSet(&pr->choice, kDoTable);
	const cp_t cp = PcutIxDo(&pr->choice.base, 0, 0, kPcutIxAll, proxy_request_mem);
	P2ChoiceVarDoTableSet(&pr->choice, NULL);
	return cp;
}
//}}}


//{{{ client-apdu
// do-fail
typedef struct {
	Pdo doa;
} PdoClientApduFail;
static cp_t PdoClientApduProcessFail(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	const uint8_t client_apdu_choice = (uint8_t)(*whole);
	qos_printf("This client_apdu_choice is to be implemented. client_apdu_choice=%02xH\n", client_apdu_choice);
	return cph;
}
#define kPdoClientApduFailDef { PDO_INIT(PdoClientApduProcessFail) }
cp_t P2ProcessClientApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	P2ClientApduPcut ca = kP2ClientApduPcutDef;

	ifer(P2ClientApduPcutOpen(&ca));
	printf_hex_ex(LINEND "client_apdu mem: ", LINEND, apdu, apdu_size, "");
	PcutAllPrint(&ca.base, 0, 0, apdu);

	PdoConnectRequest do_connect_request = { PDO_INIT(PdoConnectRequestProcess), fill_repository_life };
	PdoGetRequest do_get_request = { PDO_INIT(PdoGetRequestProcess), fill_repository_life };
	PdoSetRequest do_set_request = { PDO_INIT(PdoSetRequestProcess), fill_repository_life };
	PdoActionRequest do_action_request = { PDO_INIT(PdoActionRequestProcess), fill_repository_life };
	PdoReportResponse do_report_response = { PDO_INIT(PdoReportResponseProcess), fill_repository_life };
	PdoProxyRequest do_proxy_request = { PDO_INIT(PdoProxyRequestProcess), fill_repository_life };
	PdoClientApduFail do_fail = kPdoClientApduFailDef;
	Pdo* const kDoTable[kP2ClientApduChoiceNum] = {
		&do_connect_request.doa,	// kP2ClientApduChoiceConnectRequest = 2,	// 建立应用连接请求 [2] CONNECT-Request，
		&do_fail.doa,	// kP2ClientApduChoiceReleaseRequest = 3,	// 断开应用连接请求 [3] RELEASE-Request，
		&do_get_request.doa,	// kP2ClientApduChoiceGetRequest = 5,	// 读取请求 [5] GET-Request，
		&do_set_request.doa,	// kP2ClientApduChoiceSetRequest = 6,	// 设置请求 [6] SET-Request，
		&do_action_request.doa,	// kP2ClientApduChoiceActionRequest = 7,	// 操作请求 [7] ACTION-Request，
		&do_report_response.doa,	// kP2ClientApduChoiceReportResponse = 8,	// 上报应答 [8] REPORT-Response，
		&do_proxy_request.doa,	// kP2ClientApduChoiceProxyRequest = 9,	// 代理请求 [9] PROXY-Request，
		&do_fail.doa,	// kP2ClientApduChoiceCompactGetRequest = 69,	// 紧凑读取请求 [69] COMPACT-GET-Request，
		&do_fail.doa,	// kP2ClientApduChoiceCompactSetRequest = 70,	// 紧凑设置请求 [70] COMPACT-SET-Request，
		&do_fail.doa,	// kP2ClientApduChoiceCompactProxyRequest = 73,	// 紧凑代理请求 [73] COMPACT-PROXY-Request，
		&do_fail.doa,	// kP2ClientApduChoiceErrorResponse = 110	// 异常响应 [110] ERROR-Response
	};
	P2ChoiceVarDoTableSet(&ca.choice_cut.choice, kDoTable);
	const cp_t cp = PcutIxDo(&ca.base, 0, 0, kPcutIxAll, apdu);
	P2ChoiceVarDoTableSet(&ca.choice_cut.choice, NULL);

	ifer(P2ClientApduPcutClose(&ca));
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
cp_t P2ProcessClientApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ProcessClientApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

