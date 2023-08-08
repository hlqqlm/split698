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

20230511-----------------------------------------------------------------------
huanglin 创建.
执行dlt698协议
*/
#include <string.h>
#include <algorithm>

#include "qos/qtest.h"
#include "qos/qstr.h"
#include "qos/qmem.h"

// dlt698_45
#include "protocol/pfill_repository.h"

#include "p2_config.h"

#include "p2_double_long_unsigned.h"
#include "p2_double_long.h"
#include "p2_long_unsigned.h"
#include "p2_octet_string.h"
#include "p2_sequence_of_var_len.h"
#include "p2_array.h"


//#include "p2_link_apdu_choice.h"
//#include "p2_link_apdu.h"
#include "p2_link_apdu_choice.h"
#include "p2_process_link_apdu.h"

/*
#include "p2_get_request_normal.h"
#include "p2_get_request_normal_list.h"
#include "p2_get_request.h"
#include "p2_client_apdu_choice.h"
#include "p2_client_apdu.h"
*/
#include "p2_client_apdu_choice.h"
#include "p2_process_client_apdu.h"

#include "p2_get_response.h"
#include "p2_get_response_normal.h"
#include "p2_get_response_normal_list.h"
#include "p2_server_apdu_choice.h"
#include "p2_server_apdu.h"

#if 0
#include "p2_security_apdu_choice.h"
#include "p2_security_request.h"
#include "p2_security_response.h"
#include "p2_security_response_data_choice.h"
#endif
#include "p2_security_apdu_choice.h"
#include "p2_process_security_apdu.h"

#include "p2_process.h"
#include "p2_process.xcp.h"
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


//{{{ class
cp_t P2ProcessOpen(P2Process *m)
{
	ifer(P2LinkOpen(&m->link));
	return 0;
}
cp_t P2ProcessClose(P2Process *m)
{
	dve(P2ProcessValid(m));
	ifer(P2LinkClose(&m->link));
	return 0;
}
cp_t P2ProcessValid(P2Process *m)
{
	ifbr(NULL != m);
	ifer(P2LinkValid(&m->link));
	return 0;
}
//}}}


#if 0
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
	// qos_printf("oad=%08x\r\n", oad);
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
			qos_printf("I can NOT response this oad in ResponseOad(). oad=%08xH\r\n", oad);
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
static cp_t PdoGetRequestNormalProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	const int kPrintPartEn = 0;		// 打印解帧过程
	//const int PRINT_FILL_EN = 1;		// 是否打印填充帧过程
	PdoGetRequestNormal *derive = (PdoGetRequestNormal*)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在get_request_choice中, get_request_normal是当前的choice
	P2GetRequestPcut *gr = (P2GetRequestPcut*)part;
	P2GetRequestNormalPcut *grn = (P2GetRequestNormalPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grn == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetRequestNormalName));

	const char * const grn_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grn_mem_len = PcutIxLen(&gr->choice.base, ix, whole);

	//qos_printf("grn_mem_len=%d\r\n", grn_mem_len);
	//printf_hex(grn_mem, grn_mem_len, " ");
	//qos_printf("\r\n");
	if (kPrintPartEn)
		printf_hex_ex("grn_mem_len: ", "\r\n", grn_mem, grn_mem_len, "");

	// 解帧，得到piid + oad
	P2GetRequestNormalValue grn_value = kP2GetRequestNormalValueDef;
	ifer(P2GetRequestNormalPcutValue(&grn_value, grn_mem));
	//P2Piid piid = kP2PiidDef;
	//P2PiidPart(&piid, grn_value.piid);

	if (kPrintPartEn)
		qos_printf("get_request_normal_value.[oad=%08xH piid=%02xH]\r\n", grn_value.oad, grn_value.piid);

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
		printf_hex_ex("tx_buf: ", "\r\n", m_tx_info.tx_buf, fill_size, "");

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
static cp_t PdoGetRequestNormalListProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
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
	P2GetRequestPcut *gr = (P2GetRequestPcut*)part;
	P2GetRequestNormalListPcut *grnl = (P2GetRequestNormalListPcut*)P2ChoicePcutVar(&gr->choice);
	dvb(grnl == (void*)PcutFindSubRecursionDepth(&gr->choice.base, kP2GetRequestNormalListName));

	const char * const grnl_mem = PcutIxPtrConst(&gr->choice.base, ix, whole);
	const int grnl_mem_len = PcutIxLen(&gr->choice.base, ix, whole);

	if (kPrintPartEn)
		printf_hex_ex("get_request_normal_list_mem: ", "\r\n", grnl_mem, grnl_mem_len, "");

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
	//qos_printf("oad=%08x\r\n", grn_value.oad);
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
#endif

