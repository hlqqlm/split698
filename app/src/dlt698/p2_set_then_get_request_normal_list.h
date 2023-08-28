#ifndef __P2_SET_THEN_GET_REQUEST_NORMAL_LIST_H_ 
#define __P2_SET_THEN_GET_REQUEST_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_oad_data_oad_unsigned.h"
#include "p2_sequence_of_var_len.h"


#define kP2SetThenGetRequestNormalListName				"action_then_get_request_normal_list"
#define kP2SetThenGetRequestNormalListNamePiid			"piid"
#define kP2SetThenGetRequestNormalListNameSequenceOf	"sequence_of_oad_data_oad_unsigned"

//{{{ base
/*
SetThenGetRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
  若干个设置后读取对象属性 SEQUENCE OF
   {
   一个设置的对象属性 OAD，
   数据 Data，
   一个读取的对象属性 OAD，
   延时读取时间 unsigned
    }
	}
*/
#define kP2SetThenGetRequestNormalListPiidOffset	(0)
#define kP2SetThenGetRequestNormalListPiidSize		(kP2PiidSize)

#define kP2SetThenGetRequestNormalListSequenceOfOffset		(kP2SetThenGetRequestNormalListPiidOffset + kP2SetThenGetRequestNormalListPiidSize)
#define kP2SetThenGetRequestNormalListSequenceOfSize(_so_size)		(_so_size)


#define kP2SetThenGetRequestNormalListWholeSize(_so_size)	(kP2SetThenGetRequestNormalListSequenceOfOffset + kP2SetThenGetRequestNormalListSequenceOfSize(_so_size))
//}}}


//{{{ cut
typedef enum
{
	kP2SetThenGetRequestNormalListCutIxPiid,
	kP2SetThenGetRequestNormalListCutIxSequenceOf,
	kP2SetThenGetRequestNormalListCutNum				
} P2SetThenGetRequestNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SetThenGetRequestNormalListCutNum];

	// sub
	P2OadDataOadUnsignedPcut oad_data_oad_unsigned_cut;
	P2SequenceOfVarLenPcut sequence_of_cut;
} P2SetThenGetRequestNormalListPcut;
#define kP2SetThenGetRequestNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2OadDataOadUnsignedPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2SetThenGetRequestNormalListPcutOpen(P2SetThenGetRequestNormalListPcut *m);
cp_t P2SetThenGetRequestNormalListPcutClose(P2SetThenGetRequestNormalListPcut *m);
cp_t P2SetThenGetRequestNormalListPcutValid(const P2SetThenGetRequestNormalListPcut *m);

cp_t P2SetThenGetRequestNormalListPcutOpenBase(Pcut *base);
cp_t P2SetThenGetRequestNormalListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SetThenGetRequestNormalListTest(QTEST_ARG);
#endif

