#ifndef __DLT698_45_GET_REQUEST_NORMAL_LIST_H_ 
#define __DLT698_45_GET_REQUEST_NORMAL_LIST_H_ 
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
#define P2_GRNL_PIID_OFFSET		(0)
#define P2_GRNL_PIID_SIZE		(1)

#define P2_GRNL_SEQUENCE_OAD_OFFSET		(P2_GRNL_PIID_OFFSET + P2_GRNL_PIID_SIZE)
#define P2_GRNL_SEQUENCE_OAD_SIZE(_sequence_oad_size)		(_sequence_oad_size)

#define P2_GRNL_WHOLE_SIZE		(P2_GRNL_SEQUENCE_OAD_OFFSET + P2_GRNL_SEQUENCE_OAD_SIZE)

// piid
uint8_t P2GetRequestNormalListPiid(const char *whole);

// oad


//{{{ part
typedef enum
{
	kP2GetRequestNormalListPartIxPiid,
	kP2GetRequestNormalListPartIxSequenceOfOad,
	kP2GetRequestNormalListPartNum				
} P2GetRequestNormalListPartIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetRequestNormalListPartNum];
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

