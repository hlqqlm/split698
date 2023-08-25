#ifndef __P2_RELEASE_NOTIFICATION_H_ 
#define __P2_RELEASE_NOTIFICATION_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"
#include "qdlt698.h"
#include "p2_datetimes.h"


#define kP2ReleaseNotificationName					"release_notification"
#define kP2ReleaseNotificationNamePiidAcd			"piid-acd"
#define kP2ReleaseNotificationNameLinkDatetimes		"link_datetimes"
#define kP2ReleaseNotificationNameServerDatetimes	"server_datetimes"


//{{{ base
/*
RELEASE-Notification∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  应用连接建立时间 date_time_s，
   服务器当前时间 date_time_s
   }
*/
#define kP2ReleaseNotificationPiidAcdOffset			(0)
#define kP2ReleaseNotificationPiidAcdSize			(kP2PiidSize)

#define kP2ReleaseNotificationLinkDatetimesOffset		\
	(kP2ReleaseNotificationPiidAcdOffset + kP2ReleaseNotificationPiidAcdSize)
#define kP2ReleaseNotificationLinkDatetimesSize			(kDlt698DatetimeSSize)

#define kP2ReleaseNotificationServerDatetimesOffset			\
	(kP2ReleaseNotificationLinkDatetimesOffset + kP2ReleaseNotificationLinkDatetimesSize)
#define kP2ReleaseNotificationServerDatetimesSize		(kDlt698DatetimeSSize)
//}}}


//{{{ cut
typedef enum
{
	kP2ReleaseNotificationCutIxPiidAcd,
	kP2ReleaseNotificationCutIxLinkDatetimes,
	kP2ReleaseNotificationCutIxServerDatetimes,
	kP2ReleaseNotificationCutNum				
} P2ReleaseNotificationCutIxT;

typedef struct 
{
	Pcut base;
	PcutItem items[kP2ReleaseNotificationCutNum];

	P2DatetimesPcut link_datetimes;
	P2DatetimesPcut server_datetimes;
} P2ReleaseNotificationPcut;
#define kP2ReleaseNotificationPcutDef { kPcutDef, { kPcutItemDef }, \
	kP2DatetimesPcutDef, kP2DatetimesPcutDef }

cp_t P2ReleaseNotificationPcutOpen(P2ReleaseNotificationPcut *m);
cp_t P2ReleaseNotificationPcutClose(P2ReleaseNotificationPcut *m);
cp_t P2ReleaseNotificationPcutValid(const P2ReleaseNotificationPcut *m);

cp_t P2ReleaseNotificationPcutOpenBase(Pcut *base);
cp_t P2ReleaseNotificationPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ReleaseNotificationTest(QTEST_ARG);
#endif

