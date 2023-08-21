#ifndef __P2_ACTION_REQUEST_NORMAL_LIST_H_ 
#define __P2_ACTION_REQUEST_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_omd_data.h"
#include "p2_sequence_of_var_len.h"


#define kP2ActionRequestNormalListName				"action_request_normal_list"
#define kP2ActionRequestNormalListNamePiid			"piid"
#define kP2ActionRequestNormalListNameSequenceOfOmdData	"sequence_of_omd_data"

//{{{ base
/*
ActionRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
  若干个对象方法 SEQUENCE OF
   {
   一个对象方法描述符 OMD，
    方法参数 Data
	 }
	 }
*/
#define kP2ActionRequestNormalListPiidOffset	(0)
#define kP2ActionRequestNormalListPiidSize		(kP2PiidSize)

#define kP2ActionRequestNormalListSequenceOfOmdDataOffset		(kP2ActionRequestNormalListPiidOffset + kP2ActionRequestNormalListPiidSize)
#define kP2ActionRequestNormalListSequenceOfOmdDataSize(_sood_size)		(_sood_size)


#define kP2ActionRequestNormalListWholeSize(_sood_size)	(kP2ActionRequestNormalListSequenceOfOmdDataOffset + kP2ActionRequestNormalListSequenceOfOmdDataSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ActionRequestNormalListCutIxPiid,
	kP2ActionRequestNormalListCutIxSequenceOfOmdData,
	kP2ActionRequestNormalListCutNum				
} P2ActionRequestNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ActionRequestNormalListCutNum];

	// sub
	P2OmdDataPcut omd_data_cut;
	P2SequenceOfVarLenPcut sequence_of_omd_data_cut;
} P2ActionRequestNormalListPcut;
#define kP2ActionRequestNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2OmdDataPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2ActionRequestNormalListPcutOpen(P2ActionRequestNormalListPcut *m);
cp_t P2ActionRequestNormalListPcutClose(P2ActionRequestNormalListPcut *m);
cp_t P2ActionRequestNormalListPcutValid(const P2ActionRequestNormalListPcut *m);

cp_t P2ActionRequestNormalListPcutOpenBase(Pcut *base);
cp_t P2ActionRequestNormalListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ActionRequestNormalListTest(QTEST_ARG);
#endif

