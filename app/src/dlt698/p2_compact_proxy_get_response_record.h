#ifndef __P2_COMPACT_PROXY_GET_RESPONSE_RECORD_H_ 
#define __P2_COMPACT_PROXY_GET_RESPONSE_RECORD_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_a_compact_result_record.h"

// name
#define kP2CompactProxyGetResponseRecordName			"compact_proxy_get_response_record"
#define kP2CompactProxyGetResponseRecordNamePiidAcd		"piid-acd"
#define kP2CompactProxyGetResponseRecordNameACompactResultRecord	kP2ACompactResultRecordName


//{{{ base
/*
CompactProxyGetResponseRecord∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  一个记录型对象属性及结果 A-CompactResultRecord
  }
*/

#define kP2CompactProxyGetResponseRecordPiidAcdOffset		(0)
#define kP2CompactProxyGetResponseRecordPiidAcdSize			(kP2PiidAcdSize)

#define kP2CompactProxyGetResponseRecordACompactResultRecordOffset		\
	(kP2CompactProxyGetResponseRecordPiidAcdOffset + kP2CompactProxyGetResponseRecordPiidAcdSize)
#define kP2CompactProxyGetResponseRecordACompactResultRecordSize(_get_result_size)		(_get_result_size)
//}}}


//{{{ cut
typedef enum
{
	kP2CompactProxyGetResponseRecordCutIxPiidAcd,
	kP2CompactProxyGetResponseRecordCutIxACompactResultRecord,
	kP2CompactProxyGetResponseRecordCutNum				
} P2CompactProxyGetResponseRecordCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxyGetResponseRecordCutNum];

	P2ACompactResultRecordPcut a_compact_result_record;
} P2CompactProxyGetResponseRecordPcut;
#define kP2CompactProxyGetResponseRecordPcutDef {					\
	kPcutDef, { kPcutItemDef }, kP2ACompactResultRecordPcutDef	\
}

cp_t P2CompactProxyGetResponseRecordPcutOpen(P2CompactProxyGetResponseRecordPcut *m);
cp_t P2CompactProxyGetResponseRecordPcutClose(P2CompactProxyGetResponseRecordPcut *m);
cp_t P2CompactProxyGetResponseRecordPcutValid(const P2CompactProxyGetResponseRecordPcut *m);

cp_t P2CompactProxyGetResponseRecordPcutOpenBase(Pcut *base);
cp_t P2CompactProxyGetResponseRecordPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyGetResponseRecordTest(QTEST_ARG);
#endif