#if 0
//{{{ client-apdu
typedef struct {
	Pdo doa;
} PdoGetRequestFail;
static cp_t PdoGetRequestProcessFail(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	const uint8_t get_request_choice = (uint8_t)(*whole);
	qos_printf("This get_request_choice is to be implemented. get_request_choice=%02xH\r\n", get_request_choice);
	return cph;
}
#define kPdoGetRequestFailDef { PDO_INIT(PdoGetRequestProcessFail) }

 
// client_apdu_choice_get_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoClientApduChoiceGetRequest;
static cp_t PdoClientApduChoiceGetRequestProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);

	PdoClientApduChoiceGetRequest *derive = (PdoClientApduChoiceGetRequest*)doa;

	// 可以确定，当前处在client_apdu_choice中, get_request是当前的choice
	P2ClientApduChoicePcut *cac = (P2ClientApduChoicePcut*)part;
	P2GetRequestPcut *gr = (P2GetRequestPcut*)P2ChoicePcutVar(&cac->choice);
	dvb(gr == (void*)PcutFindSubRecursionDepth(&cac->choice.base, kP2GetRequestName));

	const char * const get_request_mem = PcutIxPtrConst(&cac->choice.base, ix, whole);
	const int get_request_mem_len = PcutIxLen(&cac->choice.base, ix, whole);

	//printf_hex_ex("get_request_mem: ", "\r\n", get_request_mem, get_request_mem_len, "");
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



// do-fail
typedef struct {
	Pdo doa;
} PdoClientApduFail;
static cp_t PdoClientApduProcessFail(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	const uint8_t client_apdu_choice = (uint8_t)(*whole);
	qos_printf("This client_apdu_choice is to be implemented. client_apdu_choice=%02xH\r\n", client_apdu_choice);
	return cph;
}
#define kPdoClientApduFailDef { PDO_INIT(PdoClientApduProcessFail) }
static cp_t ProcessClientApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	P2ClientApduPcut ca = kP2ClientApduPcutDef;

	ifer(P2ClientApduPcutOpen(&ca));
	printf_hex_ex("\r\nclient_apdu mem: ", "\r\n", apdu, apdu_size, "");
	PcutAllPrint(&ca.base, 0, apdu);

	PdoClientApduChoiceGetRequest do_get_request = { PDO_INIT(PdoClientApduChoiceGetRequestProcess), fill_repository_life };
	PdoClientApduFail do_fail = kPdoClientApduFailDef;
	Pdo* const kDoTable[kP2ClientApduChoiceNum] = {
		&do_fail.doa,	// kP2ClientApduChoiceConnectRequest = 2,	// 建立应用连接请求 [2] CONNECT-Request，
		&do_fail.doa,	// kP2ClientApduChoiceReleaseRequest = 3,	// 断开应用连接请求 [3] RELEASE-Request，
		&do_get_request.doa,	// kP2ClientApduChoiceGetRequest = 5,	// 读取请求 [5] GET-Request，
		&do_fail.doa,	// kP2ClientApduChoiceSetRequest = 6,	// 设置请求 [6] SET-Request，
		&do_fail.doa,	// kP2ClientApduChoiceActionRequest = 7,	// 操作请求 [7] ACTION-Request，
		&do_fail.doa,	// kP2ClientApduChoiceReportResponse = 8,	// 上报应答 [8] REPORT-Response，
		&do_fail.doa,	// kP2ClientApduChoiceProxyRequest = 9,	// 代理请求 [9] PROXY-Request，
		&do_fail.doa,	// kP2ClientApduChoiceCompactGetRequest = 69,	// 紧凑读取请求 [69] COMPACT-GET-Request，
		&do_fail.doa,	// kP2ClientApduChoiceCompactSetRequest = 70,	// 紧凑设置请求 [70] COMPACT-SET-Request，
		&do_fail.doa,	// kP2ClientApduChoiceCompactProxyRequest = 73,	// 紧凑代理请求 [73] COMPACT-PROXY-Request，
		&do_fail.doa,	// kP2ClientApduChoiceErrorResponse = 110	// 异常响应 [110] ERROR-Response
	};
	P2ChoiceVarDoTableSet(&ca.choice_part.choice, kDoTable);
	const cp_t cp = PcutIxDo(&ca.base, 0, 0, kPcutIxAll, apdu);
	P2ChoiceVarDoTableSet(&ca.choice_part.choice, NULL);

	ifer(P2ClientApduPcutClose(&ca));
	return cp;
}
//}}}
#endif

