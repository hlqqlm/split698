#ifndef __P2_COMPACT_SET_RESPONSE_NORMAL_H_ 
#define __P2_COMPACT_SET_RESPONSE_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_dar.h"


#define kP2CompactSetResponseNormalName				"action_response_normal"
#define kP2CompactSetResponseNormalNamePiidAcd		"piid-acd"
#define kP2CompactSetResponseNormalNameOmd			"omd"
#define kP2CompactSetResponseNormalNameDar			"dar"
#define kP2CompactSetResponseNormalNameOptionalData	"data_optional"





//{{{ base
/*
CompactSetResponseNormal∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 一个对象属性描述符 OAD，
  设置执行结果 DAR
  }
*/

// basic information
#define kP2CompactSetResponseNormalPiidAcdOffset		(0)
#define kP2CompactSetResponseNormalPiidAcdSize			(1)

#define kP2CompactSetResponseNormalDarOffset	(kP2CompactSetResponseNormalPiidAcdOffset + kP2CompactSetResponseNormalPiidAcdSize)
#define kP2CompactSetResponseNormalDarSize		(kP2DarSize)
//}}}


//{{{ cut
typedef enum
{
	kP2CompactSetResponseNormalCutIxPiidAcd,
	kP2CompactSetResponseNormalCutIxDar,
	kP2CompactSetResponseNormalCutNum				
} P2CompactSetResponseNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2CompactSetResponseNormalCutNum];

	// sub
	P2DarPcut dar_cut;
} P2CompactSetResponseNormalPcut;
#define kP2CompactSetResponseNormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DarPcutDef \
}

cp_t P2CompactSetResponseNormalPcutOpen(P2CompactSetResponseNormalPcut *m);
cp_t P2CompactSetResponseNormalPcutClose(P2CompactSetResponseNormalPcut *m);
cp_t P2CompactSetResponseNormalPcutValid(const P2CompactSetResponseNormalPcut *m);

cp_t P2CompactSetResponseNormalPcutOpenBase(Pcut *base);
cp_t P2CompactSetResponseNormalPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2CompactSetResponseNormalTest(QTEST_ARG);
#endif

