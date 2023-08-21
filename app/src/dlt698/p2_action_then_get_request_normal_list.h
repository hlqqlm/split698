#ifndef __P2_ACTION_THEN_GET_REQUEST_NORMAL_LIST_H_ 
#define __P2_ACTION_THEN_GET_REQUEST_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_omd_data_oad_unsigned.h"
#include "p2_sequence_of_var_len.h"


#define kP2ActionThenGetRequestNormalListName				"action_then_get_request_normal_list"
#define kP2ActionThenGetRequestNormalListNamePiid			"piid"
#define kP2ActionThenGetRequestNormalListNameSequenceOfOmdDataOadUnsigned	"sequence_of_omd_data_oad_unsigned"

//{{{ base
/*
ActionThenGetRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
  操作若干个对象方法后读取对象属性 SEQUENCE OF
   {
   一个设置的对象方法描述符 OMD，
   方法参数 Data，
   一个读取的对象属性描述符 OAD，
   读取延时 unsigned
    }
	}
*/
#define kP2ActionThenGetRequestNormalListPiidOffset	(0)
#define kP2ActionThenGetRequestNormalListPiidSize		(kP2PiidSize)

#define kP2ActionThenGetRequestNormalListSequenceOfOmdDataOadUnsignedOffset		(kP2ActionThenGetRequestNormalListPiidOffset + kP2ActionThenGetRequestNormalListPiidSize)
#define kP2ActionThenGetRequestNormalListSequenceOfOmdDataOadUnsignedSize(_soodou_size)		(_soodou_size)


#define kP2ActionThenGetRequestNormalListWholeSize(_soodou_size)	(kP2ActionThenGetRequestNormalListSequenceOfOmdDataOadUnsignedOffset + kP2ActionThenGetRequestNormalListSequenceOfOmdDataOadUnsignedSize(_soodou_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ActionThenGetRequestNormalListCutIxPiid,
	kP2ActionThenGetRequestNormalListCutIxSequenceOfOmdDataOadUnsigned,
	kP2ActionThenGetRequestNormalListCutNum				
} P2ActionThenGetRequestNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ActionThenGetRequestNormalListCutNum];

	// sub
	P2OmdDataOadUnsignedPcut omd_data_oad_unsigned_cut;
	P2SequenceOfVarLenPcut sequence_of_omd_data_oad_unsigned_cut;
} P2ActionThenGetRequestNormalListPcut;
#define kP2ActionThenGetRequestNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2OmdDataPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2ActionThenGetRequestNormalListPcutOpen(P2ActionThenGetRequestNormalListPcut *m);
cp_t P2ActionThenGetRequestNormalListPcutClose(P2ActionThenGetRequestNormalListPcut *m);
cp_t P2ActionThenGetRequestNormalListPcutValid(const P2ActionThenGetRequestNormalListPcut *m);

cp_t P2ActionThenGetRequestNormalListPcutOpenBase(Pcut *base);
cp_t P2ActionThenGetRequestNormalListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ActionThenGetRequestNormalListTest(QTEST_ARG);
#endif

