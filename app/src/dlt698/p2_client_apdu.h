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

/*
// choice对应的数据，var即变化部分。例如：choice是5 GET_Request，后面对应的数据。
// _data_size，是通过sub part计算出来的。例如：choice是5 GET_Request，然后设置sub part，计算出来size
#define P2_CLIENT_APDU_VAR_OFFSET		(kP2ClientApduChoiceOffset + kP2ClientApduChoiceSize)
#define P2_CLIENT_APDU_VAR_SIZE(_var_size)	(_var_size)
*/

#define kP2ClientApduOptionalTimetagOffset(_choice_size)	\
			(kP2ClientApduChoiceOffset + kP2ClientApduChoiceSize(_choice_size))
#define kP2ClientApduOptionalTimetagSize(_optional_timetag_size)	(_optional_timetag_size)

/*
// timetag的可选部分
#define P2_CLIENT_APDU_TIMETAG_OPTIONAL_OFFSET(_data_size)	\
			(kP2ClientApduChoiceOffset + kP2ClientApduChoiceSize(_data_size))
#define P2_CLIENT_APDU_TIMETAG_OPTIONAL_SIZE	(1)

// 因为timetag是可选部分，因此，这部分不一定存在; 如果不存在，_timetag_size=0
#define P2_CLIENT_APDU_TIMETAG_OFFSET(_data_size)	\
			(P2_CLIENT_APDU_TIMETAG_OPTIONAL_OFFSET + P2_CLIENT_APDU_TIMETAG_OPTIONAL_SIZE(_data_size))
#define P2_CLIENT_APDU_TIMETAG_SIZE(_timetag_size)	(_timetag_size)
*/

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

	P2ClientApduChoicePcut choice_part;
	P2OptionalPcut optional_part;
	P2TimetagPcut timetag_part;
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

