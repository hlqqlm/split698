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
DLT698_45报文解析

用来描述dlt698_45中的 ProxyResponse TransCommandResponse
*/
#include "p2_proxy_response_trans_command_response.h"
#include "p2_proxy_response_trans_command_response.xcp.h"




#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ProxyResponseTransCommandResponseCutNum)



// {{{ piid
static int LenPiidAcd(Pcut *part, int ix, const char *whole) { return kP2ProxyResponseTransCommandResponsePiidAcdSize; }
static int OffsetPiidAcd(Pcut *part, int ix, const char *whole) { return kP2ProxyResponseTransCommandResponsePiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ oad
static int LenOad(Pcut *part, int ix, const char *whole) { return kP2ProxyResponseTransCommandResponseOadSize; }
static int OffsetOad(Pcut *part, int ix, const char *whole) { return kP2ProxyResponseTransCommandResponseOadOffset; }
static cp_t ValidOad(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


//{{{ trans_result
#define LenTransResult		PcutItemLenBySub
static int OffsetTransResult(Pcut *part, int ix, const char *whole) { return kP2ProxyResponseTransCommandResponseTransResultOffset; }
//#define OffsetTransResult	PcutItemOffsetDef
#define ValidTransResult		PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ProxyResponseTransCommandResponseNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2ProxyResponseTransCommandResponseNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2ProxyResponseTransCommandResponseNameTransResult, LenTransResult, OffsetTransResult, ValidTransResult, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyResponseTransCommandResponseCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyResponseTransCommandResponseCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyResponseTransCommandResponseCutIxTransResult]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ProxyResponseTransCommandResponsePcutOpen(P2ProxyResponseTransCommandResponsePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TransResultPcutOpen(&m->trans_result_cut));
	PcutSubSet(&m->base, kP2ProxyResponseTransCommandResponseCutIxTransResult, &m->trans_result_cut.choice.base, kP2ProxyResponseTransCommandResponseNameTransResult);
	return 0;
}
cp_t P2ProxyResponseTransCommandResponsePcutClose(P2ProxyResponseTransCommandResponsePcut *m)
{
	dve(P2ProxyResponseTransCommandResponsePcutValid(m));

	PcutSubSet(&m->base, kP2ProxyResponseTransCommandResponseCutIxTransResult, NULL, NULL);
	ifer(P2TransResultPcutClose(&m->trans_result_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ProxyResponseTransCommandResponsePcutValid(const P2ProxyResponseTransCommandResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TransResultPcutValid(&m->trans_result_cut));
	return 0;
}


cp_t P2ProxyResponseTransCommandResponsePcutOpenBase(Pcut *base)
{
	P2ProxyResponseTransCommandResponsePcut *m = (P2ProxyResponseTransCommandResponsePcut*)base;
	return P2ProxyResponseTransCommandResponsePcutOpen(m);
}
cp_t P2ProxyResponseTransCommandResponsePcutCloseBase(Pcut *base)
{
	P2ProxyResponseTransCommandResponsePcut *m = (P2ProxyResponseTransCommandResponsePcut*)base;
	return P2ProxyResponseTransCommandResponsePcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ProxyResponseTransCommandResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ProxyResponseTransCommandResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
