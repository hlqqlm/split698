#ifndef __P2_OAD_DATA_H_
#define __P2_OAD_DATA_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"




/*
为了描述SetRequestNormalList中的sequence of，先描述oad + data

SetRequestNormalList∷=SEQUENCE
{
 服务序号-优先级 PIID，
  若干个对象属性 SEQUENCE OF
   {
   一个对象属性描述符 OAD，
    数据 Data
	 }
	 }
*/
#define kP2OadDataName				"oad_data"
#define kP2OadDataNameOad			"oad"
#define kP2OadDataNameData			"data"


// basic information
#define kP2OadDataOadOffset					(0)
#define kP2OadDataOadSize					(OAD_MEM_SIZE)

#define kP2OadDataDataOffset				(kP2OadDataOadOffset + kP2OadDataOadSize)
#define kP2OadDataDataSize(_data_size)		(_data_size)




//{{{ cut
typedef enum
{
	kP2OadDataCutIxOad,
	kP2OadDataCutIxData,
	kP2OadDataCutNum				
} P2OadDataCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2OadDataCutNum];

	P2DataChoicePcut data_cut;
} P2OadDataPcut;
#define kP2OadDataPcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2DataChoicePcutDef	\
}

cp_t P2OadDataPcutOpen(P2OadDataPcut *m);
cp_t P2OadDataPcutClose(P2OadDataPcut *m);
cp_t P2OadDataPcutValid(const P2OadDataPcut *m);

cp_t P2OadDataPcutOpenBase(Pcut *base);
cp_t P2OadDataPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2OadDataTest(QTEST_ARG);
#endif

