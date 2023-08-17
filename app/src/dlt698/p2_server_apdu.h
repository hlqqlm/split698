#ifndef __P2_SERVER_APDU_H_ 
#define __P2_SERVER_APDU_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_server_apdu_choice.h"
#include "p2_optional.h"
#include "p2_follow_report.h"
#include "p2_timetag.h"

// name
#define kP2ServerApduName					"server_apdu"
#define kP2ServerApduNameChoice				"server_apdu_choice"
#define kP2ServerApduNameChoiceVar			"server_apdu_choice_var"
#define kP2ServerApduNameFollowReportOptional	"server_apdu_follow_report_optional"
#define kP2ServerApduNameFollowReport		"server_apdu_follow_report"
#define kP2ServerApduNameTimetagOptional	"server_apdu_timetag_optional"
#define kP2ServerApduNameTimetag			"server_apdu_timetag"


//{{{ base
// 基本信息
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
#define kP2ServerApduChoiceOffset				(0)
#define kP2ServerApduChoiceSize(_choice_size)	(_choice_size)

#define kP2ServerApduOptionalFollowReportOffset(_choice_size)	(kP2ServerApduChoiceOffset + kP2ServerApduChoiceSize(_choice_size))
#define kP2ServerApduOptionalFollowReportSize(_optional_follow_report_size)	(_optional_follow_report_size)

#define kP2ServerApduOptionalTimetagOffset(_choice_size, _optional_follow_report_size)	\
			(kP2ServerApduOptionalFollowReportOffset(_choice_size) + kP2ServerApduOptionalFollowReportSize(_optional_follow_report_size))
#define kP2ServerApduOptionalTimetagSize(_optional_timetag_size)	(_optional_timetag_size)
//}}}

// choice
uint8_t P2ServerApduChoice(const char *whole);


//{{{ cut
typedef enum
{
	kP2ServerApduCutIxChoice,
	kP2ServerApduCutIxOptionalFollowReport,
	kP2ServerApduCutIxOptionalTimetag,
	kP2ServerApduCutNum				
} P2ServerApduCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ServerApduCutNum];

	P2ServerApduChoicePcut choice_part;

	P2OptionalPcut optional_follow_report;
	P2FollowReportPcut follow_report_part;

	P2OptionalPcut optional_timetag;
	P2TimetagPcut timetag_part;
} P2ServerApduPcut;
#define kP2ServerApduPcutDef {							\
	kPcutDef, { kPcutItemDef },						\
	kP2ServerApduChoicePcutDef,						\
	kP2OptionalPcutDef, kP2FollowReportPcutDef,		\
	kP2OptionalPcutDef, kP2TimetagPcutDef				\
}

cp_t P2ServerApduPcutOpen(P2ServerApduPcut *m);
cp_t P2ServerApduPcutClose(P2ServerApduPcut *m);
uint8_t P2ServerApduPcutChoice(P2ServerApduPcut *m);
//}}}


// if NULL==follow_report, optional=0
// if NULL==timetag, optional=0
cp_t P2ServerApduFillInit(Pfill *m, uint8_t choice, Pfill *sub_choice_var, Pfill *follow_report, Pfill *timetag);


// test
cp_t P2ServerApduTest(QTEST_ARG);
#endif

