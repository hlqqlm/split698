#ifndef __P2_OMD_DATA_H_
#define __P2_OMD_DATA_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"




/*
为了描述ActionRequestNormalList中的sequence of，先描述omd + data

 若干个对象方法 SEQUENCE OF
  {
  一个对象方法描述符 OMD，
   方法参数 Data
    }
*/
#define kP2OmdDataName				"omd_data"
#define kP2OmdDataNameOmd			"omd"
#define kP2OmdDataNameData			"data"


// basic information
#define kP2OmdDataOmdOffset					(0)
#define kP2OmdDataOmdSize					(OMD_MEM_SIZE)

#define kP2OmdDataDataOffset				(kP2OmdDataOmdOffset + kP2OmdDataOmdSize)
#define kP2OmdDataDataSize(_data_size)		(_data_size)




//{{{ cut
typedef enum
{
	kP2OmdDataCutIxOmd,
	kP2OmdDataCutIxData,
	kP2OmdDataCutNum				
} P2OmdDataCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2OmdDataCutNum];

	P2DataChoicePcut data_cut;
} P2OmdDataPcut;
#define kP2OmdDataPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2DataChoicePcutDef	\
}

cp_t P2OmdDataPcutOpen(P2OmdDataPcut *m);
cp_t P2OmdDataPcutClose(P2OmdDataPcut *m);
cp_t P2OmdDataPcutValid(const P2OmdDataPcut *m);

cp_t P2OmdDataPcutOpenBase(Pcut *base);
cp_t P2OmdDataPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2OmdDataTest(QTEST_ARG);
#endif

