#ifndef __P2_COMPACT_GET_RESPONSE_NORMAL_LIST_H_ 
#define __P2_COMPACT_GET_RESPONSE_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_get_result.h"
#include "p2_sequence_of_var_len.h"


#define kP2CompactGetResponseNormalListName				"compact_get_response_normal_list"
#define kP2CompactGetResponseNormalListNamePiidAcd		"piid-acd"
#define kP2CompactGetResponseNormalListNameSequenceOf	"sequence_of_get_result"

//{{{ base
/*
CompactGetResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 若干个对象属性及结果 SEQUENCE OF Get-Result
 }
*/
#define kP2CompactGetResponseNormalListPiidAcdOffset		(0)
#define kP2CompactGetResponseNormalListPiidAcdSize			(kP2PiidAcdSize)

#define kP2CompactGetResponseNormalListSequenceOfOffset	\
	(kP2CompactGetResponseNormalListPiidAcdOffset + kP2CompactGetResponseNormalListPiidAcdSize)
#define kP2CompactGetResponseNormalListSequenceOfSize(_so_size)	(_so_size)


#define kP2CompactGetResponseNormalListWholeSize(_so_size)	(kP2CompactGetResponseNormalListSequenceOfOffset + kP2CompactGetResponseNormalListSequenceOfSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2CompactGetResponseNormalListCutIxPiidAcd,
	kP2CompactGetResponseNormalListCutIxSequenceOf,
	kP2CompactGetResponseNormalListCutNum				
} P2CompactGetResponseNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactGetResponseNormalListCutNum];

	// sub
	P2GetResultPcut get_result;
	P2SequenceOfVarLenPcut sequence_of;
} P2CompactGetResponseNormalListPcut;
#define kP2CompactGetResponseNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2GetResultPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2CompactGetResponseNormalListPcutOpen(P2CompactGetResponseNormalListPcut *m);
cp_t P2CompactGetResponseNormalListPcutClose(P2CompactGetResponseNormalListPcut *m);
cp_t P2CompactGetResponseNormalListPcutValid(const P2CompactGetResponseNormalListPcut *m);

cp_t P2CompactGetResponseNormalListPcutOpenBase(Pcut *base);
cp_t P2CompactGetResponseNormalListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2CompactGetResponseNormalListTest(QTEST_ARG);
#endif

