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

用来描述dlt698_45中的ConnectResponse
*/
#include <string.h>

#include "p2_connect_response.h"
#include "p2_connect_response.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ConnectResponseCutNum)

 
// {{{ piid-acd
static int LenPiidAcd(Pcut *part, int ix, const char *whole) { return kP2ConnectResponsePiidAcdSize; }
static int OffsetPiidAcd(Pcut *part, int ix, const char *whole) { return kP2ConnectResponsePiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ factory_version
#define LenFactoryVersion		PcutItemLenBySub	
static int OffsetFactoryVersion(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFactoryVersionOffset; }
#define ValidFactoryVersion		PcutItemValidBySub
//}}}


// {{{ protocol_version
static int LenProtocolVersion(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseProtocolVersionSize; }
static int OffsetProtocolVersion(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseProtocolVersionOffset; }
static cp_t ValidProtocolVersion(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ protocol_conformance
// todo: 用sub对象来解析protocol_conformance
static int LenProtocolConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseProtocolConformanceSize; }
static int OffsetProtocolConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseProtocolConformanceOffset; }
static cp_t ValidProtocolConformance(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ function_conformance
// todo: 用sub对象来解析function_conformance
static int LenFunctionConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFunctionConformanceSize; }
static int OffsetFunctionConformance(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFunctionConformanceOffset; }
static cp_t ValidFunctionConformance(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ server_tx_size_max
static int LenServerTxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseServerTxSizeMaxSize; }
static int OffsetServerTxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseServerTxSizeMaxOffset; }
static cp_t ValidServerTxSizeMax (Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ server_rx_size_max
static int LenServerRxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseServerRxSizeMaxSize; }
static int OffsetServerRxSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseServerRxSizeMaxOffset; }
static cp_t ValidServerRxSizeMax (Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ frame_num_max
static int LenFrameNumMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFrameNumMaxSize; }
static int OffsetFrameNumMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFrameNumMaxOffset; }
static cp_t ValidFrameNumMax(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ frame_size_max
static int LenFrameSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFrameSizeMaxSize; }
static int OffsetFrameSizeMax(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseFrameSizeMaxOffset; }
static cp_t ValidFrameSizeMax(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ connection_timeout
static int LenConnectionTimeout(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseConnectionTimeoutSize; }
static int OffsetConnectionTimeout(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseConnectionTimeoutOffset; }
static cp_t ValidConnectionTimeout(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ connect_response_info
#define LenConnectResponseInfo			PcutItemLenBySub	
static int OffsetConnectResponseInfo(Pcut *part, int ix, const char *whole) { return kP2ConnectResponseConnectResponseInfoOffset; }
#define ValidConnectResponseInfo		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kThisCutNum] = {
	// name len offset valid explain
	{ "piid-acd", LenPiidAcd, OffsetPiidAcd, ValidPiidAcd }, 
	{ "factory_version", LenFactoryVersion, OffsetFactoryVersion, ValidFactoryVersion }, 
	{ "protocol_version", LenProtocolVersion, OffsetProtocolVersion, ValidProtocolVersion }, 
	{ "protocol_conformance", LenProtocolConformance, OffsetProtocolConformance, ValidProtocolConformance },		// kP2ConnectResponseCutIxProtocolConformance,
	{ "function_conformance", LenFunctionConformance, OffsetFunctionConformance, ValidFunctionConformance },		// kP2ConnectResponseCutIxFunctionConformance,
	{ "server_tx_size_max", LenServerTxSizeMax, OffsetServerTxSizeMax, ValidServerTxSizeMax },		// kP2ConnectResponseCutIxServerTxSizeMax,
	{ "server_rx_size_max", LenServerRxSizeMax, OffsetServerRxSizeMax, ValidServerRxSizeMax },		// kP2ConnectResponseCutIxServerRxSizeMax,
	{ "frame_num_max", LenFrameNumMax, OffsetFrameNumMax, ValidFrameNumMax },		
	{ "frame_size_max", LenFrameSizeMax, OffsetFrameSizeMax, ValidFrameSizeMax },		
	{ "connection_timeout", LenConnectionTimeout, OffsetConnectionTimeout, ValidConnectionTimeout },
	{ "connect_response_info", LenConnectResponseInfo, OffsetConnectResponseInfo, ValidConnectResponseInfo },
};
	

static const PcutItem kPartItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxFactoryVersion]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxProtocolVersion]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxProtocolConformance]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxFunctionConformance]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxServerTxSizeMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxServerRxSizeMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxFrameNumMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxFrameSizeMax]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxConnectionTimeout]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2ConnectResponseCutIxConnectResponseInfo]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}



cp_t P2ConnectResponsePcutOpen(P2ConnectResponsePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2FactoryVersionPcutOpen(&m->factory_version_cut));
	PcutSubSet(&m->base, kP2ConnectResponseCutIxFactoryVersion, &m->factory_version_cut.base, NULL);

	ifer(P2ConnectResponseInfoPcutOpen(&m->connect_response_info));
	PcutSubSet(&m->base, kP2ConnectResponseCutIxConnectResponseInfo, &m->connect_response_info.base, NULL);
	return 0;
}
cp_t P2ConnectResponsePcutClose(P2ConnectResponsePcut *m)
{
	dve(P2ConnectResponsePcutValid(m));

	PcutSubSet(&m->base, kP2ConnectResponseCutIxFactoryVersion, NULL, NULL);
	ifer(P2FactoryVersionPcutClose(&m->factory_version_cut));

	PcutSubSet(&m->base, kP2ConnectResponseCutIxConnectResponseInfo, NULL, NULL);
	ifer(P2ConnectResponseInfoPcutClose(&m->connect_response_info));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ConnectResponsePcutValid(const P2ConnectResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2FactoryVersionPcutValid(&m->factory_version_cut));
	ifer(P2ConnectResponseInfoPcutValid(&m->connect_response_info));
	return 0;
}


cp_t P2ConnectResponsePcutOpenBase(Pcut *base)
{
	P2ConnectResponsePcut *m = (P2ConnectResponsePcut*)base;
	return P2ConnectResponsePcutOpen(m);
}
cp_t P2ConnectResponsePcutCloseBase(Pcut *base)
{
	P2ConnectResponsePcut *m = (P2ConnectResponsePcut*)base;
	return P2ConnectResponsePcutClose(m);
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
cp_t P2ConnectResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ConnectResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


