#ifndef __P2_GET_REQUEST_RECORD_H_ 
#define __P2_GET_REQUEST_RECORD_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_get_record.h"


#define kP2GetRequestRecordName				"get_request_record"
#define kP2GetRequestRecordNamePiid			"piid"
#define kP2GetRequestRecordNameGetRecord	"get_record"





//{{{ base
/*
GetRequestRecord∷=SEQUENCE
{
 服务序号-优先级 PIID，
  读取一个记录型对象属性 GetRecord
  }
*/

// basic information
#define kP2GetRequestRecordPiidOffset		(0)
#define kP2GetRequestRecordPiidSize			(1)

#define kP2GetRequestRecordGetRecordOffset	(kP2GetRequestRecordPiidOffset + kP2GetRequestRecordPiidSize)
#define kP2GetRequestRecordGetRecordSize(_gr_size)	(_gr_sizeOAD_MEM_SIZE)
//}}}


//{{{ cut
typedef enum
{
	kP2GetRequestRecordCutIxPiid,
	kP2GetRequestRecordCutIxGetRecord,
	kP2GetRequestRecordCutNum				
} P2GetRequestRecordCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetRequestRecordCutNum];

	// sub
	P2GetRecordPcut get_record_cut;
} P2GetRequestRecordPcut;
#define kP2GetRequestRecordPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2GetRecordPcutDef						\
}

cp_t P2GetRequestRecordPcutOpen(P2GetRequestRecordPcut *m);
cp_t P2GetRequestRecordPcutClose(P2GetRequestRecordPcut *m);
cp_t P2GetRequestRecordPcutValid(const P2GetRequestRecordPcut *m);

cp_t P2GetRequestRecordPcutOpenBase(Pcut *base);
cp_t P2GetRequestRecordPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2GetRequestRecordTest(QTEST_ARG);
#endif

