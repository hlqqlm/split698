#ifndef __P2_CLIENT_APDU_H_ 
#define __P2_CLIENT_APDU_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_client_apdu_choice.h"
#include "p2_optional.h"
#include "p2_timetag.h"

//{{{ base
// 基本信息
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
#define kP2ClientApduChoiceOffset				(0)
#define kP2ClientApduChoiceSize(_choice_size)	(_choice_size)

#define kP2ClientApduOptionalTimetagOffset(_choice_size)	\
			(kP2ClientApduChoiceOffset + kP2ClientApduChoiceSize(_choice_size))
#define kP2ClientApduOptionalTimetagSize(_optional_timetag_size)	(_optional_timetag_size)


// choice
uint8_t P2ClientApduChoice(const char *whole);

// var
int P2ClientApduVarSize(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2ClientApduCutIxChoice,
	kP2ClientApduCutIxOptionalTimetag,
	kP2ClientApduCutNum				
} P2ClientApduCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ClientApduCutNum];

	P2ClientApduChoicePcut choice_cut;
	P2OptionalPcut optional_cut;
	P2TimetagPcut timetag_cut;
} P2ClientApduPcut;
#define kP2ClientApduPcutDef {												\
	kPcutDef, { kPcutItemDef },										\
	kP2ClientApduChoicePcutDef, kP2OptionalPcutDef, kP2TimetagPcutDef	\
}

cp_t P2ClientApduPcutOpen(P2ClientApduPcut *m);
cp_t P2ClientApduPcutClose(P2ClientApduPcut *m);
uint8_t P2ClientApduPcutChoice(P2ClientApduPcut *m);
//}}}


// test
cp_t P2ClientApduTest(QTEST_ARG);
#endif

