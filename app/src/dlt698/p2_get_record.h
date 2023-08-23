#ifndef __P2_GET_RECORD_H_ 
#define __P2_GET_RECORD_H_ 
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_rsd.h"
#include "p2_rcsd.h"

// name
#define kP2GetRecordName			"get_record"
#define kP2GetRecordNameOad			"oad"
#define kP2GetRecordNameRsd			"rsd"
#define kP2GetRecordNameRcsd		"rcsd"


//{{{ base
/*
GetRecord∷=SEQUENCE
{
 对象属性描述符 OAD，
  记录行选择描述符 RSD，
   记录列选择描述符 RCSD
   }
*/

#define kP2GetRecordOadOffset		(0)
#define kP2GetRecordOadSize			(OAD_MEM_SIZE)

#define kP2GetRecordRsdOffset				(kP2GetRecordOadOffset + kP2GetRecordOadSize)
#define kP2GetRecordRsdSize(_rsd_size)		(_rsd_size)

#define kP2GetRecordRcsdOffset(_rsd_size)	(kP2GetRecordRsdOffset + kP2GetRecordRsdSize(_rsd_size))
#define kP2GetRecordRcsdSize(_rcsd_size)	(_rcsd_size)

//}}}


//{{{ cut
typedef enum
{
	kP2GetRecordCutIxOad,
	kP2GetRecordCutIxRsd,
	kP2GetRecordCutIxRcsd,
	kP2GetRecordCutNum				
} P2GetRecordCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2GetRecordCutNum];

	P2RsdPcut rsd_cut;
	P2RcsdPcut rcsd_cut;
} P2GetRecordPcut;
#define kP2GetRecordPcutDef {									\
	kPcutDef, { kPcutItemDef }, kP2RsdPcutDef, kP2RcsdPcutDef	\
}

cp_t P2GetRecordPcutOpen(P2GetRecordPcut *m);
cp_t P2GetRecordPcutClose(P2GetRecordPcut *m);
cp_t P2GetRecordPcutValid(const P2GetRecordPcut *m);

cp_t P2GetRecordPcutOpenBase(Pcut *base);
cp_t P2GetRecordPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2GetRecordTest(QTEST_ARG);
#endif

