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
DLT698_45报文解析

用来描述dlt698_45中的GetResponseNormalList
*/
#include "qdlt698.h"

// for test
#include "qos/qstr.h"
#include "qos/qmem.h"
#include "protocol/pfill_repository.h"
#include "p2_base.h"
#include "p2_octet_string.h"
#include "p2_sequence_of_var_len.h"
#include "p2_choice.h"
#include "p2_client_apdu.h"
#include "p2_client_apdu_choice.h"
#include "p2_server_apdu_choice.h"
#include "p2_server_apdu.h"
#include "p2_get_request_normal_list.h"
#include "p2_get_response.h"

#include "p2_get_response_normal_list.h"
#include "p2_get_response_normal_list.xcp.h"
//#define this_file_id	0x140164e0 // echo -n dlt698_45_get_response_normal_list.c | rhash --simple -


#define TEST_EN						(1)
#define TEST_PRINT_FILL_EN			(0)


//{{{ fill
typedef struct 
{
	PfillItem base;
	P2PiidAcd piid_acd;
} FillItemPiidAcd;
static cp_t FillItemProcessPiidAcd(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemPiidAcd *derive = (FillItemPiidAcd*)fi;

	mem[offset] = P2PiidAcdFill(&derive->piid_acd);
	*fill_size = kP2PiidAcdSize;
	return 0;
}
#define kFillItemPiidAcdDef {			\
	PFILL_ITEM(kP2GetResponseNormalListPiidAcd, PfillItemOffsetFix, FillItemProcessPiidAcd, kP2GetResponseNormalListPiidAcdOffset, NULL),			\
	kP2PiidAcdDef,			\
}


typedef struct 
{
	PfillItem base;
} FillItemSequenceOfAResultNormal;
#define kFillItemSequenceOfAResultNormalDef(_sub) {			\
	PFILL_ITEM(kP2GetResponseNormalListSequenceOfAResultNormal, PfillItemOffsetFix, PfillItemProcessBySub, kP2GetResponseNormalListSequenceOfAResultNormalOffset, (_sub)),	\
}



