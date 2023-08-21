#ifndef __P2_ACTION_RESPONSE_NORMAL_LIST_H_ 
#define __P2_ACTION_RESPONSE_NORMAL_LIST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
//#include "p2_dar.h"
//#include "p2_optional.h"
#include "p2_omd_dar_data_optional.h"
#include "p2_sequence_of_var_len.h"


#define kP2ActionResponseNormalListName				"action_response_normal_list"
#define kP2ActionResponseNormalListNamePiidAcd		"piid-acd"
#define kP2ActionResponseNormalListNameSequenceOfOmdDarDataOptional			"sequence_of_omd_dar_data_optional"





//{{{ base
/*
ActionResponseNormalList∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
  若干个对象方法操作结果 SEQUENCE OF
   {
   一个对象方法描述符 OMD，
   操作执行结果 DAR，
   操作返回数据 Data OPTIONAL
   }
   }
*/

// basic information
#define kP2ActionResponseNormalListPiidAcdOffset		(0)
#define kP2ActionResponseNormalListPiidAcdSize			(1)

#define kP2ActionResponseNormalListSequenceOfOmdDarDataOptionalOffset	(kP2ActionResponseNormalListPiidAcdOffset + kP2ActionResponseNormalListPiidAcdSize)
#define kP2ActionResponseNormalListSequenceOfOmdDarDataOptionalSize(_sooddo_size)		(_sooddo_size)
//}}}


//{{{ cut
typedef enum
{
	kP2ActionResponseNormalListCutIxPiidAcd,
	kP2ActionResponseNormalListCutIxSequenceOfOmdDarDataOptional,
	kP2ActionResponseNormalListCutNum				
} P2ActionResponseNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ActionResponseNormalListCutNum];

	// sub
	P2OmdDarDataOptionalPcut omd_dar_data_optional_cut;
	P2SequenceOfVarLenPcut sequence_of_omd_dar_data_optional_cut;
	// } P2ActionRequestNormalListPcut;

	//P2DarPcut dar_cut;

	//P2OptionalPcut optional_data_cut;
	//P2DataChoicePcut data_cut;
} P2ActionResponseNormalListPcut;
#define kP2ActionResponseNormalListPcutDef {						\
	kPcutDef, { kPcutItemDef }										\
	, kP2OmdDarDataOptionalPcutDef, kP2SequenceOfVarLenPcutDef		\
}

cp_t P2ActionResponseNormalListPcutOpen(P2ActionResponseNormalListPcut *m);
cp_t P2ActionResponseNormalListPcutClose(P2ActionResponseNormalListPcut *m);
cp_t P2ActionResponseNormalListPcutValid(const P2ActionResponseNormalListPcut *m);

cp_t P2ActionResponseNormalListPcutOpenBase(Pcut *base);
cp_t P2ActionResponseNormalListPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2ActionResponseNormalListTest(QTEST_ARG);
#endif

