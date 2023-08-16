#ifndef __P2_LINK_APDU_H_ 
#define __P2_LINK_APDU_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"

#include "p2_link_apdu_choice.h"

//{{{ base
// 基本信息
/*
LINK-APDU∷=CHOICE
{
预连接请求 [1] LINK-Request，
预连接响应 [129] LINK-Response
}
*/
#define kP2LinkApduChoiceOffset				(0)
#define kP2LinkApduChoiceSize(_choice_size)	(_choice_size)


// choice
uint8_t P2LinkApduChoice(const char *whole);

// var
int P2LinkApduVarSize(const char *whole);
//}}}


//{{{ part
typedef enum
{
	kP2LinkApduPartIxChoice,
	kP2LinkApduPartNum				
} P2LinkApduPartIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2LinkApduPartNum];

	P2LinkApduChoicePcut choice_part;
} P2LinkApduPcut;
#define kP2LinkApduPcutDef {		\
	kPcutDef, { kPcutItemDef },	\
	kP2LinkApduChoicePcutDef		\
}

cp_t P2LinkApduPcutOpen(P2LinkApduPcut *m);
cp_t P2LinkApduPcutClose(P2LinkApduPcut *m);
uint8_t P2LinkApduPcutChoice(P2LinkApduPcut *m);
//}}}


// test
cp_t P2LinkApduTest(QTEST_ARG);
#endif

