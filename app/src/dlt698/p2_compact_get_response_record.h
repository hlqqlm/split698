#ifndef __P2_COMPACT_GET_RESPONSE_RECORD_H_ 
#define __P2_COMPACT_GET_RESPONSE_RECORD_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_a_compact_result_record.h"

// name
#define kP2CompactGetResponseRecordName				"compact_get_response_record"
#define kP2CompactGetResponseRecordNamePiidAcd		"piid-acd"
#define kP2CompactGetResponseRecordNameACompactResultRecord	kP2ACompactResultRecordName


//{{{ base
/*
CompactGetResponseRecord∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  一个记录型对象属性及结果 A-CompactResultRecord
  }
*/

#define kP2CompactGetResponseRecordPiidAcdOffset		(0)
#define kP2CompactGetResponseRecordPiidAcdSize			(kP2PiidAcdSize)

#define kP2CompactGetResponseRecordACompactResultRecordOffset		\
	(kP2CompactGetResponseRecordPiidAcdOffset + kP2CompactGetResponseRecordPiidAcdSize)
#define kP2CompactGetResponseRecordACompactResultRecordSize(_get_result_size)		(_get_result_size)
//}}}


//{{{ cut
typedef enum
{
	kP2CompactGetResponseRecordCutIxPiidAcd,
	kP2CompactGetResponseRecordCutIxACompactResultRecord,
	kP2CompactGetResponseRecordCutNum				
} P2CompactGetResponseRecordCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactGetResponseRecordCutNum];

	P2ACompactResultRecordPcut a_compact_result_record;
} P2CompactGetResponseRecordPcut;
#define kP2CompactGetResponseRecordPcutDef {					\
	kPcutDef, { kPcutItemDef }, kP2ACompactResultRecordPcutDef	\
}

cp_t P2CompactGetResponseRecordPcutOpen(P2CompactGetResponseRecordPcut *m);
cp_t P2CompactGetResponseRecordPcutClose(P2CompactGetResponseRecordPcut *m);
cp_t P2CompactGetResponseRecordPcutValid(const P2CompactGetResponseRecordPcut *m);

cp_t P2CompactGetResponseRecordPcutOpenBase(Pcut *base);
cp_t P2CompactGetResponseRecordPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactGetResponseRecordTest(QTEST_ARG);
#endif

