#ifndef __P2_COMPACT_GET_RESPONSE_NORMAL_H_ 
#define __P2_COMPACT_GET_RESPONSE_NORMAL_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_get_result.h"

// name
#define kP2CompactGetResponseNormalName				"compact_get_response_normal"
#define kP2CompactGetResponseNormalNamePiidAcd		"piid-acd"
#define kP2CompactGetResponseNormalNameGetResult	kP2GetResultName


//{{{ base
/*
CompactGetResponseNormal∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  一个对象属性及结果 Get-Result
  }
*/

#define kP2CompactGetResponseNormalPiidAcdOffset		(0)
#define kP2CompactGetResponseNormalPiidAcdSize			(kP2PiidAcdSize)

#define kP2CompactGetResponseNormalGetResultOffset		\
	(kP2CompactGetResponseNormalPiidAcdOffset + kP2CompactGetResponseNormalPiidAcdSize)
#define kP2CompactGetResponseNormalGetResultSize(_get_result_size)		(_get_result_size)
//}}}


//{{{ cut
typedef enum
{
	kP2CompactGetResponseNormalCutIxPiidAcd,
	kP2CompactGetResponseNormalCutIxGetResult,
	kP2CompactGetResponseNormalCutNum				
} P2CompactGetResponseNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactGetResponseNormalCutNum];

	P2GetResultPcut get_result_cut;
} P2CompactGetResponseNormalPcut;
#define kP2CompactGetResponseNormalPcutDef {					\
	kPcutDef, { kPcutItemDef }, kP2GetResultPcutDef	\
}

cp_t P2CompactGetResponseNormalPcutOpen(P2CompactGetResponseNormalPcut *m);
cp_t P2CompactGetResponseNormalPcutClose(P2CompactGetResponseNormalPcut *m);
cp_t P2CompactGetResponseNormalPcutValid(const P2CompactGetResponseNormalPcut *m);

cp_t P2CompactGetResponseNormalPcutOpenBase(Pcut *base);
cp_t P2CompactGetResponseNormalPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactGetResponseNormalTest(QTEST_ARG);
#endif

