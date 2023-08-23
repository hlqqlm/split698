#ifndef __P2_SET_REQUEST_NORMAL_H_ 
#define __P2_SET_REQUEST_NORMAL_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"


#define kP2SetRequestNormalName				"set_request_normal"
#define kP2SetRequestNormalNamePiid			"piid"
#define kP2SetRequestNormalNameOad			"oad"
#define kP2SetRequestNormalNameData			"data"

//{{{ base
/*
SetRequestNormal∷=SEQUENCE
{
 服务序号-优先级 PIID，
  一个对象方法描述符 OAD,
   方法参数 Data
   }
*/
// Arn = Set Request Normal
#define kP2SetRequestNormalPiidOffset	(0)
#define kP2SetRequestNormalPiidSize		(kP2PiidSize)

#define kP2SetRequestNormalOadOffset	(kP2SetRequestNormalPiidOffset + kP2SetRequestNormalPiidSize)
#define kP2SetRequestNormalOadSize		(OAD_MEM_SIZE)

#define kP2SetRequestNormalDataOffset	(kP2SetRequestNormalOadOffset + kP2SetRequestNormalOadSize)
#define kP2SetRequestNormalDataSize(_data_size)		(_data_size)

#define kP2SetRequestNormalWholeSize(_data_size)	(kP2SetRequestNormalDataOffset + kP2SetRequestNormalDataSize(_data_size))
//}}}


//{{{ cut
typedef enum
{
	kP2SetRequestNormalCutIxPiid,
	kP2SetRequestNormalCutIxOad,
	kP2SetRequestNormalCutIxData,
	kP2SetRequestNormalCutNum				
} P2SetRequestNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2SetRequestNormalCutNum];

	// sub
	P2DataChoicePcut data;
} P2SetRequestNormalPcut;
#define kP2SetRequestNormalPcutDef {			\
	kPcutDef, { kPcutItemDef }					\
	, kP2DataChoicePcutDef						\
}

cp_t P2SetRequestNormalPcutOpen(P2SetRequestNormalPcut *m);
cp_t P2SetRequestNormalPcutClose(P2SetRequestNormalPcut *m);
cp_t P2SetRequestNormalPcutValid(const P2SetRequestNormalPcut *m);

cp_t P2SetRequestNormalPcutOpenBase(Pcut *base);
cp_t P2SetRequestNormalPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2SetRequestNormalTest(QTEST_ARG);
#endif

