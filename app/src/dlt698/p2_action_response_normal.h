#ifndef __P2_ACTION_RESPONSE_NORMAL_H_ 
#define __P2_ACTION_RESPONSE_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_dar.h"
#include "p2_optional.h"
#include "p2_data_choice.h"


#define kP2ActionResponseNormalName				"action_response_normal"
#define kP2ActionResponseNormalNamePiidAcd		"piid-acd"
#define kP2ActionResponseNormalNameOmd			"omd"
#define kP2ActionResponseNormalNameDar			"dar"
#define kP2ActionResponseNormalNameOptionalData	"data_optional"





//{{{ base
/*
ActionResponseNormal∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 一个对象属性描述符 OAD，
  设置执行结果 DAR
  }
*/

// basic information
#define kP2ActionResponseNormalPiidAcdOffset		(0)
#define kP2ActionResponseNormalPiidAcdSize			(1)

#define kP2ActionResponseNormalOmdOffset	(kP2ActionResponseNormalPiidAcdOffset + kP2ActionResponseNormalPiidAcdSize)
#define kP2ActionResponseNormalOmdSize		(OMD_MEM_SIZE)

#define kP2ActionResponseNormalDarOffset	(kP2ActionResponseNormalOmdOffset + kP2ActionResponseNormalOmdSize)
#define kP2ActionResponseNormalDarSize		(kP2DarSize)

#define kP2ActionResponseNormalOptionalDataOffset	(kP2ActionResponseNormalDarOffset + kP2ActionResponseNormalDarSize)
#define kP2ActionResponseNormalOptionalDataSize(_data_optional_size)		(_data_optional_size)
//}}}


//{{{ cut
typedef enum
{
	kP2ActionResponseNormalCutIxPiidAcd,
	kP2ActionResponseNormalCutIxOmd,
	kP2ActionResponseNormalCutIxDar,
	kP2ActionResponseNormalCutIxOptionalData,
	kP2ActionResponseNormalCutNum				
} P2ActionResponseNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ActionResponseNormalCutNum];

	// sub
	P2DarPcut dar_cut;

	P2OptionalPcut optional_data_cut;
	P2DataChoicePcut data_cut;
} P2ActionResponseNormalPcut;
#define kP2ActionResponseNormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DarPcutDef, kP2OptionalPcutDef, kP2DataChoicePcutDef			\
}

cp_t P2ActionResponseNormalPcutOpen(P2ActionResponseNormalPcut *m);
cp_t P2ActionResponseNormalPcutClose(P2ActionResponseNormalPcut *m);
cp_t P2ActionResponseNormalPcutValid(const P2ActionResponseNormalPcut *m);

cp_t P2ActionResponseNormalPcutOpenBase(Pcut *base);
cp_t P2ActionResponseNormalPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2ActionResponseNormalTest(QTEST_ARG);
#endif

