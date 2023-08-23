#ifndef __P2_SET_REQUEST_NORMAL_LIST_H_ 
#define __P2_SET_REQUEST_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_oad_data.h"
#include "p2_sequence_of_var_len.h"


#define kP2SetRequestNormalListName				"set_request_normal_list"
#define kP2SetRequestNormalListNamePiid			"piid"
#define kP2SetRequestNormalListNameSequenceOfOadData	"sequence_of_oad_data"

//{{{ base
/*
SetRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
  若干个对象属性 SEQUENCE OF
   {
   一个对象属性描述符 OAD，
    数据 Data
	 }
	 }
*/
#define kP2SetRequestNormalListPiidOffset	(0)
#define kP2SetRequestNormalListPiidSize		(kP2PiidSize)

#define kP2SetRequestNormalListSequenceOfOadDataOffset		(kP2SetRequestNormalListPiidOffset + kP2SetRequestNormalListPiidSize)
#define kP2SetRequestNormalListSequenceOfOadDataSize(_sood_size)		(_sood_size)


#define kP2SetRequestNormalListWholeSize(_sood_size)	(kP2SetRequestNormalListSequenceOfOadDataOffset + kP2SetRequestNormalListSequenceOfOadDataSize(_sood_size))
//}}}


//{{{ cut
typedef enum
{
	kP2SetRequestNormalListCutIxPiid,
	kP2SetRequestNormalListCutIxSequenceOfOadData,
	kP2SetRequestNormalListCutNum				
} P2SetRequestNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SetRequestNormalListCutNum];

	// sub
	P2OadDataPcut oad_data_cut;
	P2SequenceOfVarLenPcut sequence_of_oad_data_cut;
} P2SetRequestNormalListPcut;
#define kP2SetRequestNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2OadDataPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2SetRequestNormalListPcutOpen(P2SetRequestNormalListPcut *m);
cp_t P2SetRequestNormalListPcutClose(P2SetRequestNormalListPcut *m);
cp_t P2SetRequestNormalListPcutValid(const P2SetRequestNormalListPcut *m);

cp_t P2SetRequestNormalListPcutOpenBase(Pcut *base);
cp_t P2SetRequestNormalListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SetRequestNormalListTest(QTEST_ARG);
#endif

