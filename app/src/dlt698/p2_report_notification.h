#ifndef __P2_REPORT_NOTIFICATION_H_ 
#define __P2_REPORT_NOTIFICATION_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"

// name
#define kP2ReportNotificationChoiceName		"report_notification_choice"
#define kP2ReportNotificationName			"report_notification"

// basic information
#define kP2ReportNotificationChoiceOffset			(0)
#define kP2ReportNotificationChoiceSize				(1)

#define kP2ReportNotificationVarOffset				\
	(kP2ReportNotificationChoiceOffset + kP2ReportNotificationChoiceSize)
#define kP2ReportNotificationVarSize(_var_size)		(_var_size)



//{{{ choice
/*
REPORT-Notification∷=CHOICE
{
通知上报若干个对象属性 [1] ReportNotificationList，
通知上报若干个记录型对象属性 [2] ReportNotificationRecordList，
通知上报透明数据 [3] ReportNotificationTransData，
请求客户机服务 [4] ReportRequestClientService，
通知上报精简的记录型对象属性 [6] ReportNotificationSimplifyRecord
}
*/
typedef enum
{
	kP2ReportNotificationChoiceList = 1, // 通知上报若干个对象属性 [1] ReportNotificationList，
	kP2ReportNotificationChoiceRecordList = 2, // 通知上报若干个记录型对象属性 [2] ReportNotificationRecordList，
	kP2ReportNotificationChoiceTransData = 3, // 通知上报透明数据 [3] ReportNotificationTransData，
	kP2ReportNotificationChoiceClientService = 4, // 请求客户机服务 [4] ReportRequestClientService，
	kP2ReportNotificationChoiceSimplifyRecord = 6, // 通知上报精简的记录型对象属性 [6] ReportNotificationSimplifyRecord
} P2ReportNotificationChoiceT;
typedef enum 
{
	kP2ReportNotificationChoiceIxList, // 
	kP2ReportNotificationChoiceIxRecordList, // 
	kP2ReportNotificationChoiceIxTransData, // 
	kP2ReportNotificationChoiceIxClientService, // 
	kP2ReportNotificationChoiceIxSimplifyRecord, // 
	kP2ReportNotificationChoiceNum
} P2ReportNotificationChoiceIxT;
int P2ReportNotificationChoice2Ix(uint8_t choice);
uint8_t P2ReportNotificationChoiceIx2Value(int choice_ix);
cp_t P2ReportNotificationChoiceIxValid(int choice_ix);
cp_t P2ReportNotificationChoiceValid(uint8_t choice);

uint8_t P2ReportNotificationChoice(const char *whole);
//}}}


//{{{ cut
typedef struct 
{
	P2ChoicePcut choice;
} P2ReportNotificationPcut;
#define kP2ReportNotificationPcutDef {								\
	kP2ChoicePcutDef,										\
}

cp_t P2ReportNotificationPcutOpen(P2ReportNotificationPcut *m);
cp_t P2ReportNotificationPcutClose(P2ReportNotificationPcut *m);
cp_t P2ReportNotificationPcutValid(const P2ReportNotificationPcut *m);

cp_t P2ReportNotificationPcutOpenBase(Pcut *base);
cp_t P2ReportNotificationPcutCloseBase(Pcut *base);
//}}}


// fill
cp_t P2ReportNotificationFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ReportNotificationTest(QTEST_ARG);
#endif