#if 0
//{{{ security_request
typedef struct {
	Pdo doa;
	PfillRepository *fill_repository_life;
} PdoSecurityRequestDataPlain;
static cp_t PdoSecurityRequestDataPlainProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	// qos_printf("plain\r\n");
	dvb(ix == kP2ChoicePartIxVar);
	PdoSecurityRequestDataPlain *derive = (PdoSecurityRequestDataPlain*)doa;
	PfillRepository *fill_repository_life = derive->fill_repository_life;

	// 可以确定，当前处在security-request-data中, plain是当前的choice
	P2SecurityRequestDataChoicePcut *srdc = (P2SecurityRequestDataChoicePcut*)part;
	P2OctetStringPcut *os = (P2OctetStringPcut*)P2ChoicePcutVar(&srdc->choice);
	dvb(os == (void*)PcutFindSubRecursionDepth(&srdc->choice.base, kP2SecurityRequestDataNamePlain));

	const char * const octet_string_mem = PcutIxPtrConst(&srdc->choice.base, ix, whole);
	//const int octet_string_len = PcutIxLen(&srdc->choice.base, ix, whole);

	const char * const content = PcutIxPtrConst(&os->base, kP2OctetStringPartIxContent, octet_string_mem);
	const int content_len = PcutIxLen(&os->base, kP2OctetStringPartIxContent, octet_string_mem);

	// printf_hex_ex("security request data plain content: ", "\r\n", content, content_len, "");
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
static cp_t PdoSecurityRequestDataCipherProcess(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	return cph;
}
#define kPdoSecurityRequestDataCipherDef { PDO_INIT(PdoSecurityRequestDataCipherProcess) }
//}}}


