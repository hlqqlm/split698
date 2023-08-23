#ifndef __P2_OAD_DATA_OAD_UNSIGNED_H_
#define __P2_OAD_DATA_OAD_UNSIGNED_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"




/*
为了描述SetThenGetRequestNormalList中的sequence of，先描述oad + data + oad + unsigned

 {
 一个设置的对象属性 OAD，
 数据 Data，
 一个读取的对象属性 OAD，
 延时读取时间 unsigned
  }

*/
#define kP2OadDataOadUnsignedName				"oad_data"
#define kP2OadDataOadUnsignedNameWriteoad		"write_oad"
#define kP2OadDataOadUnsignedNameData			"data"
#define kP2OadDataOadUnsignedNameReadoad		"read_oad"
#define kP2OadDataOadUnsignedNameUnsigned		"unsigned"


// basic information
#define kP2OadDataOadUnsignedWriteoadOffset			(0)
#define kP2OadDataOadUnsignedWriteoadSize			(OAD_MEM_SIZE)

#define kP2OadDataOadUnsignedDataOffset				(kP2OadDataOadUnsignedWriteoadOffset + kP2OadDataOadUnsignedWriteoadSize)
#define kP2OadDataOadUnsignedDataSize(_data_size)	(_data_size)

#define kP2OadDataOadUnsignedReadoadOffset(_data_size)	(kP2OadDataOadUnsignedDataOffset + kP2OadDataOadUnsignedDataSize(_data_size))
#define kP2OadDataOadUnsignedReadoadSize				(OAD_MEM_SIZE)

#define kP2OadDataOadUnsignedUnsignedOffset(_data_size)	(kP2OadDataOadUnsignedReadoadOffset(_data_size) + kP2OadDataOadUnsignedReadoadSize)
#define kP2OadDataOadUnsignedUnsignedSize			(kDlt698UnsignedSize)


//{{{ cut
typedef enum
{
	kP2OadDataOadUnsignedCutIxWriteoad,
	kP2OadDataOadUnsignedCutIxData,
	kP2OadDataOadUnsignedCutIxReadoad,
	kP2OadDataOadUnsignedCutIxUnsigned,
	kP2OadDataOadUnsignedCutNum				
} P2OadDataOadUnsignedCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2OadDataOadUnsignedCutNum];

	P2DataChoicePcut data_cut;
} P2OadDataOadUnsignedPcut;
#define kP2OadDataOadUnsignedPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2DataChoicePcutDef	\
}

cp_t P2OadDataOadUnsignedPcutOpen(P2OadDataOadUnsignedPcut *m);
cp_t P2OadDataOadUnsignedPcutClose(P2OadDataOadUnsignedPcut *m);
cp_t P2OadDataOadUnsignedPcutValid(const P2OadDataOadUnsignedPcut *m);

cp_t P2OadDataOadUnsignedPcutOpenBase(Pcut *base);
cp_t P2OadDataOadUnsignedPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2OadDataOadUnsignedTest(QTEST_ARG);
#endif

