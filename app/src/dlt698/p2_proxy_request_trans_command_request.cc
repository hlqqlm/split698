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

20230814-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的 ProxyRequest TransCommandRequest
*/
#include "p2_proxy_request_trans_command_request.h"
#include "p2_proxy_request_trans_command_request.xcp.h"




#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ProxyRequestTransCommandRequestCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


//{{{ comdcb
#define LenComdcb		PcutItemLenBySub
#define OffsetComdcb	PcutItemOffsetDef
#define ValidComdcb		PcutItemValidBySub
//}}}


//{{{ rx_timeout_sec
uint16_t P2ProxyRequestTransCommandRequestRxTimeoutSecValue(const char *whole)
{
	const char *mem = whole + kP2ProxyRequestTransCommandRequestRxTimeoutSecOffset;
	return Dlt698Uint16(mem);
}

static int LenRxTimeoutSec(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestRxTimeoutSecSize; }
static int OffsetRxTimeoutSec(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestRxTimeoutSecOffset; }
static cp_t ValidRxTimeoutSec(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
static cp_t ExplainRxTimeoutSec(Pcut *cut, int ix, const char *whole) 
{
	const uint16_t value = P2ProxyRequestTransCommandRequestRxTimeoutSecValue(whole);
	qos_printf("%uD", value);
	return 0;
}
//}}}


//{{{ rx_byte_timeout_ms
uint16_t P2ProxyRequestTransCommandRequestRxByteTimeoutMsValue(const char *whole)
{
	const char *mem = whole + kP2ProxyRequestTransCommandRequestRxByteTimeoutMsOffset;
	return Dlt698Uint16(mem);
}

static int LenRxByteTimeoutMs(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestRxByteTimeoutMsSize; }
static int OffsetRxByteTimeoutMs(Pcut *cut, int ix, const char *whole) { return kP2ProxyRequestTransCommandRequestRxByteTimeoutMsOffset; }
static cp_t ValidRxByteTimeoutMs(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}

static cp_t ExplainRxByteTimeoutMs(Pcut *cut, int ix, const char *whole) 
{
	const uint16_t value = P2ProxyRequestTransCommandRequestRxByteTimeoutMsValue(whole);
	qos_printf("%uD", value);
	return 0;
}
//}}}


//{{{ cmd
#define LenCmd		PcutItemLenBySub
#define OffsetCmd	PcutItemOffsetDef
#define ValidCmd	PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ProxyRequestTransCommandRequestNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2ProxyRequestTransCommandRequestNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2ProxyRequestTransCommandRequestNameComdcb, LenComdcb, OffsetComdcb, ValidComdcb, NULL },
	{ kP2ProxyRequestTransCommandRequestNameRxTimeoutSec, LenRxTimeoutSec, OffsetRxTimeoutSec, ValidRxTimeoutSec, ExplainRxTimeoutSec },
	{ kP2ProxyRequestTransCommandRequestNameRxByteTimeoutMs, LenRxByteTimeoutMs, OffsetRxByteTimeoutMs, ValidRxByteTimeoutMs, ExplainRxByteTimeoutMs },
	{ kP2ProxyRequestTransCommandRequestNameCmd, LenCmd, OffsetCmd, ValidCmd, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyRequestTransCommandRequestCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyRequestTransCommandRequestCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyRequestTransCommandRequestCutIxComdcb]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyRequestTransCommandRequestCutIxRxTimeoutSec]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyRequestTransCommandRequestCutIxRxByteTimeoutMs]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ProxyRequestTransCommandRequestCutIxCmd]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ProxyRequestTransCommandRequestPcutOpen(P2ProxyRequestTransCommandRequestPcut *m)
{

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2ComdcbPcutOpen(&m->comdcb_cut));
	PcutSubSet(&m->base, kP2ProxyRequestTransCommandRequestCutIxComdcb, &m->comdcb_cut.base, kP2ProxyRequestTransCommandRequestNameComdcb);

	ifer(P2OctetStringPcutOpen(&m->cmd_cut));
	PcutSubSet(&m->base, kP2ProxyRequestTransCommandRequestCutIxCmd, &m->cmd_cut.base, kP2ProxyRequestTransCommandRequestNameCmd);
	return 0;
}
cp_t P2ProxyRequestTransCommandRequestPcutClose(P2ProxyRequestTransCommandRequestPcut *m)
{
	dve(P2ProxyRequestTransCommandRequestPcutValid(m));

	PcutSubSet(&m->base, kP2ProxyRequestTransCommandRequestCutIxCmd, NULL, NULL);
	ifer(P2OctetStringPcutClose(&m->cmd_cut));
	PcutSubSet(&m->base, kP2ProxyRequestTransCommandRequestCutIxComdcb, NULL, NULL);
	ifer(P2ComdcbPcutClose(&m->comdcb_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ProxyRequestTransCommandRequestPcutValid(const P2ProxyRequestTransCommandRequestPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2OctetStringPcutValid(&m->cmd_cut));
	ifer(P2ComdcbPcutValid(&m->comdcb_cut));
	return 0;
}


cp_t P2ProxyRequestTransCommandRequestPcutOpenBase(Pcut *base)
{
	P2ProxyRequestTransCommandRequestPcut *m = (P2ProxyRequestTransCommandRequestPcut*)base;
	return P2ProxyRequestTransCommandRequestPcutOpen(m);
}
cp_t P2ProxyRequestTransCommandRequestPcutCloseBase(Pcut *base)
{
	P2ProxyRequestTransCommandRequestPcut *m = (P2ProxyRequestTransCommandRequestPcut*)base;
	return P2ProxyRequestTransCommandRequestPcutClose(m);
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
cp_t P2ProxyRequestTransCommandRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ProxyRequestTransCommandRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
