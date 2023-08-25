#ifndef __P2_CLIENT_APDU_CHOICE_H_ 
#define __P2_CLIENT_APDU_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_choice.h"


#define P2ClientApduChoiceName			"client_apdu_choice"

//{{{ choice
/*
Client-APDU∷=SEQUENCE
{
 应用层服务 CHOICE
 {
 建立应用连接请求 [2] CONNECT-Request，
 断开应用连接请求 [3] RELEASE-Request，
 读取请求 [5] GET-Request，
 设置请求 [6] SET-Request，
 操作请求 [7] ACTION-Request，
上报应答 [8] REPORT-Response，
代理请求 [9] PROXY-Request，
紧凑读取请求 [69] COMPACT-GET-Request，
紧凑设置请求 [70] COMPACT-SET-Request，
紧凑代理请求 [73] COMPACT-PROXY-Request，
异常响应 [110] ERROR-Response
}，
时间标签域 TimeTag OPTIONAL
}
*/
#define kP2ClientApduChoiceInvalid		(0)
typedef enum 
{
	kP2ClientApduChoiceConnectRequest = 2,	// 建立应用连接请求 [2] CONNECT-Request，
	kP2ClientApduChoiceReleaseRequest = 3,	// 断开应用连接请求 [3] RELEASE-Request，
	kP2ClientApduChoiceGetRequest = 5,	// 读取请求 [5] GET-Request，
	kP2ClientApduChoiceSetRequest = 6,	// 设置请求 [6] SET-Request，
	kP2ClientApduChoiceActionRequest = 7,	// 操作请求 [7] ACTION-Request，
	kP2ClientApduChoiceReportResponse = 8,	// 上报应答 [8] REPORT-Response，
	kP2ClientApduChoiceProxyRequest = 9,	// 代理请求 [9] PROXY-Request，
	kP2ClientApduChoiceCompactGetRequest = 69,	// 紧凑读取请求 [69] COMPACT-GET-Request，
	kP2ClientApduChoiceCompactSetRequest = 70,	// 紧凑设置请求 [70] COMPACT-SET-Request，
	kP2ClientApduChoiceCompactProxyRequest = 73,	// 紧凑代理请求 [73] COMPACT-PROXY-Request，
	kP2ClientApduChoiceErrorResponse = 110	// 异常响应 [110] ERROR-Response
} P2ClientApduChoiceT;

typedef enum 
{
	kP2ClientApduChoiceIxConnectRequest,	// 建立应用连接请求 [2] CONNECT-Request，
	kP2ClientApduChoiceIxReleaseRequest,	// 断开应用连接请求 [3] RELEASE-Request，
	kP2ClientApduChoiceIxGetRequest,	// 读取请求 [5] GET-Request，
	kP2ClientApduChoiceIxSetRequest,	// 设置请求 [6] SET-Request，
	kP2ClientApduChoiceIxActionRequest,	// 操作请求 [7] ACTION-Request，
	kP2ClientApduChoiceIxReportResponse,	// 上报应答 [8] REPORT-Response，
	kP2ClientApduChoiceIxProxyRequest,	// 代理请求 [9] PROXY-Request，
	kP2ClientApduChoiceIxCompactGetRequest,	// 紧凑读取请求 [69] COMPACT-GET-Request，
	kP2ClientApduChoiceIxCompactSetRequest,	// 紧凑设置请求 [70] COMPACT-SET-Request，
	kP2ClientApduChoiceIxCompactProxyRequest,	// 紧凑代理请求 [73] COMPACT-PROXY-Request，
	kP2ClientApduChoiceIxErrorResponse,		// 异常响应 [110] ERROR-Response
	kP2ClientApduChoiceNum
} P2ClientApduChoiceIxT;
int P2ClientApduChoice2Ix(uint8_t choice);
uint8_t P2ClientApduChoiceIx2Value(int choice_ix);
cp_t P2ClientApduChoiceIxValid(int choice_ix);
cp_t P2ClientApduChoiceValid(uint8_t choice);
const char *P2ClientApduChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2ClientApduChoicePcut;
#define kP2ClientApduChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ClientApduChoicePcutOpen(P2ClientApduChoicePcut *m);
cp_t P2ClientApduChoicePcutClose(P2ClientApduChoicePcut *m);
cp_t P2ClientApduChoicePcutValid(const P2ClientApduChoicePcut *m);
//}}}


// test
cp_t P2ClientApduChoiceTest(QTEST_ARG);
#endif

