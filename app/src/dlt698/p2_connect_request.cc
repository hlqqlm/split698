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

用来描述dlt698_45中的ConnectRequest
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"

#include "p2_connect_request.h"
#include "p2_connect_request.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ConnectRequestCutNum)

 
// {{{ piid
static int LenPiid(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestPiidSize; }
static int OffsetPiid(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestPiidOffset; }
static cp_t ValidPiid(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}



uint8_t P2ConnectRequestPiid(const char *whole)
{
	return *(whole + kP2ConnectRequestPiidOffset);
}
//}}}


// {{{ app_protocol_version
static int LenAppProtocolVersion(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestAppProtocolVersionSize; }
static int OffsetAppProtocolVersion(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestAppProtocolVersionOffset; }
static cp_t ValidAppProtocolVersion(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}


uint16_t P2ConnectRequestAppProtocolVersion(const char *whole)
{
	const char *mem = whole + kP2ConnectRequestAppProtocolVersionOffset;
	const uint16_t version = Dlt698Uint16(mem);
	return version;
}

//}}}


// {{{ protocol_conformance
// todo: 用sub对象来解析protocol_conformance
static int LenProtocolConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestProtocolConformanceSize; }
static int OffsetProtocolConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestProtocolConformanceOffset; }
static cp_t ValidProtocolConformance(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ function_conformance
// todo: 用sub对象来解析function_conformance
static int LenFunctionConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestFunctionConformanceSize; }
static int OffsetFunctionConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestFunctionConformanceOffset; }
static cp_t ValidFunctionConformance(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ client_tx_size_max
static int LenClientTxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestClientTxSizeMaxSize; }
static int OffsetClientTxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestClientTxSizeMaxOffset; }
static cp_t ValidClientTxSizeMax (Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ client_rx_size_max
static int LenClientRxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestClientRxSizeMaxSize; }
static int OffsetClientRxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestClientRxSizeMaxOffset; }
static cp_t ValidClientRxSizeMax (Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ frame_num_max
static int LenFrameNumMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestFrameNumMaxSize; }
static int OffsetFrameNumMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestFrameNumMaxOffset; }
static cp_t ValidFrameNumMax(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ apdu_size_max
static int LenApduSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestApduSizeMaxSize; }
static int OffsetApduSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestApduSizeMaxOffset; }
static cp_t ValidApduSizeMax(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ connection_timeout
static int LenConnectionTimeout(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestConnectionTimeoutSize; }
static int OffsetConnectionTimeout(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestConnectionTimeoutOffset; }
static cp_t ValidConnectionTimeout(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ connect_mechanism_info
#define LenConnectMechanismInfo			PcutItemLenBySub	
static int OffsetConnectMechanismInfo(Pcut *part, int ix, const char *whole) { return kP2ConnectRequestConnectMechanismInfoOffset; }
#define ValidConnectMechanismInfo		PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kThisCutNum] = {
	// name len offset valid explain
	{ "piid", LenPiid, OffsetPiid, ValidPiid }, // kP2ConnectRequestCutIxPiid,
	{ "app_protocol_version", LenAppProtocolVersion, OffsetAppProtocolVersion, ValidAppProtocolVersion }, // kP2ConnectRequestCutIxAppProtocolVersion,
	{ "protocol_conformance", LenProtocolConformance, OffsetProtocolConformance, ValidProtocolConformance },		// kP2ConnectRequestCutIxProtocolConformance,
	{ "function_conformance", LenFunctionConformance, OffsetFunctionConformance, ValidFunctionConformance },		// kP2ConnectRequestCutIxFunctionConformance,
	{ "client_tx_size_max", LenClientTxSizeMax, OffsetClientTxSizeMax, ValidClientTxSizeMax },		// kP2ConnectRequestCutIxClientTxSizeMax,
	{ "client_rx_size_max", LenClientRxSizeMax, OffsetClientRxSizeMax, ValidClientRxSizeMax },		// kP2ConnectRequestCutIxClientRxSizeMax,
	{ "frame_num_max", LenFrameNumMax, OffsetFrameNumMax, ValidFrameNumMax },		// kP2ConnectRequestCutIxFrameNumMax,
	{ "apdu_size_max", LenApduSizeMax, OffsetApduSizeMax, ValidApduSizeMax },		// kP2ConnectRequestCutIxApduSizeMax,
	{ "connection_timeout", LenConnectionTimeout, OffsetConnectionTimeout, ValidConnectionTimeout },		// kP2ConnectRequestCutIxConnectionTimeout,
	{ "connect_mechanism_info", LenConnectMechanismInfo, OffsetConnectMechanismInfo, ValidConnectMechanismInfo },		// kP2ConnectRequestCutIxConnectMechanismInfo,
};
	

static const PcutItem kPartItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxAppProtocolVersion]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxProtocolConformance]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxFunctionConformance]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxClientTxSizeMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxClientRxSizeMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxFrameNumMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxApduSizeMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxConnectionTimeout]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectRequestCutIxConnectMechanismInfo]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}



cp_t P2ConnectRequestPcutOpen(P2ConnectRequestPcut *m)
{
	ifer(P2ConnectMechanismInfoChoicePcutOpen(&m->cmi));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	PcutSubSet(&m->base, kP2ConnectRequestCutIxConnectMechanismInfo, &m->cmi.choice.base, NULL);
	return 0;
}
cp_t P2ConnectRequestPcutClose(P2ConnectRequestPcut *m)
{
	dve(P2ConnectRequestPcutValid(m));

	PcutSubSet(&m->base, kP2ConnectRequestCutIxConnectMechanismInfo, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2ConnectMechanismInfoChoicePcutClose(&m->cmi));
	return 0;
}
cp_t P2ConnectRequestPcutValid(const P2ConnectRequestPcut *m)
{
	ifer(P2ConnectMechanismInfoChoicePcutValid(&m->cmi));
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2ConnectRequestPcutOpenBase(Pcut *base)
{
	P2ConnectRequestPcut *m = (P2ConnectRequestPcut*)base;
	return P2ConnectRequestPcutOpen(m);
}
cp_t P2ConnectRequestPcutCloseBase(Pcut *base)
{
	P2ConnectRequestPcut *m = (P2ConnectRequestPcut*)base;
	return P2ConnectRequestPcutClose(m);
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
cp_t P2ConnectRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ConnectRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


