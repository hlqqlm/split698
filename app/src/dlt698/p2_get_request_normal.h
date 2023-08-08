#ifndef __DLT698_45_GET_REQUEST_NORMAL_H_ 
#define __DLT698_45_GET_REQUEST_NORMAL_H_ 
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
#define P2_GRN_PIID_OFFSET		(0)
#define P2_GRN_PIID_SIZE		(1)

#define P2_GRN_OAD_OFFSET		(P2_GRN_PIID_OFFSET + P2_GRN_PIID_SIZE)
#define P2_GRN_OAD_SIZE			(sizeof(OadT))

#define P2_GRN_WHOLE_SIZE		(P2_GRN_OAD_OFFSET + P2_GRN_OAD_SIZE)

// piid
uint8_t P2GetRequestNormalPiid(const char *whole);

// oad
OadT P2GetRequestNormalOad(const char *whole);



//{{{ part
typedef enum
{
	kP2GetRequestNormalPartIxPiid,
	kP2GetRequestNormalPartIxOad,
	kP2GetRequestNormalPartNum				
} P2GetRequestNormalPartIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetRequestNormalPartNum];
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

