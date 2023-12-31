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

20230526-----------------------------------------------------------------------
huanglin 创建.
用来描述dlt698_45中的Server-APDU choice
*/

// var
#include "p2_connect_response.h"
#include "p2_release_response.h"
#include "p2_release_notification.h"
#include "p2_get_response.h"
#include "p2_set_response.h"
#include "p2_action_response.h"
#include "p2_report_notification.h"
#include "p2_proxy_response_choice.h"
#include "p2_compact_get_response.h"
#include "p2_compact_set_response.h"
#include "p2_compact_proxy_response.h"


#include "p2_error_response.h"

#include "p2_server_apdu_choice.h"
#include "p2_server_apdu_choice.xcp.h"


#define TEST_EN				(0)
#define kChoiceNum			(kP2ServerApduChoiceNum)

// {{{ choice
// 必须按大小顺序排
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kP2ServerApduChoiceConnectResponse, "connect_response" },	// 建立应用连接响应 [130] CONNECT-Response，
	{ kP2ServerApduChoiceReleaseResponse, "release_response" },	// 断开应用连接响应 [131] RELEASE-Response，
	{ kP2ServerApduChoiceReleaseNotification, "release_notification" },	// 断开应用连接通知 [132] RELEASE-Notification，
	{ kP2ServerApduChoiceGetResponse, "get_response" },	// 读取响应 [133] GET-Response，
	{ kP2ServerApduChoiceSetResponse, "set_response" },	// 设置响应 [134] SET-Response，
	{ kP2ServerApduChoiceActionResponse, "action_response" },	// 操作响应 [135] ACTION-Response，
	{ kP2ServerApduChoiceReportNotification, "report_notification" },	// 上报通知 [136] REPORT-Notification，
	{ kP2ServerApduChoiceProxyResponse, "proxy_response" },	// 代理响应 [137] PROXY-Response，
	{ kP2ServerApduChoiceCompactGetResponse, "compact_get_response" },	// 紧凑读取响应 [197] COMPACT-GET-Response，
	{ kP2ServerApduChoiceCompactSetResponse, "compact_set_response" },	// 紧凑设置响应 [198] COMPACT-SET-Response，
	{ kP2ServerApduChoiceCompactProxyResponse, "compact_proxy_response" },	// 紧凑代理响应 [201] COMPACT-PROXY-Response，
	{ kP2ServerApduChoiceErrorResponse, "error_response" },	// 异常响应 [238] ERROR-Response
};
int P2ServerApduChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2ServerApduChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2ServerApduChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2ServerApduChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
const char *P2ServerApduChoiceStr(uint8_t choice)
{
	return P2ChoiceStr(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2ConnectResponsePcut kP2ConnectResponsePcutDefVar = kP2ConnectResponsePcutDef;
static const P2ReleaseResponsePcut kP2ReleaseResponsePcutDefVar = kP2ReleaseResponsePcutDef;
static const P2ReleaseNotificationPcut kP2ReleaseNotificationPcutDefVar = kP2ReleaseNotificationPcutDef;
static const P2GetResponsePcut kP2GetResponsePcutDefVar = kP2GetResponsePcutDef;
static const P2SetResponsePcut kP2SetResponsePcutDefVar = kP2SetResponsePcutDef;
static const P2ActionResponsePcut kP2ActionResponsePcutDefVar = kP2ActionResponsePcutDef;
static const P2ReportNotificationPcut kP2ReportNotificationPcutDefVar = kP2ReportNotificationPcutDef;
static const P2ProxyResponseChoicePcut kP2ProxyResponseChoicePcutDefVar = kP2ProxyResponseChoicePcutDef;
static const P2CompactGetResponsePcut kP2CompactGetResponsePcutDefVar = kP2CompactGetResponsePcutDef;
static const P2CompactSetResponsePcut kP2CompactSetResponsePcutDefVar = kP2CompactSetResponsePcutDef;


static const P2ErrorResponsePcut kP2ErrorResponsePcutDefVar = kP2ErrorResponsePcutDef;

static const PcutFactoryInfo kVarFactoryInfoList[kChoiceNum] = {
	// name		size	init	derive_open		derive_close
	{ kP2ConnectResponseName, sizeof(P2ConnectResponsePcut), &kP2ConnectResponsePcutDefVar, P2ConnectResponsePcutOpenBase, P2ConnectResponsePcutCloseBase },	// 建立应用连接响应 [130] CONNECT-Response，
	{ kP2ReleaseResponseName, sizeof(P2ReleaseResponsePcut), &kP2ReleaseResponsePcutDefVar, P2ReleaseResponsePcutOpenBase, P2ReleaseResponsePcutCloseBase },	// 断开应用连接响应 [131] RELEASE-Response，
	{ kP2ReleaseNotificationName, sizeof(P2ReleaseNotificationPcut), &kP2ReleaseNotificationPcutDefVar, P2ReleaseNotificationPcutOpenBase, P2ReleaseNotificationPcutCloseBase },	// 断开应用连接通知 [132] RELEASE-Notification，
	{ kP2GetResponseName, sizeof(P2GetResponsePcut), &kP2GetResponsePcutDefVar, P2GetResponsePcutOpenBase, P2GetResponsePcutCloseBase },	// 读取响应 [133] GET-Response，
	{ kP2SetResponseName, sizeof(P2SetResponsePcut), &kP2SetResponsePcutDefVar, P2SetResponsePcutOpenBase, P2SetResponsePcutCloseBase },	// 设置响应 [134] SET-Response，
	{ kP2ActionResponseName, sizeof(P2ActionResponsePcut), &kP2ActionResponsePcutDefVar, P2ActionResponsePcutOpenBase, P2ActionResponsePcutCloseBase },	// 操作响应 [135] ACTION-Response，
	{ kP2ReportNotificationName, sizeof(P2ReportNotificationPcut), &kP2ReportNotificationPcutDefVar, P2ReportNotificationPcutOpenBase, P2ReportNotificationPcutCloseBase },	// 上报通知 [136] REPORT-Notification，
	{ kP2ProxyResponseName, sizeof(P2ProxyResponseChoicePcut), &kP2ProxyResponseChoicePcutDefVar, P2ProxyResponseChoicePcutOpenBase, P2ProxyResponseChoicePcutCloseBase },	//  代理响应 [137] PROXY-Response，
	{ kP2CompactGetResponseName, sizeof(P2CompactGetResponsePcut), &kP2CompactGetResponsePcutDefVar, P2CompactGetResponsePcutOpenBase, P2CompactGetResponsePcutCloseBase },	// 紧凑读取响应 [197] COMPACT-GET-Response，
	{ kP2CompactSetResponseName, sizeof(P2CompactSetResponsePcut), &kP2CompactSetResponsePcutDefVar, P2CompactSetResponsePcutOpenBase, P2CompactSetResponsePcutCloseBase },	// 紧凑设置响应 [198] COMPACT-SET-Response，
	kPcutFactoryInfoDef("COMPACT-PROXY-Response"),	// 紧凑代理响应 [201] COMPACT-PROXY-Response，
	{ kP2ErrorResponseName, sizeof(P2ErrorResponsePcut), &kP2ErrorResponsePcutDefVar, P2ErrorResponsePcutOpenBase, P2ErrorResponsePcutCloseBase },	// 异常响应 [238] ERROR-Response
	// kPcutFactoryInfoDef("example"),	//
};
//}}}


//{{{ cut
cp_t P2ServerApduChoicePcutOpen(P2ServerApduChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2ServerApduChoiceNameChoice, kChoiceList, kChoiceNum, kVarFactoryInfoList);
}
cp_t P2ServerApduChoicePcutClose(P2ServerApduChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2ServerApduChoicePcutValid(const P2ServerApduChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}
//}}}


//{{{ fill
cp_t P2ServerApduChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2ServerApduChoiceNameChoice, PfillItemOffsetFix, 0, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2ServerApduChoiceNameVar, PfillItemOffsetFix, PfillItemProcessBySub, 1, sub_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVar), &kFiVar));

	return 0;
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
cp_t P2ServerApduChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ServerApduChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