//{{{ security-apdu
typedef struct PdoSecurityApduRequestS {
	Pdo doa;
	Pcut *part;
	int lud_size;	
	PfillRepository *fill_repository_life;
} PdoSecurityApduRequest;
static cp_t PdoProcessSecurityApduRequest(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	dvb(ix == kP2ChoicePartIxVar);
	struct PdoSecurityApduRequestS *derive = (struct PdoSecurityApduRequestS *)doa;
	PfillRepository * const fill_repository_life = derive->fill_repository_life;
	dve(PfillRepositoryValid(fill_repository_life));

	// 可以确定，当前处在security-apdu中，choice为request
	P2SecurityApduChoicePcut *sac = (P2SecurityApduChoicePcut*)part;
	P2ChoicePcut *choice = (P2ChoicePcut*)part;
	Pcut *sac_base = (Pcut*)part;
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
	

	dvb(part == derive->part);
	const int whole_len = PcutIxLen(part, kPcutIxAll, whole);
	ifbr(whole_len == derive->lud_size);
	const int invalid_ix = PcutInvalidIx(part, whole);
	ifbr(-1 == invalid_ix);
	printf_hex_ex("\r\nsecurity_apdu_request mem: ", "\r\n", whole, whole_len, "");
	PcutAllPrint(part, 0, whole);
	
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
static cp_t PdoProcessSecurityApduResponse(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	return cph;
}
#define kPdoSecurityApduResponseDef { PDO_INIT(PdoProcessSecurityApduResponse) }
static cp_t ProcessSecurityApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
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
#endif

#if 0
//{{{ link_apdu
static cp_t ProcessLinkApdu(PfillRepository *fill_repository_life, const char *apdu, int apdu_size)
{
	P2LinkApduPcut la = kP2LinkApduPcutDef;

	ifer(P2LinkApduPcutOpen(&la));
	printf_hex_ex("\r\nlink_apdu mem: ", "\r\n", apdu, apdu_size, "");
	PcutAllPrint(&la.base, 0, apdu);

	PdoLinkApduChoiceLinkRequest do_link_request = { PDO_INIT(PdoClientApduChoiceGetRequestProcess), fill_repository_life };
	PdoLinkApduFail do_fail = kPdoLinkApduFailDef;
	Pdo* const kDoTable[kP2LinkApduChoiceNum] = {
		&do_fail.doa,	// kP2ClientApduChoiceConnectRequest = 2,	// 建立应用连接请求 [2] CONNECT-Request，
		&do_fail.doa,	// kP2ClientApduChoiceReleaseRequest = 3,	// 断开应用连接请求 [3] RELEASE-Request，
		&do_get_request.doa,	// kP2ClientApduChoiceGetRequest = 5,	// 读取请求 [5] GET-Request，
	};
	P2ChoiceVarDoTableSet(&la.choice_part.choice, kDoTable);
	const cp_t cp = PcutIxDo(&la.base, 0, 0, kPcutIxAll, apdu);
	P2ChoiceVarDoTableSet(&la.choice_part.choice, NULL);

	ifer(P2LinkApduPcutClose(&la));
	return cp;
}
//}}}
#endif

//{{{ process
static cp_t ProcessApduCp(PfillRepository *fill_repository_life, uint8_t apdu_choice, const char *apdu, int apdu_size)
{
	if (0 == P2SecurityApduChoiceValid(apdu_choice))
	{
		// 按security_apdu解析
		ifer(P2ProcessSecurityApdu(fill_repository_life, apdu, apdu_size));
		// p2_security_apdu_choice.h
	}
	else if (0 == P2ClientApduChoiceValid(apdu_choice))
	{
		// 按client_apdu解析
		ifer(P2ProcessClientApdu(fill_repository_life, apdu, apdu_size));
	}
	else if (0 == P2LinkApduChoiceValid(apdu_choice))
	{
		// 按link_apdu解析
		ifer(P2ProcessLinkApdu(fill_repository_life, apdu, apdu_size));
	}
	else
	{
		qos_printf("This apdu_choice can NOT be processed. apdu_choice=%02x\r\n", apdu_choice);
		return cph;
	}

	return 0;
}
static cp_t ProcessApdu(PfillRepository *fill_repository_life, uint8_t apdu_choice, const char *apdu, int apdu_size)
{
	
	ifer(ProcessApduCp(fill_repository_life, apdu_choice, apdu, apdu_size));
	const int num = PfillRepositoryNum(fill_repository_life);
	if (0 == num)
	{
		qos_printf("Note: No answer for this rxing frame.\r\n");
		return 0;
	}

	// 找apdu
	ifbr(0 < num);
	// 认为最外层（最后一个）fill是apdu。apdu可能有多个名字：client_server_apdu，security_response等。
	// 可以默认最后一个是apdu。因为组帧，apdu一定是最后一个
	// 这样找有个好处，不用层层传递apdu指针。减轻传递变量的繁琐，也减少栈的占用，时间换空间
	const int last_ix = num - 1;		// 最后一个索引号
	ifer(PfillRepositoryIxValid(fill_repository_life, last_ix));
	const PfillRepositoryItem *fr_item_last = PfillRepositoryIxItemConst(fill_repository_life, last_ix);
	// 确认fill_apdu在fill_repository_life中，且是最后一个
	ifbr(NULL != fr_item_last);
	// ifbr(0 == strcmp(fr_item_last->name, kP2NameApdu));
	Pfill *fill_apdu = fr_item_last->fill;

	// 构建最外层
	P2Pfill fill_frame = kP2PfillDef(fill_apdu);
	const P2Control control = { kP2DirPrmServerResponse, 0, 0, kP2FunCodeUserData };
	const int addr_size = 6;
	const char kAddr[] = kAddrParameterMrCommAddress;
	const P2AddrValue addr = { kP2AddrSaTypeSingle
		, {
			kAddr[0],
			kAddr[1],  
			kAddr[2],  
			kAddr[3],  
			kAddr[4],  
			kAddr[5]
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
		printf_hex_ex("tx_buf: ", "\r\n", m_tx_info.tx_buf, fill_size, "");

	ifer(P2PfillClose(&fill_frame));
	return 0;
}
static cp_t Process(P2Process *m)
{
	dve(P2ProcessValid(m));
	P2Link * const link = &m->link;

	dve(P2LinkFrameValid(link));
	P2LinkFramePrint(link);
	// int P2LinkFrameSize(P2Link *m);
	
	P2Pcut * const part = P2LinkPart(link);
	const char * const frame = P2LinkBuf(link);

	const uint8_t apdu_choice = P2LudChoice(frame);
	const char * const apdu = P2Lud(frame);
	const int apdu_size = P2LudSize(frame);

	// fill仓库，用来管理Pfill指针
	// 用来保存所有fill指针，负责fill的生命周期，即调用fill的close+qfree
	// 为什么所有fill的生命周期，要用一个fill_repository来管理，因为其默认记录了fill的生成顺序，也即释放顺序。
	// 如果分成多个fill_repository，则这个顺序就不好维护了。
	const int malloc_cnt_old = MallocCnt();
	PfillRepository fill_repository_life = kPfillRepositoryDef;	// 用来调用fill的close和qfree
#if CHECK_MEM_LEAK_EN > 0
		qos_printf("Before process_apdu()\r\n");
		QmemtablePrintAllGlobal();
#endif
	ifer(PfillRepositoryOpen(&fill_repository_life));
	const cp_t cp = ProcessApdu(&fill_repository_life, apdu_choice, apdu, apdu_size);

	if (QmemtableCheckBarrier(QmemtableGlobal()) > 0)
		qos_printf("ERROR: Memory barrier WRONG.\r\n");

	ifer(PfillRepositoryClose(&fill_repository_life));
	const int malloc_cnt_curr = MallocCnt();
	if (malloc_cnt_old != malloc_cnt_curr)
	{
		qos_printf("WARNING: Memory maybe leaked. malloc_cnt old=%d curr=%d\r\n", malloc_cnt_old, malloc_cnt_curr);
		qos_printf("After process_apdu()\r\n");
		QmemtablePrintAllGlobal();
	}

	//const char * const lud = PcutIxPtrConst(&part->base, kP2PartIxLud, P2LinkBuf(link));		// const版本
	//const uint8_t choice = lud[0];
	//if (choice)
//调试这里

	return cp;
}

// 如何处理698中随机数：
// 方法1：回一个明文。（要确定）
// 方法2：回一个带mac的报文，mac随意。同时：主站设置 ESAM接口->安全模式选择->0，该值控制是否检查mac。
// 方法3：增加一个电表esam卡，需要更改硬件。
static void PlcTaskTx(const char *tx_buf, int tx_size)
{
	printf_hex_ex("tx: ", "\r\n", tx_buf, tx_size, "");
	return;
}
cp_t P2ProcessInput(P2Process *m, const char *rx_buf, int rx_len, char *tx_buf, int tx_buf_size)
{
	dve(P2ProcessValid(m));
	P2Link * const link = &m->link;

	dvb(P2_LEAD_NUM < tx_buf_size);
	/*
	*tx_size = 0;
	
	// 留出前导符的位置
	m_tx_info.tx_buf = tx_buf + P2_LEAD_NUM;
	m_tx_info.tx_buf_size = tx_buf_size - P2_LEAD_NUM;
	m_tx_info.tx_size = 0;
	*/

	// 调试接收过程
	P2LinkConfigPrintRxError(link, P2LinkPrintRxErrorNormal);
	while (rx_len > 0)
	{
		const int room = P2LinkRoom(link);
		const int copy_len = std::min(rx_len, room);
		if (copy_len <= 0)
		{
			// 有异常，可以考虑清空缓冲区
			dvb(false);
			//return 0;
		}

		memcpy(P2LinkRxPos(link), rx_buf, copy_len);
		ifer(P2LinkAppend(link, copy_len));
		// 是否有合法帧在缓冲区？
		if (P2LinkUpdate(link))
		{
			// 填写发送信息
			// 留出前导符的位置
			m_tx_info.tx_buf = tx_buf + P2_LEAD_NUM;
			m_tx_info.tx_buf_size = tx_buf_size - P2_LEAD_NUM;
			m_tx_info.tx_size = 0;

			const cp_t cp = Process(m);
			if (0 != cp)
			{
				char cp_buf[CPSTR_SIZE];
				qos_printf("Hplc process frame FAILED. at %s\r\n", CpStr(cp_buf, cp));
				printf_hex_ex("the failed rx: ", "\r\n", rx_buf, rx_len, "");
				// 这里失败，会导致main()返回负值，从而知道解析错误
				return cp;
			}

			// 填充前导符，发送
			ifbr(m_tx_info.tx_size <= m_tx_info.tx_buf_size);
			if (0 == cp)
			{
				// 填前导符0xfe
				P2FrameFillLead(tx_buf);
				const int tx_size = m_tx_info.tx_size + P2_LEAD_NUM;
				dvb(tx_size <= tx_buf_size);
				// 因为有可能一次收到多帧，所以，只能在这里发送。无法返回，然后让
				// 发送
				PlcTaskTx(tx_buf, tx_size);
			}

			ifer(P2LinkDiscardValidFrame(link));
		}
		rx_buf += copy_len;
		rx_len -= copy_len;
		dvb(rx_len >= 0);
	}

	return 0;
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
cp_t P2ProcessTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ProcessTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

