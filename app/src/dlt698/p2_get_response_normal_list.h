#ifndef __P2_GET_RESPONSE_NORMAL_LIST_H_ 
#define __P2_GET_RESPONSE_NORMAL_LIST_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_a_result_normal.h"
#include "p2_sequence_of_var_len.h"


#define kP2GetResponseNormalListName			"get_response_normal_list"
#define kP2GetResponseNormalListNamePiidAcd			"piid-acd"
#define kP2GetResponseNormalListNameSequenceOfAResultNormal	"sequence_of_a_result_normal"


// basic information
#define kP2GetResponseNormalListPiidAcdOffset		(0)
#define kP2GetResponseNormalListPiidAcdSize			(kP2PiidAcdSize)

#define kP2GetResponseNormalListSequenceOfAResultNormalOffset	\
	(kP2GetResponseNormalListPiidAcdOffset + kP2GetResponseNormalListPiidAcdSize)
#define kP2GetResponseNormalListSequenceOfAResultNormalSize(_sequence_of_a_result_normal_size)		\
	(_sequence_of_a_result_normal_size)

/*
GetResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 若干个对象属性及结果 SEQUENCE OF A-ResultNormal
}
*/


//{{{ cut
typedef enum
{
	kP2GetResponseNormalListCutIxPiidAcd,
	kP2GetResponseNormalListCutIxSequenceOf,
	kP2GetResponseNormalListCutNum				
} P2GetResponseNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetResponseNormalListCutNum];

	// sub
	P2AResultNormalPcut a_result_normal;
	P2SequenceOfVarLenPcut sequence_of;
} P2GetResponseNormalListPcut;
#define kP2GetResponseNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2AResultNormalPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2GetResponseNormalListPcutOpen(P2GetResponseNormalListPcut *m);
cp_t P2GetResponseNormalListPcutClose(P2GetResponseNormalListPcut *m);
cp_t P2GetResponseNormalListPcutValid(const P2GetResponseNormalListPcut *m);

cp_t P2GetResponseNormalListPcutOpenBase(Pcut *base);
cp_t P2GetResponseNormalListPcutCloseBase(Pcut *base);
//}}}




// fill
cp_t P2GetResponseNormalListFillInit(Pfill *m, const P2PiidAcd *piid_acd, Pfill *fill_sequence_of_a_result_normal);

// test
cp_t P2GetResponseNormalListTest(QTEST_ARG);
#endif

