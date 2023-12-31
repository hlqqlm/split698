#ifndef __P2_NVAR_H_ 
#define __P2_NVAR_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"


#define kP2NvarOffset	(0)
#define kP2NvarSize(_size)	(_size)

#define kP2NvarWholeSize(_size)	(kP2NvarOffset + kP2NvarSize(_size))


//{{{ cut
typedef struct
{
	Pcut base;		
	PcutItem *items;			// 指向num个PcutItem，动态分配

	int num;
} P2NvarPcut;
#define kP2NvarPcutDef {		\
	kPcutDef, NULL,			\
	0							\
}

// num			元素个数
// sub_cut		用来解析一个元素
// name			元素名称
cp_t P2NvarPcutOpen(P2NvarPcut *m, int num, Pcut *sub_cut, const char *name);
cp_t P2NvarPcutClose(P2NvarPcut *m);
cp_t P2NvarPcutValid(const P2NvarPcut *m);

// set one sub_cut
cp_t P2NvarPcutSetSub(P2NvarPcut *m, int ix, Pcut *sub_cut, const char *name);
int P2NvarPcutNum(const P2NvarPcut *m);
//}}}



// test
cp_t P2NvarTest(QTEST_ARG);
#endif

