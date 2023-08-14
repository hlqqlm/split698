#ifndef __P2_ACTION_REQUEST_NORMAL_H_ 
#define __P2_ACTION_REQUEST_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"


#define kP2ActionRequestNormalName				"action_request_normal"
#define kP2ActionRequestNormalNamePiid			"piid"
#define kP2ActionRequestNormalNameOmd			"omd"
#define kP2ActionRequestNormalNameData			"data"

//{{{ base
/*
ActionRequestNormal∷=SEQUENCE
{
 服务序号-优先级 PIID，
  一个对象方法描述符 OMD，
   方法参数 Data
   }
*/
// Arn = Action Request Normal
#define kP2ActionRequestNormalPiidOffset	(0)
#define kP2ActionRequestNormalPiidSize		(kP2PiidSize)

#define kP2ActionRequestNormalOmdOffset		(kP2ActionRequestNormalPiidOffset + kP2ActionRequestNormalPiidSize)
#define kP2ActionRequestNormalOmdSize		(OMD_MEM_SIZE)

#define kP2ActionRequestNormalDataOffset	(kP2ActionRequestNormalOmdOffset + kP2ActionRequestNormalOmdSize)
#define kP2ActionRequestNormalDataSize(_data_size)		(_data_size)

#define kP2ActionRequestNormalWholeSize(_data_size)	(kP2ActionRequestNormalDataOffset + kP2ActionRequestNormalDataSize(_data_size))
//}}}


//{{{ cut
typedef enum
{
	kP2ActionRequestNormalCutIxPiid,
	kP2ActionRequestNormalCutIxOmd,
	kP2ActionRequestNormalCutIxData,
	kP2ActionRequestNormalCutNum				
} P2ActionRequestNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ActionRequestNormalCutNum];

	// sub
	P2DataChoicePcut data;
} P2ActionRequestNormalPcut;
#define kP2ActionRequestNormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DataChoicePcutDef						\
}

cp_t P2ActionRequestNormalPcutOpen(P2ActionRequestNormalPcut *m);
cp_t P2ActionRequestNormalPcutClose(P2ActionRequestNormalPcut *m);
cp_t P2ActionRequestNormalPcutValid(const P2ActionRequestNormalPcut *m);

cp_t P2ActionRequestNormalPcutOpenBase(Pcut *base);
cp_t P2ActionRequestNormalPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ActionRequestNormalTest(QTEST_ARG);
#endif

