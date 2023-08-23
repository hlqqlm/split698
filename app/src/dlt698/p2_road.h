#ifndef __P2_ROAD_H_ 
#define __P2_ROAD_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "qdlt698.h"
#include "p2_sequence_of.h"


#define kP2RoadName					"road"
#define kP2RoadNameOad				"oad"
#define kP2RoadNameSequenceOfOad	"sequence_of_oad"

//{{{ base
/*
记录型对象属性描述符ROAD（Record Object Attribute Descriptor）的数据类型定义见表10 。
ROAD∷=SEQUENCE
{
对象属性描述符 OAD，
关联对象属性描述符 SEQUENCE OF OAD
}
*/
#define kP2RoadOadOffset	(0)
#define kP2RoadOadSize		(OAD_MEM_SIZE)

#define kP2RoadSequenceOfOadOffset				(kP2RoadOadOffset + kP2RoadOadSize)
#define kP2RoadSequenceOfOadSize(_soa_size)		(_soa_size)


#define kP2RoadWholeSize(_soa_size)	(kP2RoadSequenceOfOadOffset + kP2RoadSequenceOfOadSize(_soa_size))
//}}}


//{{{ cut
typedef enum
{
	kP2RoadCutIxOad,
	kP2RoadCutIxSequenceOfOad,
	kP2RoadCutNum				
} P2RoadCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2RoadCutNum];

	// sub
	P2SequenceOfPcut sequence_of_oad_cut;
} P2RoadPcut;
#define kP2RoadPcutDef {					\
	kPcutDef, { kPcutItemDef }				\
	, kP2SequenceOfPcutDef					\
}

cp_t P2RoadPcutOpen(P2RoadPcut *m);
cp_t P2RoadPcutClose(P2RoadPcut *m);
cp_t P2RoadPcutValid(const P2RoadPcut *m);

cp_t P2RoadPcutOpenBase(Pcut *base);
cp_t P2RoadPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2RoadTest(QTEST_ARG);
#endif