cp_t P2GetResponseNormalListFillInit(Pfill *m, const P2PiidAcd *piid_acd, Pfill *fill_sequence_of_a_result_normal)
{
	FillItemPiidAcd fi_piidacd = kFillItemPiidAcdDef;
	fi_piidacd.piid_acd = *piid_acd;
	ifbr(NULL != PfillItemFactory(m, sizeof(fi_piidacd), &fi_piidacd));

	const FillItemSequenceOfAResultNormal kFi = kFillItemSequenceOfAResultNormalDef(fill_sequence_of_a_result_normal);
	// const PfillItem kFi = PFILL_ITEM(kP2AResultNormalNameGetResult, PfillItemOffsetFix, PfillItemProcessBySub, kP2GetResponseNormalAResultNormalOffset, sub_a_result_normal);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFi), &kFi));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t FillOadOctetString(Pfill *fill_data, Pfill *fill_get_result, const char *mem, int size)
{
	ifer(P2OctetStringFillInitByMem(fill_data, true, mem, size));
	ifer(P2GetResultFillInit(fill_get_result, kP2DarSuccess, fill_data));
	return 0;
}
// 应答这个oad，填充到fill_a_result_normal中
// 中间生成的Pfill对象，托管到fill_repository(fill仓库，负责Pfill的销毁)
// fill_repository_a_result_normal		专门存放a_result_normal类型的fill
// fill_repository_other				存放其它类型的fill
static cp_t FillAResultNormalOad(
		PfillRepository *fill_repository_a_result_normal
		, PfillRepository *fill_repository_other
		, OadT oad)
{
	// 逆序释放内存
	Pfill *fill_data = PfillRepositoryFactory(fill_repository_other, "data");
	ifbr(NULL != fill_data);
	Pfill *fill_get_result = PfillRepositoryFactory(fill_repository_other, "get_result");
	ifbr(NULL != fill_get_result);
	Pfill *fill_a_result_normal = PfillRepositoryFactory(fill_repository_a_result_normal, "a_result_normal");
	ifbr(NULL != fill_a_result_normal);

	switch (oad)
	{
		case 0x00100200:
			ifer(FillOadOctetString(fill_data, fill_get_result, "1", 1));
			break;
		case 0x20000200:
			ifer(FillOadOctetString(fill_data, fill_get_result, "23", 2));
			break;
		case 0x20040200:
			ifer(FillOadOctetString(fill_data, fill_get_result, "456", 3));
			break;
		default:
			return cph;
	}
	ifer(P2AResultNormalFillInit(fill_a_result_normal, oad, fill_get_result));
	return 0;
}
static cp_t TestPfill(void)
{
	// 00 03 00 10 02 00 20 00 02 00 20 04 02 00 00 
	// 3个oad
	const char grnl_mem[] = "\x05\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00";
	P2GetRequestNormalListValue grnl_value = kP2GetRequestNormalListValueDef;
	ifer(P2GetRequestNormalListPcutValue(&grnl_value, grnl_mem));

	//const char addr_mem[] = "\x12\x34\x56\x78\x90\x19";
	//const int addr_size = sizeof(addr_mem) - 1;

	/*
	Pfill fill_octet_string = kPfillDef;
	ifer(PfillOpen(&fill_octet_string));
	ifer(P2OctetStringFillInit(&fill_octet_string, true, addr_mem, addr_size));

	Pfill fill_get_result = kPfillDef;
	ifer(PfillOpen(&fill_get_result));
	ifer(P2GetResultFillInit(&fill_get_result, kP2DarSuccess, &fill_octet_string));

	Pfill fill_a_result_normal = kPfillDef;
	ifer(PfillOpen(&fill_a_result_normal));
//qos_printf("oad=%08x\r\n", grn_value.oad);
	ifer(P2AResultNormalFillInit(&fill_a_result_normal, grn_value.oad, &fill_get_result));
	*/

	// fill仓库，用来管理Pfill指针
	PfillRepository fill_repository_other = kPfillRepositoryDef;	// 用来调用fill的close和qfree
	ifer(PfillRepositoryOpen(&fill_repository_other));

	PfillRepository fill_repository_a_result_normal = kPfillRepositoryDef;	// a_result_normal fill列表，传入sequence of中
	ifer(PfillRepositoryOpen(&fill_repository_a_result_normal));
	// 生成n个fill_a_result_normal对象，托管给fill_repository
	for (int i = 0; i < grnl_value.oad.num; ++i)
	{
		const cp_t cp = FillAResultNormalOad(&fill_repository_a_result_normal, &fill_repository_other, grnl_value.oad.list[i]);
		// 出现错误，关闭所有对象，释放所有申请的内存
		if (0 != cp)
		{
			ifer(PfillRepositoryClose(&fill_repository_a_result_normal));
			ifer(PfillRepositoryClose(&fill_repository_other));
			return cp;
		}
	}

	Pfill fill_sequence_of_a_result_normal = kPfillDef;
	ifer(PfillOpen(&fill_sequence_of_a_result_normal));
	// QarraySetPrint(&fill_sequence_of_a_result_normal.array, QarrayPrintFull);
	//qos_printf("oad=%08x\r\n", grn_value.oad);
	ifer(P2SequenceOfVarLenFillInit(&fill_sequence_of_a_result_normal, &fill_repository_a_result_normal));


	Pfill fill_get_response_normal_list = kPfillDef;
	ifer(PfillOpen(&fill_get_response_normal_list));

	P2Piid piid_split = kP2PiidDef;
	P2PiidPart(&piid_split, grnl_value.piid);
	const P2PiidAcd piid_acd_value = {piid_split.service_priority, 0, piid_split.service_code};
	ifer(P2GetResponseNormalListFillInit(&fill_get_response_normal_list, &piid_acd_value, &fill_sequence_of_a_result_normal));

	Pfill fill_get_response = kPfillDef;
	ifer(PfillOpen(&fill_get_response));
	ifer(P2GetResponseFillInit(&fill_get_response, kP2GetResponseChoiceGetResponseNormalList, &fill_get_response_normal_list));

	// fill_server_apdu就是Lud/Apdu
	Pfill fill_server_apdu = kPfillDef;
	ifer(PfillOpen(&fill_server_apdu));
	ifer(P2ServerApduFillInit(&fill_server_apdu, kP2ServerApduChoiceGetResponse, &fill_get_response, NULL, NULL));

	if (TEST_PRINT_FILL_EN)
		PfillSetPrintFillWithSubs(&fill_server_apdu, PfillPrintFillNormal);

	char mem[100] = { 0 };
	const int mem_size = sizeof(mem);
	ifer(PfillDo(&fill_server_apdu, 0, mem, mem_size));
	const int fill_size = PfillIxLen(&fill_server_apdu, kPfillIxAll);

	if (TEST_PRINT_FILL_EN)
		printf_hex_ex("mem: ", "\r\n", mem, fill_size, "");

	const char expect[] = "\x85\x02\x05\x03\x00\x10\x02\x00\x01\x09\x01\x31\x20\x00\x02\x00\x01\x09\x02\x32\x33\x20\x04\x02\x00\x01\x09\x03\x34\x35\x36\x00\x00";
	//85 02 05 03 00 10 02 00 01 09 01 31 20 00 02 00 01 09 02 32 33 20 04 02 00 01 09 03 34 35 36 00 00 
	const int expect_size = sizeof(expect) - 1;
	ifbr(expect_size == fill_size);
	ifbr(0 == memcmp(mem, expect, expect_size));

	ifer(PfillClose(&fill_server_apdu));
	ifer(PfillClose(&fill_get_response));
	ifer(PfillClose(&fill_get_response_normal_list));
	ifer(PfillClose(&fill_sequence_of_a_result_normal));
	ifer(PfillRepositoryClose(&fill_repository_a_result_normal));
	ifer(PfillRepositoryClose(&fill_repository_other));

	return 0;
}

static const QtestItem kTestItem[] = {
	TestPfill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2GetResponseNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetResponseNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

