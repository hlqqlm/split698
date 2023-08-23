#ifndef __P2_GET_REQUEST_NORMAL_LIST_H_ 
#define __P2_GET_REQUEST_NORMAL_LIST_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_sequence_of.h"


#define kP2GetRequestNormalListName		"get_request_normal_list"

/*
GetRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
 若干个对象属性描述符 SEQUENCE OF OAD
}
*/
// GRNL = GetRequestNormalList
#define kP2GetRequestNormalListPiidOffset		(0)
#define kP2GetRequestNormalListPiidSize		(1)

#define kP2GetRequestNormalListSequenceOfOadOffset						(kP2GetRequestNormalListPiidOffset + kP2GetRequestNormalListPiidSize)
#define kP2GetRequestNormalListSequenceOfOadSize(_sequence_oad_size)	(_sequence_oad_size)

#define kP2GetRequestNormalListWholeSize(_sequence_of_oad_size)		(kP2GetRequestNormalListSequenceOfOadOffset + kP2GetRequestNormalListSequenceOfOadSize(_sequence_of_oad_size))

// piid
uint8_t P2GetRequestNormalListPiid(const char *whole);

// oad


//{{{ cut
typedef enum
{
	kP2GetRequestNormalListCutIxPiid,
	kP2GetRequestNormalListCutIxSequenceOfOad,
	kP2GetRequestNormalListCutNum				
} P2GetRequestNormalListCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetRequestNormalListCutNum];
	P2SequenceOfPcut sequence_of_oad;
} P2GetRequestNormalListPcut;
#define kP2GetRequestNormalListPcutDef { kPcutDef, { kPcutItemDef }, kP2SequenceOfPcutDef }

cp_t P2GetRequestNormalListPcutOpen(P2GetRequestNormalListPcut *m);
cp_t P2GetRequestNormalListPcutClose(P2GetRequestNormalListPcut *m);
cp_t P2GetRequestNormalListPcutValid(const P2GetRequestNormalListPcut *m);

cp_t P2GetRequestNormalListPcutOpenBase(Pcut *base);
cp_t P2GetRequestNormalListPcutCloseBase(Pcut *base);
//}}}


//{{{ value
#define kP2GetRequestNormalListOadNumMax		(10)
typedef struct
{
	int num;
	OadT list[kP2GetRequestNormalListOadNumMax];
} P2GetRequestNormalListOadValue;
#define kP2GetRequestNormalListOadValueDef { 0, {0} }
typedef struct 
{
	uint8_t piid;
	P2GetRequestNormalListOadValue oad;
} P2GetRequestNormalListValue;
#define kP2GetRequestNormalListValueDef { 0, kP2GetRequestNormalListOadValueDef }
cp_t P2GetRequestNormalListPcutValue(P2GetRequestNormalListValue *value, const char *whole);

void P2GetRequestNormalListPcutValuePrint(const P2GetRequestNormalListValue *value);
//}}}

// test
cp_t P2GetRequestNormalListTest(QTEST_ARG);
#endif

