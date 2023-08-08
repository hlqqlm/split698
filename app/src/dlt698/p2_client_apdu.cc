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

用来描述dlt698_45中的Client-APDU
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qmem.h"
#include "p2_client_apdu.h"
#include "p2_client_apdu.xcp.h"
//#define this_file_id	0xd657d393 // echo -n dlt698_45_client_apdu.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)
#define PRINT_FILL_IN_TEST_EN	(0)


// {{{ choice
uint8_t P2ClientApduChoice(const char *whole)
{
	return whole[kP2ClientApduChoiceOffset];
}

#define LenChoice	PcutItemLenBySub	
static int OffsetChoice(Pcut *part ,int ix, const char *whole) { return kP2ClientApduChoiceOffset; }
#define ValidChoice	PcutItemValidBySub
//}}}


// {{{ optional_timetag
#define LenOptionalTimetag		PcutItemLenBySub
#define OffsetOptionalTimetag	PcutItemOffsetDef
#define ValidOptionalTimetag	PcutItemValidBySub
// 用sub来填写data
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2ClientApduPartNum] = {
	// name len offset valid
	{ "client_apdu", LenChoice, OffsetChoice, ValidChoice },
	{ "optional_timetag", LenOptionalTimetag, OffsetOptionalTimetag, ValidOptionalTimetag },
};
	

static const PcutItem kPartItemsPattern[kP2ClientApduPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ClientApduPartIxChoice]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ClientApduPartIxOptionalTimetag]),
};
static void PcutItemsInit(PcutItem items[kP2ClientApduPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}


cp_t P2ClientApduPcutValid(const P2ClientApduPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2ClientApduChoicePcutValid(&m->choice_part));
	ifer(P2OptionalPcutValid(&m->optional_part));
	ifer(P2TimetagPcutValid(&m->timetag_part));
	return 0;
}
cp_t P2ClientApduPcutOpen(P2ClientApduPcut *m)
{
	ifer(P2ClientApduChoicePcutOpen(&m->choice_part));
	ifer(P2TimetagPcutOpen(&m->timetag_part));
	ifer(P2OptionalPcutOpen(&m->optional_part, &m->timetag_part.base, "timetag"));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2ClientApduPartNum));

	PcutSubSet(&m->base, kP2ClientApduPartIxChoice, &m->choice_part.choice.base, NULL);
	PcutSubSet(&m->base, kP2ClientApduPartIxOptionalTimetag, &m->optional_part.base, NULL);
	return 0;
}
cp_t P2ClientApduPcutClose(P2ClientApduPcut *m)
{
	dve(P2ClientApduPcutValid(m));

	PcutSubSet(&m->base, kP2ClientApduPartIxChoice, NULL, NULL);
	PcutSubSet(&m->base, kP2ClientApduPartIxOptionalTimetag, NULL, NULL);

	ifer(PcutClose(&m->base));
	ifer(P2ClientApduChoicePcutClose(&m->choice_part));
	ifer(P2OptionalPcutClose(&m->optional_part));
	ifer(P2TimetagPcutClose(&m->timetag_part));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
#include "dlt698_45_get_request.h"
#include "dlt698_45_get_request_normal.h"
static cp_t TestPcut(void)
{
	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x05\x01\x00\x40\x01\x02\x00\x00";
	const int whole_size = sizeof(whole) - 1;

	ifbr(0 == MallocCnt());
	P2ClientApduPcut caq = kP2ClientApduPcutDef;
	ifer(P2ClientApduPcutOpen(&caq));
	Pcut * const m = &caq.base;

	ifbr(7 == PcutIxLen(m, kP2ClientApduPartIxChoice, whole));
	ifbr(1 == PcutIxLen(m, kP2ClientApduPartIxOptionalTimetag, whole));
	ifbr(whole_size == PcutIxLen(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifbr(kP2ClientApduChoiceGetRequest == P2ClientApduChoice(whole));

	PcutItem *item_not_exist = NULL;
	int len_not_exist = 0;
	const char *mem_not_exist = PcutFindMem(m, "not_exist", whole, &item_not_exist, &len_not_exist);
	ifbr(NULL == mem_not_exist);
	ifbr(NULL == item_not_exist);

	// 通过名字，可以找到对应的mem，非常方便、直观
	PcutItem *item_get_request = NULL;
	int len_get_request = 0;
	const char *mem_get_request = PcutFindMem(m, "get_request", whole, &item_get_request, &len_get_request);
	ifbr((whole + 1) == mem_get_request);
	ifbr(6 == len_get_request);
	ifbr(kP2GetRequestChoiceGetRequestNormal == P2GetRequestChoice(mem_get_request));

	PcutItem *item_get_request_normal = NULL;
	int len_get_request_normal = 0;
	const char *mem_get_request_normal = PcutFindMem(m, "get_request_normal", whole, &item_get_request_normal, &len_get_request_normal);
	ifbr((whole + 2) == mem_get_request_normal);
	ifbr(5 == len_get_request_normal);

	const uint8_t piid = P2GetRequestNormalPiid(mem_get_request_normal);
	ifbr(0 == piid);
	const OadT oad = P2GetRequestNormalOad(mem_get_request_normal);
	//qos_printf("oad=%08x\r\n", oad);
	ifbr(0x40010200 == oad);

	if (PRINT_PART_IN_TEST_EN)
		PcutAllPrint(m, 0, whole);

	ifer(P2ClientApduPcutClose(&caq));
	ifbr(0 == MallocCnt());
	return 0;
}
static cp_t TestQpack(void)
{
	// 准备get_request
	P2GetRequestNormalQpack get_request_normal_pack = kP2GetRequestNormalQpackDef;
	ifer(P2GetRequestNormalQpackOpen(&get_request_normal_pack));
	{
	const P2GetRequestNormalValue value = {0, 0x40010200};
	// 下面两个方法等效，都是设置value的值
	ifer(QpackValueCopy(&get_request_normal_pack.base, &value, sizeof(value)));
	get_request_normal_pack.value = value;
	}

	P2GetRequestQpack get_request_pack = kP2GetRequestQpackDef;
	ifer(P2GetRequestQpackOpen(&get_request_pack));
	ifer(P2ChoiceQpackSetChoice(&get_request_pack.base, kP2GetRequestChoiceGetRequestNormal, &get_request_normal_pack.base));

	P2ClientApduQpack ca = kP2ClientApduQpackDef;
	P2ClientApduQpackOpen(&ca);
	Qpack * const pack = &ca.base;

	ifer(P2ChoiceQpackSetChoice(&ca.choice_pack.base, kP2ClientApduChoiceGetRequest, &get_request_pack.base.base));
	// optional timetag设置为0
	P2OptionalQpackSetVar(&ca.optional_pack, NULL);

	const char result[] = "\x05\x01\x00\x40\x01\x02\x00\x00";
	const int result_size = sizeof(result) - 1;
	char mem[10] = { 0 };

	// 打开注释，可以测试组帧过程的打印
	if (PRINT_FILL_IN_TEST_EN)
		QpackSetPrintFill(pack, QpackPrintFillNormal);

	ifer(QpackDo(pack, mem, sizeof(mem)));
	const int fill_size = QpackIxLen(pack, kQpackIxAll);
	// qos_printf("fill_size=%d result_size=%d\r\n", fill_size, result_size);
	ifbr(fill_size == result_size);
	ifbr(0 == memcmp(mem, result, result_size));

	ifer(P2ClientApduQpackClose(&ca));

	ifer(P2GetRequestQpackClose(&get_request_pack));
	ifer(P2GetRequestNormalQpackClose(&get_request_normal_pack));
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ClientApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ClientApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

