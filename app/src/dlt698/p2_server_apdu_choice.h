#ifndef __P2_SERVER_APDU_CHOICE_H_ 
#define __P2_SERVER_APDU_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_choice.h"

// name
#define kP2ServerApduChoiceNameChoice	"server_apdu_choice"
#define kP2ServerApduChoiceNameVar		"server_apdu_var"

//{{{ choice
/*
Server-APDU∷=SEQUENCE
{
 应用层服务 CHOICE
 {
建立应用连接响应 [130] CONNECT-Response，
断开应用连接响应 [131] RELEASE-Response，
断开应用连接通知 [132] RELEASE-Notification，
读取响应 [133] GET-Response，
设置响应 [134] SET-Response，
操作响应 [135] ACTION-Response，
上报通知 [136] REPORT-Notification，
代理响应 [137] PROXY-Response，
紧凑读取响应 [197] COMPACT-GET-Response，
紧凑设置响应 [198] COMPACT-SET-Response，
紧凑代理响应 [201] COMPACT-PROXY-Response，
异常响应 [238] ERROR-Response
}，
 跟随上报信息域 FollowReport OPTIONAL，
时间标签域 TimeTag OPTIONAL
}
*/
typedef enum 
{
	kP2ServerApduChoiceConnectResponse = 130,	// 建立应用连接响应 [130] CONNECT-Response，
	kP2ServerApduChoiceReleaseResponse = 131,	// 断开应用连接响应 [131] RELEASE-Response，
	kP2ServerApduChoiceReleaseNotification = 132,	// 断开应用连接通知 [132] RELEASE-Notification，
	kP2ServerApduChoiceGetResponse = 133,	// 读取响应 [133] GET-Response，
	kP2ServerApduChoiceSetResponse = 134,	// 设置响应 [134] SET-Response，
	kP2ServerApduChoiceActionResponse = 135,	// 操作响应 [135] ACTION-Response，
	kP2ServerApduChoiceReportNotification = 136,	// 上报通知 [136] REPORT-Notification，
	kP2ServerApduChoiceProxyResponse = 137,	// 代理响应 [137] PROXY-Response，
	kP2ServerApduChoiceCompactGetResponse = 197,	// 紧凑读取响应 [197] COMPACT-GET-Response，
	kP2ServerApduChoiceCompactSetResponse = 198,	// 紧凑设置响应 [198] COMPACT-SET-Response，
	kP2ServerApduChoiceCompactProxyResponse = 201,	// 紧凑代理响应 [201] COMPACT-PROXY-Response，
	kP2ServerApduChoiceErrorResponse = 238,	// 异常响应 [238] ERROR-Response
} P2ServerApduChoiceT;

typedef enum 
{
	kP2ServerApduChoiceIxConnectResponse,	// 建立应用连接响应 [130] CONNECT-Response，
	kP2ServerApduChoiceIxReleaseResponse,	// 断开应用连接响应 [131] RELEASE-Response，
	kP2ServerApduChoiceIxReleaseNotification,	// 断开应用连接通知 [132] RELEASE-Notification，
	kP2ServerApduChoiceIxGetResponse,	// 读取响应 [133] GET-Response，
	kP2ServerApduChoiceIxSetResponse,	// 设置响应 [134] SET-Response，
	kP2ServerApduChoiceIxActionResponse,	// 操作响应 [135] ACTION-Response，
	kP2ServerApduChoiceIxReportNotification,	// 上报通知 [136] REPORT-Notification，
	kP2ServerApduChoiceIxProxyResponse,	// 代理响应 [137] PROXY-Response，
	kP2ServerApduChoiceIxCompactGetResponse,	// 紧凑读取响应 [197] COMPACT-GET-Response，
	kP2ServerApduChoiceIxCompactSetResponse,	// 紧凑设置响应 [198] COMPACT-SET-Response，
	kP2ServerApduChoiceIxCompactProxyResponse,	// 紧凑代理响应 [201] COMPACT-PROXY-Response，
	kP2ServerApduChoiceIxErrorResponse,	// 异常响应 [238] ERROR-Response
	kP2ServerApduChoiceNum
} P2ServerApduChoiceIxT;
int P2ServerApduChoice2Ix(uint8_t choice);
uint8_t P2ServerApduChoiceIx2Value(int choice_ix);
cp_t P2ServerApduChoiceIxValid(int choice_ix);
cp_t P2ServerApduChoiceValid(uint8_t choice);
const char *P2ServerApduChoiceStr(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2ServerApduChoicePcut;
#define kP2ServerApduChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ServerApduChoicePcutOpen(P2ServerApduChoicePcut *m);
cp_t P2ServerApduChoicePcutClose(P2ServerApduChoicePcut *m);
cp_t P2ServerApduChoicePcutValid(const P2ServerApduChoicePcut *m);
//}}}


// test
cp_t P2ServerApduChoiceTest(QTEST_ARG);
#endif

