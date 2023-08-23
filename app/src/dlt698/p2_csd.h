#ifndef __P2_CSD_H_ 
#define __P2_CSD_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "qdlt698.h"
#include "p2_choice.h"

// name
#define kP2CsdNameChoice	"csd_choice"
#define kP2CsdNameVar		"csd_var"


// choice
/*
CSD∷=CHOICE
{
对象属性描述符 [0] OAD，
记录型对象属性描述符 [1] ROAD
}
*/
typedef enum 
{
	kP2CsdOad = 0,	// 对象属性描述符 [0] OAD，
	kP2CsdRoad = 1,	// 记录型对象属性描述符 [1] ROAD
} P2CsdT;

typedef enum 
{
	kP2CsdIxOad,	// 
	kP2CsdIxRoad,	// 
	kP2CsdNum
} P2CsdIxT;
int P2Csd2Ix(uint8_t choice);
uint8_t P2CsdIx2Value(int choice_ix);
cp_t P2CsdIxValid(int choice_ix);
cp_t P2CsdValid(uint8_t choice);
const char *P2CsdStr(uint8_t choice);
//


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2CsdPcut;
#define kP2CsdPcutDef {	kP2ChoicePcutDef, }

cp_t P2CsdPcutOpen(P2CsdPcut *m);
cp_t P2CsdPcutClose(P2CsdPcut *m);
cp_t P2CsdPcutValid(const P2CsdPcut *m);
//}}}


// test
cp_t P2CsdTest(QTEST_ARG);
#endif

