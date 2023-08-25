#ifndef __P2_REPORT_RESPONSE_CHOICE_H_
#define __P2_REPORT_RESPONSE_CHOICE_H_



#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

#include "p2_choice.h"


#define kP2ReportResponseName				"report_response"
#define kP2ReportResponseNameChoice			"report_response_choice"
#define kP2ReportResponseNameVar			"report_response_var"

#define kP2ReportResponseChoiceOffset		(0)
#define kP2ReportResponseChoiceSize			(1)

#define kP2ReportResponseVarOffset			(kP2ReportResponseChoiceOffset + kP2ReportResponseChoiceSize)
#define kP2ReportResponseVarSize(_var_size)	(_var_size)

//{{{ choice
/*
REPORT
Respons e ∷∷=CHOICE
{
上报若干个对象属性的响应 [1] ReportResponseList
上报若干个记录型对象属性的响应 [2] ReportResponseRecordList
上报透明数据的响应 [3] ReportResponseTransData
请求客户机服务的响应 [4] ReportResponseClientService
上报精简的记录型对象属性的响应 [6] ReportResponseSimplifyRecord
}
*/
typedef enum 
{
	kP2ReportResponseChoiceList = 1,		// 上报若干个对象属性的响应 [1] ReportResponseList
	kP2ReportResponseChoiceRecordList = 2,	// 上报若干个记录型对象属性的响应 [2] ReportResponseRecordList
	kP2ReportResponseChoiceTransData = 3,	// 上报透明数据的响应 [3] ReportResponseTransData
	kP2ReportResponseChoiceClientService = 4,	// 请求客户机服务的响应 [4] ReportResponseClientService
	kP2ReportResponseChoiceSimplifyRecord = 6,	// 上报精简的记录型对象属性的响应 [6] ReportResponseSimplifyRecord
} P2ReportResponseChoiceT;

typedef enum 
{
	kP2ReportResponseChoiceIxList,				// 
	kP2ReportResponseChoiceIxRecordList,		// 
	kP2ReportResponseChoiceIxTransData,			// 
	kP2ReportResponseChoiceIxClientService,		// 
	kP2ReportResponseChoiceIxSimplifyRecord,	// 
	kP2ReportResponseChoiceNum
} P2ReportResponseChoiceIxT;
int P2ReportResponseChoice2Ix(uint8_t choice);
uint8_t P2ReportResponseChoiceIx2Value(int choice_ix);
cp_t P2ReportResponseChoiceIxValid(int choice_ix);
cp_t P2ReportResponseChoiceValid(uint8_t choice);
const char *P2ReportResponseChoiceStr(uint8_t choice);
//}}}


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2ReportResponseChoicePcut;
#define kP2ReportResponseChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2ReportResponseChoicePcutOpen(P2ReportResponseChoicePcut *m);
cp_t P2ReportResponseChoicePcutClose(P2ReportResponseChoicePcut *m);
cp_t P2ReportResponseChoicePcutValid(const P2ReportResponseChoicePcut *m);


// 声明用子类base来open/close父类的函数
//PCUT_DECLEAR_OPEN_BASE(P2ReportResponseChoicePcut);
//PCUT_DECLEAR_CLOSE_BASE(P2ReportResponseChoicePcut);
cp_t P2ReportResponseChoicePcutOpenBase(Pcut *base);
cp_t P2ReportResponseChoicePcutCloseBase(Pcut *base);
//}}}


cp_t P2ReportResponseChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2ReportResponseChoiceTest(QTEST_ARG);
#endif

