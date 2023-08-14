#ifndef __P2_NFIX_H_ 
#define __P2_NFIX_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"


#define P2_NFIX_OFFSET	(0)
#define P2_NFIX_SIZE(_num, _one_size)	((_num) * (_one_size))

#define P2_NFIX_WHOLE_SIZE(_num, _one_size)	(P2_NFIX_OFFSET + P2_NFIX_SIZE(_num, _one_size))



//{{{ part
typedef struct
{
	Pcut base;		
	PcutItem *items;			// 指向num个PcutItem，动态分配

	int num;
	int one_size;
} P2NfixPcut;
#define kP2NfixPcutDef {		\
	kPcutDef, NULL,				\
	0, 0						\
}

cp_t P2NfixPcutOpen(P2NfixPcut *m, int num, int one_size, const char *name);
cp_t P2NfixPcutClose(P2NfixPcut *m);
cp_t P2NfixPcutValid(const P2NfixPcut *m);
//}}}


// test
cp_t P2NfixTest(QTEST_ARG);
#endif

