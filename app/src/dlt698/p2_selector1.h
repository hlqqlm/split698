#ifndef __P2_SELECTOR1_H_
#define __P2_SELECTOR1_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"




/*
Selector1∷=SEQUENCE
{
 对象属性描述符 OAD，
  数值 Data
  }
*/
#define kP2Selector1Name			"selector1"
#define kP2Selector1NameOad			"oad"
#define kP2Selector1NameData		"data"


// basic information
#define kP2Selector1OadOffset				(0)
#define kP2Selector1OadSize					(OAD_MEM_SIZE)

#define kP2Selector1DataOffset				(kP2Selector1OadOffset + kP2Selector1OadSize)
#define kP2Selector1DataSize(_data_size)	(_data_size)




//{{{ cut
typedef enum
{
	kP2Selector1CutIxOad,
	kP2Selector1CutIxData,
	kP2Selector1CutNum				
} P2Selector1CutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2Selector1CutNum];

	P2DataChoicePcut data_cut;
} P2Selector1Pcut;
#define kP2Selector1PcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2DataChoicePcutDef	\
}

cp_t P2Selector1PcutOpen(P2Selector1Pcut *m);
cp_t P2Selector1PcutClose(P2Selector1Pcut *m);
cp_t P2Selector1PcutValid(const P2Selector1Pcut *m);

cp_t P2Selector1PcutOpenBase(Pcut *base);
cp_t P2Selector1PcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2Selector1Test(QTEST_ARG);
#endif

