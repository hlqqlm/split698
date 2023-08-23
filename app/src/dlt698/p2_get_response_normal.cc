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

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的GetResponseNormal
*/

// for test
#include "qos/qstr.h"
#include "p2_base.h"
#include "p2_octet_string.h"
#include "p2_choice.h"
#include "p2_client_apdu.h"
#include "p2_client_apdu_choice.h"
#include "p2_server_apdu_choice.h"
#include "p2_server_apdu.h"
#include "p2_get_request_normal.h"
#include "p2_get_response.h"

#include "p2_get_response_normal.h"
#include "p2_get_response_normal.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2GetResponseNormalCutNum)

// {{{ piid_acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2GetResponseNormalPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2GetResponseNormalPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ a_result_normal
#define LenAResultNormal		PcutItemLenBySub
static int OffsetAResultNormal(Pcut *cut, int ix, const char *whole) { return kP2GetResponseNormalAResultNormalOffset; }
#define ValidAResultNormal	PcutItemValidBySub
//}}}
 

//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2GetResponseNormalNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2GetResponseNormalNameAResultNormal, LenAResultNormal, OffsetAResultNormal, ValidAResultNormal, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetResponseNormalCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetResponseNormalCutIxAResultNormal]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2GetResponseNormalPcutOpen(P2GetResponseNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2AResultNormalPcutOpen(&m->a_result_normal_cut));
	PcutSubSet(&m->base, kP2GetResponseNormalCutIxAResultNormal, &m->a_result_normal_cut.base, kP2GetResponseNormalNameAResultNormal);
	return 0;
}
cp_t P2GetResponseNormalPcutClose(P2GetResponseNormalPcut *m)
{
	dve(P2GetResponseNormalPcutValid(m));

	PcutSubSet(&m->base, kP2GetResponseNormalCutIxAResultNormal, NULL, NULL);
	ifer(P2AResultNormalPcutClose(&m->a_result_normal_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2GetResponseNormalPcutValid(const P2GetResponseNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2AResultNormalPcutValid(&m->a_result_normal_cut));
	return 0;
}


cp_t P2GetResponseNormalPcutOpenBase(Pcut *base)
{
	P2GetResponseNormalPcut *m = (P2GetResponseNormalPcut*)base;
	return P2GetResponseNormalPcutOpen(m);
}
cp_t P2GetResponseNormalPcutCloseBase(Pcut *base)
{
	P2GetResponseNormalPcut *m = (P2GetResponseNormalPcut*)base;
	return P2GetResponseNormalPcutClose(m);
}
//}}}


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
	PFILL_ITEM(kP2GetResponseNormalNamePiidAcd, PfillItemOffsetFix, FillItemProcessPiidAcd, kP2GetResponseNormalPiidAcdOffset, NULL),	\
	kP2PiidAcdDef			\
}


typedef struct 
{
	PfillItem base;
} FillItemAResultNormal;
#define kFillItemAResultNormalDef(_sub) {			\
	PFILL_ITEM(kP2GetResponseNormalNameAResultNormal, PfillItemOffsetFix, PfillItemProcessBySub, kP2GetResponseNormalAResultNormalOffset, (_sub)),	\
}



cp_t P2GetResponseNormalFillInit(Pfill *m, const P2PiidAcd *piid_acd, Pfill *fill_a_result_normal)
{
	FillItemPiidAcd fi_piidacd = kFillItemPiidAcdDef;
	fi_piidacd.piid_acd = *piid_acd;
	ifbr(NULL != PfillItemFactory(m, sizeof(fi_piidacd), &fi_piidacd));

	const FillItemAResultNormal kFi = kFillItemAResultNormalDef(fill_a_result_normal);
	// const PfillItem kFi = PFILL_ITEM(kP2AResultNormalNameGetResult, PfillItemOffsetFix, PfillItemProcessBySub, kP2GetResponseNormalAResultNormalOffset, sub_a_result_normal);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFi), &kFi));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0

static cp_t TestPfill(void)
{
	// 《698面向对象的用电信息数据交换协议（20210910）_有标签.pdf》附录H3有oad40010200的应答报文
	// 主站问地址，报文示例：
	// => 68 17 00 43 45 AA AA AA AA AA AA 00 5B 4F 05 01 25 40 01 02 00 00 0A 92 16
	// <= 68 21 00 C3 05 33 09 20 39 00 00 00 4A 55 85 01 25 40 01 02 00 01 09 06 00 00 39 20 09 33 00 00 B2 D8 16
	
	const char apdu_mem[] = "\x05\x01\x01\x40\x01\x02\x00\x00";
	P2ClientApduPcut client_apdu_pcut = kP2ClientApduPcutDef;
	ifer(P2ClientApduPcutOpen(&client_apdu_pcut));

	ifbr(-1 == PcutInvalidIx(&client_apdu_pcut.base, apdu_mem));
	
	P2GetRequestNormalValue grn_value; 
	ifer(P2GetRequestNormalPcutValue(&grn_value, apdu_mem + 2));

	const char addr_mem[] = "\x12\x34\x56\x78\x90\x19";
	const int addr_size = sizeof(addr_mem) - 1;

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

	Pfill fill_get_response_normal = kPfillDef;
	ifer(PfillOpen(&fill_get_response_normal));

	P2Piid piid_split = kP2PiddDef;
	P2PiidCut(&piid_split, grn_value.piid);
	const P2PiidAcd piid_acd_value = {piid_split.service_priority, 0, piid_split.service_code};
	ifer(P2GetResponseNormalFillInit(&fill_get_response_normal, &piid_acd_value, &fill_a_result_normal));

	Pfill fill_get_response = kPfillDef;
	ifer(PfillOpen(&fill_get_response));
	ifer(P2GetResponseFillInit(&fill_get_response, kP2GetResponseChoiceGetResponseNormal, &fill_get_response_normal));

	// fill_server_apdu就是Lud/Apdu
	Pfill fill_server_apdu = kPfillDef;
	ifer(PfillOpen(&fill_server_apdu));
	ifer(P2ServerApduFillInit(&fill_server_apdu, kP2ServerApduChoiceGetResponse, &fill_get_response, NULL, NULL));

	if (TEST_PRINT_FILL_EN)
		PfillSetPrintFillWithSubs(&fill_server_apdu, PfillPrintFillNormal);

	char mem[20] = { 0 };
	const int mem_size = sizeof(mem);
	ifer(PfillDo(&fill_server_apdu, 0, mem, mem_size));
	const int fill_size = PfillIxLen(&fill_server_apdu, kPfillIxAll);

	if (TEST_PRINT_FILL_EN)
		printf_hex_ex("mem: ", "\r\n", mem, fill_size, "");

	const char expect[] = "\x85\x01\x01\x40\x01\x02\x00\x01\x09\x06\x12\x34\x56\x78\x90\x19\x00\x00";
	// 85 01 01 01 02 00 00 01 09 06 12 34 56 78 90 19 00 00
	const int expect_size = sizeof(expect) - 1;
	ifbr(expect_size == fill_size);
	ifbr(0 == memcmp(mem, expect, expect_size));

	ifer(PfillClose(&fill_server_apdu));
	ifer(PfillClose(&fill_get_response));
	ifer(PfillClose(&fill_get_response_normal));
	ifer(PfillClose(&fill_a_result_normal));
	ifer(PfillClose(&fill_get_result));
	ifer(PfillClose(&fill_octet_string));
	ifer(P2ClientApduPcutClose(&client_apdu_pcut));

	return 0;
}

static const QtestItem kTestItem[] = {
	TestPfill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2GetResponseNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetResponseNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

