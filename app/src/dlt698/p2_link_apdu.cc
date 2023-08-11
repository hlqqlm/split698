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

用来描述dlt698_45中的Link-APDU
*/
#include <string.h>

#include "qos/qmem.h"
#include "p2_link_apdu.h"
#include "p2_link_apdu.xcp.h"
//#define this_file_id	0xd657d393 // echo -n dlt698_45_link_apdu.c | rhash --simple -


#define TEST_EN					(1)
#define PRINT_PART_IN_TEST_EN	(0)
#define PRINT_FILL_IN_TEST_EN	(0)


// {{{ choice
uint8_t P2LinkApduChoice(const char *whole)
{
	return whole[kP2LinkApduChoiceOffset];
}

#define LenChoice	PcutItemLenBySub	
static int OffsetChoice(Pcut *part ,int ix, const char *whole) { return kP2LinkApduChoiceOffset; }
#define ValidChoice	PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2LinkApduPartNum] = {
	// name len offset valid explain
	{ "link_apdu", LenChoice, OffsetChoice, ValidChoice, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2LinkApduPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2LinkApduPartIxChoice]),
};
static void PcutItemsInit(PcutItem items[kP2LinkApduPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}


cp_t P2LinkApduPcutValid(const P2LinkApduPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2LinkApduChoicePcutValid(&m->choice_part));
	return 0;
}
cp_t P2LinkApduPcutOpen(P2LinkApduPcut *m)
{
	ifer(P2LinkApduChoicePcutOpen(&m->choice_part));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2LinkApduPartNum));

	PcutSubSet(&m->base, kP2LinkApduPartIxChoice, &m->choice_part.choice.base, NULL);
	return 0;
}
cp_t P2LinkApduPcutClose(P2LinkApduPcut *m)
{
	dve(P2LinkApduPcutValid(m));

	PcutSubSet(&m->base, kP2LinkApduPartIxChoice, NULL, NULL);

	ifer(PcutClose(&m->base));
	ifer(P2LinkApduChoicePcutClose(&m->choice_part));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
#include "p2_link_request.h"


static cp_t TestPcut(void)
{
#if 0
	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x05\x01\x00\x40\x01\x02\x00\x00";
	const int whole_size = sizeof(whole) - 1;

	ifbr(0 == MallocCnt());
	P2LinkApduPcut caq = kP2LinkApduPcutDef;
	ifer(P2LinkApduPcutOpen(&caq));
	Pcut * const m = &caq.base;

	ifbr(7 == PcutIxLen(m, kP2LinkApduPartIxChoice, whole));
	ifbr(1 == PcutIxLen(m, kP2LinkApduPartIxOptionalTimetag, whole));
	ifbr(whole_size == PcutIxLen(m, kPcutIxAll, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifbr(kP2LinkApduChoiceGetRequest == P2LinkApduChoice(whole));

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

	ifer(P2LinkApduPcutClose(&caq));
	ifbr(0 == MallocCnt());
#endif
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2LinkApduTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2LinkApduTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

