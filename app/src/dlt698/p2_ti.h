#ifndef __P2_TI_H_ 
#define __P2_TI_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"
#include "p2_enum.h"


#define kP2TiName			"ti"
#define kP2TiNameUnit		"unit"
#define kP2TiNameInterval	"interval"


//{{{ base
/*
TI∷=SEQUENCE
{
单位 ENUMERATED
{
秒 （0），
分 （1），
时 （2），
日 （3），
月 （4），
 年 （5）
  }，
  间隔值 long-unsigned
  }
*/

// basic information
#define kP2TiUnitOffset			(0)
#define kP2TiUnitSize			(1)

#define kP2TiIntervalOffset		(kP2TiUnitOffset + kP2TiUnitSize)
#define kP2TiIntervalSize		(kDlt698LongUnsignedSize)
//}}}


//{{{ unit
/*
   单位 ENUMERATED
   {
   秒 （0），
   分 （1），
   时 （2），
   日 （3），
   月 （4），
   年 （5）
   }，
*/
typedef enum 
{
	kP2TiUnitSecond = 0,	// 秒 （0），
	kP2TiUnitMinute = 1,	// 分 （1），
	kP2TiUnitHour = 2,		// 时 （2），
	kP2TiUnitDay = 3,		// 日 （3），
	kP2TiUnitMonth = 4,		// 月 （4），
	kP2TiUnitYear = 5,		// 年 （5）
} P2TiUnitEnumT;

typedef enum 
{
	kP2TiUnitIxSecond,		// 
	kP2TiUnitIxMinute,		// 
	kP2TiUnitIxHour,		// 
	kP2TiUnitIxDay,			// 
	kP2TiUnitIxMonth,		// 
	kP2TiUnitIxYear,		// 
	kP2TiUnitNum			// 
} P2TiUnitIxT;
int P2TiUnitEnum2Ix(uint8_t enumv);
uint8_t P2TiUnitEnumIx2Value(int enumv_ix);
cp_t P2TiUnitEnumIxValid(int enumv_ix);
cp_t P2TiUnitEnumValid(uint8_t enumv);
const char *P2TiUnitEnumStr(uint8_t enumv);

const char *P2TiUnitMem(const char *whole);
uint8_t P2TiUnitValue(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2TiCutIxUnit,
	kP2TiCutIxInterval,
	kP2TiCutNum				
} P2TiCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2TiCutNum];

	// sub
	P2EnumPcut enum_unit;
} P2TiPcut;
#define kP2TiPcutDef {				\
	kPcutDef, { kPcutItemDef }		\
	, kP2EnumPcutDef				\
}

cp_t P2TiPcutOpen(P2TiPcut *m);
cp_t P2TiPcutClose(P2TiPcut *m);
cp_t P2TiPcutValid(const P2TiPcut *m);

cp_t P2TiPcutOpenBase(Pcut *base);
cp_t P2TiPcutCloseBase(Pcut *base);
//}}}



// test
cp_t P2TiTest(QTEST_ARG);
#endif

