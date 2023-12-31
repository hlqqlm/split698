#ifndef __P2_GET_REQUEST_NORMAL_H_ 
#define __P2_GET_REQUEST_NORMAL_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"


#define kP2GetRequestNormalName		"get_request_normal"

/*
GetRequestNormal∷=SEQUENCE
{
 服务序号-优先级 PIID，
 一个对象属性描述符 OAD
}
*/
#define kP2GetRequestNormalPiidOffset		(0)
#define kP2GetRequestNormalPiidSize			(1)

#define kP2GetRequestNormalOadOffset		(kP2GetRequestNormalPiidOffset + kP2GetRequestNormalPiidSize)
#define kP2GetRequestNormalOadSize			(OAD_MEM_SIZE)

#define kP2GetRequestNormalWholeSize		(kP2GetRequestNormalOadOffset + kP2GetRequestNormalOadSize)

// piid
uint8_t P2GetRequestNormalPiid(const char *whole);

// oad
OadT P2GetRequestNormalOad(const char *whole);



//{{{ cut
typedef enum
{
	kP2GetRequestNormalCutIxPiid,
	kP2GetRequestNormalCutIxOad,
	kP2GetRequestNormalCutNum				
} P2GetRequestNormalCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetRequestNormalCutNum];
} P2GetRequestNormalPcut;
#define kP2GetRequestNormalPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2GetRequestNormalPcutOpen(P2GetRequestNormalPcut *m);
cp_t P2GetRequestNormalPcutClose(P2GetRequestNormalPcut *m);
cp_t P2GetRequestNormalPcutValid(const P2GetRequestNormalPcut *m);

cp_t P2GetRequestNormalPcutOpenBase(Pcut *base);
cp_t P2GetRequestNormalPcutCloseBase(Pcut *base);

//}}}


//{{{ value
typedef struct 
{
	uint8_t piid;
	OadT oad;
} P2GetRequestNormalValue;
#define kP2GetRequestNormalValueDef { 0, 0 }
cp_t P2GetRequestNormalPcutValue(P2GetRequestNormalValue *value, const char *whole);
//}}}

#if 0
//{{{ qpack
typedef enum {
	kP2GetRequestNormalPackIxPiid,
	kP2GetRequestNormalPackIxOad,
	kP2GetRequestNormalPackNum
} P2GetRequestNormalPackIxT;
typedef struct
{
	Qpack base;
	QpackItem items[kP2GetRequestNormalPackNum];

	P2GetRequestNormalValue value;
} P2GetRequestNormalQpack;
#define kP2GetRequestNormalQpackDef {									\
	kQpackDef, { kQpackItemDef },		\
	kP2GetRequestNormalValueDef,		\
}
cp_t P2GetRequestNormalQpackOpen(P2GetRequestNormalQpack *m);
cp_t P2GetRequestNormalQpackClose(P2GetRequestNormalQpack *m);
cp_t P2GetRequestNormalQpackValid(const P2GetRequestNormalQpack *m);
//cp_t P2GetRequestNormalQpackSetValue(P2ChoiceQpack *m, uint8_t piid);
//cp_t P2GetRequestNormalQpackSetOad(P2ChoiceQpack *m, OadT oad);
//}}}
#endif

// test
cp_t P2GetRequestNormalTest(QTEST_ARG);
#endif

