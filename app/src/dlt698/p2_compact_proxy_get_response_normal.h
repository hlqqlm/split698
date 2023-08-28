#ifndef __P2_COMPACT_PROXY_GET_RESPONSE_NORMAL_H_ 
#define __P2_COMPACT_PROXY_GET_RESPONSE_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_get_result.h"
#include "p2_sequence_of_var_len.h"


#define kP2CompactProxyGetResponseNormalName			"compact_proxy_get_response_normal"
#define kP2CompactProxyGetResponseNormalNamePiidAcd		"piid-acd"
#define kP2CompactProxyGetResponseNormalNameSequenceOf	"sequence_of_get_result"

//{{{ base
/*
CompactProxyGetResponseNormal∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 若干个对象属性及结果 SEQUENCE OF Get-Result
 }
*/
#define kP2CompactProxyGetResponseNormalPiidAcdOffset		(0)
#define kP2CompactProxyGetResponseNormalPiidAcdSize			(kP2PiidAcdSize)

#define kP2CompactProxyGetResponseNormalSequenceOfOffset	\
	(kP2CompactProxyGetResponseNormalPiidAcdOffset + kP2CompactProxyGetResponseNormalPiidAcdSize)
#define kP2CompactProxyGetResponseNormalSequenceOfSize(_so_size)	(_so_size)


#define kP2CompactProxyGetResponseNormalWholeSize(_so_size)	(kP2CompactProxyGetResponseNormalSequenceOfOffset + kP2CompactProxyGetResponseNormalSequenceOfSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2CompactProxyGetResponseNormalCutIxPiidAcd,
	kP2CompactProxyGetResponseNormalCutIxSequenceOf,
	kP2CompactProxyGetResponseNormalCutNum				
} P2CompactProxyGetResponseNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactProxyGetResponseNormalCutNum];

	// sub
	P2GetResultPcut get_result;
	P2SequenceOfVarLenPcut sequence_of;
} P2CompactProxyGetResponseNormalPcut;
#define kP2CompactProxyGetResponseNormalPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2GetResultPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2CompactProxyGetResponseNormalPcutOpen(P2CompactProxyGetResponseNormalPcut *m);
cp_t P2CompactProxyGetResponseNormalPcutClose(P2CompactProxyGetResponseNormalPcut *m);
cp_t P2CompactProxyGetResponseNormalPcutValid(const P2CompactProxyGetResponseNormalPcut *m);

cp_t P2CompactProxyGetResponseNormalPcutOpenBase(Pcut *base);
cp_t P2CompactProxyGetResponseNormalPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactProxyGetResponseNormalTest(QTEST_ARG);
#endif

