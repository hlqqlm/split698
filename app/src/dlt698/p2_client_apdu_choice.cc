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
DLT698_45 client apdu choice 变化部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的Client-APDU choice
*/

#include "qos/qcp.h"
#include "qos/qtest.h"

// var
#include "p2_connect_request.h"
#include "p2_release_request.h"
#include "p2_get_request.h"
#include "p2_set_request_choice.h"
#include "p2_action_request_choice.h"
#include "p2_report_response_choice.h"
#include "p2_proxy_request_choice.h"
#include "p2_compact_get_request.h"
#include "p2_compact_set_request.h"
#include "p2_compact_proxy_request.h"
#include "p2_error_response.h"

#include "p2_client_apdu_choice.h"
#include "p2_client_apdu_choice.xcp.h"


#define TEST_EN				(0)


// {{{ choice
// 必须按大小顺序排
#define kChoiceNum kP2ClientApduChoiceNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ClientApduChoiceConnectRequest, "connect_request" },	// 建立应用连接请求 [2] CONNECT-Request，
	{ kP2ClientApduChoiceReleaseRequest, "release_request" },	// 断开应用连接请求 [3] RELEASE-Request，
	{ kP2ClientApduChoiceGetRequest, "get_request" },	// 读取请求 [5] GET-Request，
	{ kP2ClientApduChoiceSetRequest, "set_request", },	// 设置请求 [6] SET-Request，
	{ kP2ClientApduChoiceActionRequest, "action_request" },	// 操作请求 [7] ACTION-Request，
	{ kP2ClientApduChoiceReportResponse, "report_response" },	// 上报应答 [8] REPORT-Response，
	{ kP2ClientApduChoiceProxyRequest, "proxy_request" },	// 代理请求 [9] PROXY-Request，
	{ kP2ClientApduChoiceCompactGetRequest,	"compact_get_request" }, // 紧凑读取请求 [69] COMPACT-GET-Request，
	{ kP2ClientApduChoiceCompactSetRequest,	"compact_set_request" }, // 紧凑设置请求 [70] COMPACT-SET-Request，
	{ kP2ClientApduChoiceCompactProxyRequest, "compact_proxy_request" },	// 紧凑代理请求 [73] COMPACT-PROXY-Request，
	{ kP2ClientApduChoiceErrorResponse, "error_response" }	// 异常响应 [110] ERROR-Response
};
int P2ClientApduChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ClientApduChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ClientApduChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ClientApduChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ClientApduChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2ConnectRequestPcut kP2ConnectRequestPcutDefVar = kP2ConnectRequestPcutDef;
static const P2ReleaseRequestPcut kP2ReleaseRequestPcutDefVar = kP2ReleaseRequestPcutDef;
static const P2GetRequestPcut kP2GetRequestPcutDefVar = kP2GetRequestPcutDef;
static const P2SetRequestChoicePcut kP2SetRequestChoicePcutDefVar = kP2SetRequestChoicePcutDef;
static const P2ActionRequestChoicePcut kP2ActionRequestChoicePcutDefVar = kP2ActionRequestChoicePcutDef;
static const P2ReportResponseChoicePcut kP2ReportResponseChoicePcutDefVar = kP2ReportResponseChoicePcutDef;
static const P2ProxyRequestChoicePcut kP2ProxyRequestChoicePcutDefVar = kP2ProxyRequestChoicePcutDef;
static const P2CompactGetRequestPcut kP2CompactGetRequestPcutDefVar = kP2CompactGetRequestPcutDef;
static const P2CompactSetRequestPcut kP2CompactSetRequestPcutDefVar = kP2CompactSetRequestPcutDef;
static const P2CompactProxyRequestPcut kP2CompactProxyRequestPcutDefVar = kP2CompactProxyRequestPcutDef;
static const P2ErrorResponsePcut kP2ErrorResponsePcutDefVar = kP2ErrorResponsePcutDef;

static const PcutFactoryInfo kVarFactoryInfoList[kP2ClientApduChoiceNum] = {
	// name		size	init	derive_open		derive_close

	{ kP2ConnectRequestName, sizeof(P2ConnectRequestPcut), &kP2ConnectRequestPcutDefVar, P2ConnectRequestPcutOpenBase, P2ConnectRequestPcutCloseBase },	 // 建立应用连接请求 [2] CONNECT-Request，
	{ kP2ReleaseRequestName, sizeof(P2ReleaseRequestPcut), &kP2ReleaseRequestPcutDefVar, P2ReleaseRequestPcutOpenBase, P2ReleaseRequestPcutCloseBase },	// 断开应用连接请求 [3] RELEASE-Request，
	{ kP2GetRequestName, sizeof(P2GetRequestPcut), &kP2GetRequestPcutDefVar, P2GetRequestPcutOpenBase, P2GetRequestPcutCloseBase },	// 读取请求 [5] GET-Request，
	{ kP2SetRequestName, sizeof(P2SetRequestChoicePcut), &kP2SetRequestChoicePcutDefVar, P2SetRequestChoicePcutOpenBase, P2SetRequestChoicePcutCloseBase },	// 设置请求 [6] SET-Request，
	{ kP2ActionRequestName, sizeof(P2ActionRequestChoicePcut), &kP2ActionRequestChoicePcutDefVar, P2ActionRequestChoicePcutOpenBase, P2ActionRequestChoicePcutCloseBase },	// 操作请求 [7] ACTION-Request，
	{ kP2ReportResponseName, sizeof(P2ReportResponseChoicePcut), &kP2ReportResponseChoicePcutDefVar, P2ReportResponseChoicePcutOpenBase, P2ReportResponseChoicePcutCloseBase },	// 上报应答 [8] REPORT-Response，
	{ kP2ProxyRequestName, sizeof(P2ProxyRequestChoicePcut), &kP2ProxyRequestChoicePcutDefVar, P2ProxyRequestChoicePcutOpenBase, P2ProxyRequestChoicePcutCloseBase },	// 代理请求 [9] PROXY-Request，
	{ kP2CompactGetRequestName, sizeof(P2CompactGetRequestPcut), &kP2CompactGetRequestPcutDefVar, P2CompactGetRequestPcutOpenBase, P2CompactGetRequestPcutCloseBase },	// 紧凑读取请求 [69] COMPACT-GET-Request，
	{ kP2CompactSetRequestName, sizeof(P2CompactSetRequestPcut), &kP2CompactSetRequestPcutDefVar, P2CompactSetRequestPcutOpenBase, P2CompactSetRequestPcutCloseBase },	// 紧凑设置请求 [70] COMPACT-SET-Request，
	{ kP2CompactProxyRequestName, sizeof(P2CompactProxyRequestPcut), &kP2CompactProxyRequestPcutDefVar, P2CompactProxyRequestPcutOpenBase, P2CompactProxyRequestPcutCloseBase },	// 紧凑代理请求 [73] COMPACT-PROXY-Request，
	{ kP2ErrorResponseName, sizeof(P2ErrorResponsePcut), &kP2ErrorResponsePcutDefVar, P2ErrorResponsePcutOpenBase, P2ErrorResponsePcutCloseBase },	// 异常响应 [110] ERROR-Response
	// kPcutFactoryInfoDef("example"),		// 
};
//}}}


//{{{ pcut
cp_t P2ClientApduChoicePcutOpen(P2ClientApduChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, P2ClientApduChoiceName, kChoiceList, kP2ClientApduChoiceNum, kVarFactoryInfoList);
}
cp_t P2ClientApduChoicePcutClose(P2ClientApduChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ClientApduChoicePcutValid(const P2ClientApduChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestQpack(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ClientApduChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ClientApduChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

