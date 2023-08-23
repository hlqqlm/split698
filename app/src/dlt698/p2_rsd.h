#ifndef __P2_RSD_H_ 
#define __P2_RSD_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_choice.h"

// name
#define kP2RsdNameChoice	"rsd_choice"
#define kP2RsdNameVar		"rsd_var"


// choice
/*
RSD∷=CHOICE
{
不选择 [0] NULL，
选择方法 1 [1] Selector1，
选择方法 2 [2] Selector2，
选择方法 3 [3] Selector3，
选择方法 4 [4] Selector4，
选择方法 5 [5] Selector5，
选择方法 6 [6] Selector6，
选择方法 7 [7] Selector7，
选择方法 8 [8] Selector8，
选择方法 9 [9] Selector9，
选择方法 10 [10] Selector10，
选择方法 11 [11] Selector11
}
*/
typedef enum 
{
	kP2RsdNull = 0,	// 不选择 [0] NULL，
	kP2RsdSelector1 = 1,	// 选择方法 1 [1] Selector1，
	kP2RsdSelector2 = 2,	// 选择方法 2 [2] Selector2，
	kP2RsdSelector3 = 3,	// 选择方法 3 [3] Selector3，
	kP2RsdSelector4 = 4,	// 选择方法 4 [4] Selector4，
	kP2RsdSelector5 = 5,	// 选择方法 5 [5] Selector5，
	kP2RsdSelector6 = 6,	// 选择方法 6 [6] Selector6，
	kP2RsdSelector7 = 7,	// 选择方法 7 [7] Selector7，
	kP2RsdSelector8 = 8,	// 选择方法 8 [8] Selector8，
	kP2RsdSelector9 = 9,	// 选择方法 9 [9] Selector9，
	kP2RsdSelector10 = 10,	// 选择方法 10 [10] Selector10，
	kP2RsdSelector11 = 11,	// 选择方法 11 [11] Selector11
} P2RsdT;

typedef enum 
{
	kP2RsdIxNull,	// 不选择 [0] NULL，
	kP2RsdIxSelector1,	// 选择方法 1 [1] Selector1，
	kP2RsdIxSelector2,	// 选择方法 2 [2] Selector2，
	kP2RsdIxSelector3,	// 选择方法 3 [3] Selector3，
	kP2RsdIxSelector4,	// 选择方法 4 [4] Selector4，
	kP2RsdIxSelector5,	// 选择方法 5 [5] Selector5，
	kP2RsdIxSelector6,	// 选择方法 6 [6] Selector6，
	kP2RsdIxSelector7,	// 选择方法 7 [7] Selector7，
	kP2RsdIxSelector8,	// 选择方法 8 [8] Selector8，
	kP2RsdIxSelector9,	// 选择方法 9 [9] Selector9，
	kP2RsdIxSelector10,	// 选择方法 10 [10] Selector10，
	kP2RsdIxSelector11,	// 选择方法 11 [11] Selector11
	kP2RsdNum
} P2RsdIxT;
int P2Rsd2Ix(uint8_t choice);
uint8_t P2RsdIx2Value(int choice_ix);
cp_t P2RsdIxValid(int choice_ix);
cp_t P2RsdValid(uint8_t choice);
const char *P2RsdStr(uint8_t choice);
//


//{{{ cut
typedef struct
{
	P2ChoicePcut choice;
} P2RsdPcut;
#define kP2RsdPcutDef {	kP2ChoicePcutDef, }

cp_t P2RsdPcutOpen(P2RsdPcut *m);
cp_t P2RsdPcutClose(P2RsdPcut *m);
cp_t P2RsdPcutValid(const P2RsdPcut *m);
//}}}


// test
cp_t P2RsdTest(QTEST_ARG);
#endif

