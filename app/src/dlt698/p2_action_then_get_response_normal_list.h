#ifndef __P2_ACTION_THEN_GET_RESPONSE_NORMAL_LIST_H_ 
#define __P2_ACTION_THEN_GET_RESPONSE_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_omd_dar_data_optional_aresultnormal.h"
#include "p2_sequence_of_var_len.h"


#define kP2ActionThenGetResponseNormalListName				"action_then_get_response_normal_list"
#define kP2ActionThenGetResponseNormalListNamePiidAcd		"piid_acd"
#define kP2ActionThenGetResponseNormalListNameSequenceOf	"sequence_of_omd_dar_data_optional_aresultnormal"

//{{{ base
/*
ctionThenGetResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  操作若干个对象方法后读取属性的结果 SEQUENCE OF
   {
   一个设置的对象方法描述符 OMD，
   操作执行结果 DAR，
    操作返回数据 Data OPTIONAL，
	 一个对象属性及结果 A-ResultNormal
	 }
	 }
*/
#define kP2ActionThenGetResponseNormalListPiidAcdOffset	(0)
#define kP2ActionThenGetResponseNormalListPiidAcdSize	(kP2PiidAcdSize)

#define kP2ActionThenGetResponseNormalListSequenceOfOffset		(kP2ActionThenGetResponseNormalListPiidAcdOffset + kP2ActionThenGetResponseNormalListPiidAcdSize)
#define kP2ActionThenGetResponseNormalListSequenceOfSize(_sequence_of_size)		(_sequence_of_size)


#define kP2ActionThenGetResponseNormalListWholeSize(_sequence_of_size)	(kP2ActionThenGetResponseNormalListSequenceOfOffset + kP2ActionThenGetResponseNormalListSequenceOfSize(_sequence_of_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ActionThenGetResponseNormalListCutIxPiidAcd,
	kP2ActionThenGetResponseNormalListCutIxSequenceOf,
	kP2ActionThenGetResponseNormalListCutNum				
} P2ActionThenGetResponseNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ActionThenGetResponseNormalListCutNum];

	// sub
	P2OmdDarDataOptionalAresultnormalPcut omd_dar_data_optional_aresultnormal_cut;
	P2SequenceOfVarLenPcut sequence_of_cut;
} P2ActionThenGetResponseNormalListPcut;
#define kP2ActionThenGetResponseNormalListPcutDef {					\
	kPcutDef, { kPcutItemDef }								\
	, kP2OmdDarDataOptionalAresultnormalPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2ActionThenGetResponseNormalListPcutOpen(P2ActionThenGetResponseNormalListPcut *m);
cp_t P2ActionThenGetResponseNormalListPcutClose(P2ActionThenGetResponseNormalListPcut *m);
cp_t P2ActionThenGetResponseNormalListPcutValid(const P2ActionThenGetResponseNormalListPcut *m);

cp_t P2ActionThenGetResponseNormalListPcutOpenBase(Pcut *base);
cp_t P2ActionThenGetResponseNormalListPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ActionThenGetResponseNormalListTest(QTEST_ARG);
#endif

