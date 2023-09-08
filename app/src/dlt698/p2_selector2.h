#ifndef __P2_SELECTOR2_H_
#define __P2_SELECTOR2_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_data_choice.h"




/*
Selector2∷=SEQUENCE
{
 对象属性描述符 OAD，
  起始值 Data，
   结束值 Data，
    数据间隔 Data
	}
*/
#define kP2Selector2Name			"selector2"
#define kP2Selector2NameOad			"oad"
#define kP2Selector2NameStart		"start"
#define kP2Selector2NameEnd			"end"
#define kP2Selector2NameInterval	"interval"


// basic information
#define kP2Selector2OadOffset				(0)
#define kP2Selector2OadSize					(OAD_MEM_SIZE)

#define kP2Selector2StartOffset				(kP2Selector2OadOffset + kP2Selector2OadSize)




//{{{ cut
typedef enum
{
	kP2Selector2CutIxOad,
	kP2Selector2CutIxStart,
	kP2Selector2CutIxEnd,
	kP2Selector2CutIxInterval,
	kP2Selector2CutNum				
} P2Selector2CutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2Selector2CutNum];

	P2DataChoicePcut start;
	P2DataChoicePcut end;
	P2DataChoicePcut interval;
} P2Selector2Pcut;
#define kP2Selector2PcutDef {				\
	kPcutDef, { kPcutItemDef }, kP2DataChoicePcutDef, kP2DataChoicePcutDef, kP2DataChoicePcutDef	\
}

cp_t P2Selector2PcutOpen(P2Selector2Pcut *m);
cp_t P2Selector2PcutClose(P2Selector2Pcut *m);
cp_t P2Selector2PcutValid(const P2Selector2Pcut *m);

cp_t P2Selector2PcutOpenBase(Pcut *base);
cp_t P2Selector2PcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2Selector2Test(QTEST_ARG);
#endif

