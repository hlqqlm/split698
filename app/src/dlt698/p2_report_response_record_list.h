#ifndef __P2_REPORT_RESPONSE_RECORD_LIST_H_ 
#define __P2_REPORT_RESPONSE_RECORD_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_sequence_of.h"


#define kP2ReportResponseRecordListName					"report_response.record_list"
#define kP2ReportResponseRecordListNamePiid				"piid"
#define kP2ReportResponseRecordListNameSequenceOfOad	"sequence_of_oad"

//{{{ base
/*
ReportResponseRecordList ∷∷=SEQUENCE
{
	服务序号 优先级 PIID
	对应上报的若干个对象属性描述符 SEQUENCE OF OAD
}
*/
// Arn = Action Request Normal
#define kP2ReportResponseRecordListPiidOffset	(0)
#define kP2ReportResponseRecordListPiidSize		(kP2PiidSize)

#define kP2ReportResponseRecordListSequenceOfOadOffset		(kP2ReportResponseRecordListPiidOffset + kP2ReportResponseRecordListPiidSize)
#define kP2ReportResponseRecordListSequenceOfOadSize(_soo_size)		(_soo_size)


#define kP2ReportResponseRecordListWholeSize(_data_size)	(kP2ReportResponseRecordListSequenceOfOadOffset + kP2ReportResponseRecordListSequenceOfOadSize(_soo_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ReportResponseRecordListCutIxPiid,
	kP2ReportResponseRecordListCutIxSequenceOfOad,
	kP2ReportResponseRecordListCutNum				
} P2ReportResponseRecordListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ReportResponseRecordListCutNum];

	// sub
	P2SequenceOfPcut sequence_of_oad;
} P2ReportResponseRecordListPcut;
#define kP2ReportResponseRecordListPcutDef { kPcutDef, { kPcutItemDef }, kP2SequenceOfPcutDef }

cp_t P2ReportResponseRecordListPcutOpen(P2ReportResponseRecordListPcut *m);
cp_t P2ReportResponseRecordListPcutClose(P2ReportResponseRecordListPcut *m);
cp_t P2ReportResponseRecordListPcutValid(const P2ReportResponseRecordListPcut *m);

cp_t P2ReportResponseRecordListPcutOpenBase(Pcut *base);
cp_t P2ReportResponseRecordListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ReportResponseRecordListTest(QTEST_ARG);
#endif

