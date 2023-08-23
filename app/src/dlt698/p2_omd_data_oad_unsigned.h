#ifndef __P2_OMD_DATA_OAD_UNSIGNED_H_
#define __P2_OMD_DATA_OAD_UNSIGNED_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"




/*
为了描述ActionThenGetRequestNormalList中的sequence of，先描述omd + data + oad + unsigned

ActionThenGetRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
  操作若干个对象方法后读取对象属性 SEQUENCE OF
   {
   一个设置的对象方法描述符 OMD，
   方法参数 Data，
   一个读取的对象属性描述符 OAD，
   读取延时 unsigned
    }
	}
*/
#define kP2OmdDataOadUnsignedName				"omd_data"
#define kP2OmdDataOadUnsignedNameOmd			"omd"
#define kP2OmdDataOadUnsignedNameData			"data"
#define kP2OmdDataOadUnsignedNameOad			"oad"
#define kP2OmdDataOadUnsignedNameUnsigned		"unsigned"


// basic information
#define kP2OmdDataOadUnsignedOmdOffset					(0)
#define kP2OmdDataOadUnsignedOmdSize					(OMD_MEM_SIZE)

#define kP2OmdDataOadUnsignedDataOffset				(kP2OmdDataOadUnsignedOmdOffset + kP2OmdDataOadUnsignedOmdSize)
#define kP2OmdDataOadUnsignedDataSize(_data_size)	(_data_size)

#define kP2OmdDataOadUnsignedOadOffset(_data_size)	(kP2OmdDataOadUnsignedDataOffset + kP2OmdDataOadUnsignedDataSize(_data_size))
#define kP2OmdDataOadUnsignedOadSize				(OAD_MEM_SIZE)

#define kP2OmdDataOadUnsignedUnsignedOffset(_data_size)	(kP2OmdDataOadUnsignedOadOffset(_data_size) + kP2OmdDataOadUnsignedOadSize)
#define kP2OmdDataOadUnsignedUnsignedSize			(kDlt698UnsignedSize)


//{{{ cut
typedef enum
{
	kP2OmdDataOadUnsignedCutIxOmd,
	kP2OmdDataOadUnsignedCutIxData,
	kP2OmdDataOadUnsignedCutIxOad,
	kP2OmdDataOadUnsignedCutIxUnsigned,
	kP2OmdDataOadUnsignedCutNum				
} P2OmdDataOadUnsignedCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2OmdDataOadUnsignedCutNum];

	P2DataChoicePcut data_cut;
} P2OmdDataOadUnsignedPcut;
#define kP2OmdDataOadUnsignedPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2DataChoicePcutDef	\
}

cp_t P2OmdDataOadUnsignedPcutOpen(P2OmdDataOadUnsignedPcut *m);
cp_t P2OmdDataOadUnsignedPcutClose(P2OmdDataOadUnsignedPcut *m);
cp_t P2OmdDataOadUnsignedPcutValid(const P2OmdDataOadUnsignedPcut *m);

cp_t P2OmdDataOadUnsignedPcutOpenBase(Pcut *base);
cp_t P2OmdDataOadUnsignedPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2OmdDataOadUnsignedTest(QTEST_ARG);
#endif

