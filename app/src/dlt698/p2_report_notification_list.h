#ifndef __P2_REPORT_NOTIFICATION_LIST_H_ 
#define __P2_REPORT_NOTIFICATION_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_a_result_normal.h"
#include "p2_sequence_of_var_len.h"


#define kP2ReportNotificationListName			"report_notification_list"
#define kP2ReportNotificationListNamePiidAcd	"piid-acd"
#define kP2ReportNotificationListNameSequenceOf	"sequence_of_a_result_normal"

//{{{ base
/*
ReportNotificationList∷=SEQUENCE
{
服务序号-优先级-ACD PIID-ACD，
若干个对象属性及数据 SEQUENCE OF A-ResultNormal
}
*/
#define kP2ReportNotificationListPiidAcdOffset		(0)
#define kP2ReportNotificationListPiidAcdSize		(kP2PiidAcdSize)

#define kP2ReportNotificationListSequenceOfOffset	\
	(kP2ReportNotificationListPiidAcdOffset + kP2ReportNotificationListPiidAcdSize)
#define kP2ReportNotificationListSequenceOfSize(_so_size)	(_so_size)


#define kP2ReportNotificationListWholeSize(_so_size)	(kP2ReportNotificationListSequenceOfOffset + kP2ReportNotificationListSequenceOfSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ReportNotificationListCutIxPiidAcd,
	kP2ReportNotificationListCutIxSequenceOf,
	kP2ReportNotificationListCutNum				
} P2ReportNotificationListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ReportNotificationListCutNum];

	// sub
	P2AResultNormalPcut a_result_normal;
	P2SequenceOfVarLenPcut sequence_of;
} P2ReportNotificationListPcut;
#define kP2ReportNotificationListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2AResultNormalPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2ReportNotificationListPcutOpen(P2ReportNotificationListPcut *m);
cp_t P2ReportNotificationListPcutClose(P2ReportNotificationListPcut *m);
cp_t P2ReportNotificationListPcutValid(const P2ReportNotificationListPcut *m);

cp_t P2ReportNotificationListPcutOpenBase(Pcut *base);
cp_t P2ReportNotificationListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ReportNotificationListTest(QTEST_ARG);
#endif

