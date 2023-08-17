#ifndef __P2_SET_RESPONSE_NORMAL_H_ 
#define __P2_SET_RESPONSE_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_dar.h"


#define kP2SetResponseNormalName				"set_response_normal"
#define kP2SetResponseNormalNamePiidAcd			"piid-acd"
#define kP2SetResponseNormalNameOad				"oad"
#define kP2SetResponseNormalNameDar				"dar"





//{{{ base
/*
SetResponseNormal∷=SEQUENCE
{
 服务序号-优先级-ACD PIID-ACD，
 一个对象属性描述符 OAD，
  设置执行结果 DAR
  }
*/

// basic information
#define kP2SetResponseNormalPiidAcdOffset		(0)
#define kP2SetResponseNormalPiidAcdSize			(1)

#define kP2SetResponseNormalOadOffset	(kP2SetResponseNormalPiidAcdOffset + kP2SetResponseNormalPiidAcdSize)
#define kP2SetResponseNormalOadSize		(OAD_MEM_SIZE)

#define kP2SetResponseNormalDarOffset	(kP2SetResponseNormalOadOffset + kP2SetResponseNormalOadSize)
#define kP2SetResponseNormalDarSize		(kP2DarSize)
//}}}


//{{{ cut
typedef enum
{
	kP2SetResponseNormalCutIxPiidAcd,
	kP2SetResponseNormalCutIxOad,
	kP2SetResponseNormalCutIxDar,
	kP2SetResponseNormalCutNum				
} P2SetResponseNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SetResponseNormalCutNum];

	// sub
	P2DarPcut dar_cut;
} P2SetResponseNormalPcut;
#define kP2SetResponseNormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DarPcutDef						\
}

cp_t P2SetResponseNormalPcutOpen(P2SetResponseNormalPcut *m);
cp_t P2SetResponseNormalPcutClose(P2SetResponseNormalPcut *m);
cp_t P2SetResponseNormalPcutValid(const P2SetResponseNormalPcut *m);

cp_t P2SetResponseNormalPcutOpenBase(Pcut *base);
cp_t P2SetResponseNormalPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2SetResponseNormalTest(QTEST_ARG);
#endif

