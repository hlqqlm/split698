#ifndef  __PFILL_REPOSITORY_H__
#define  __PFILL_REPOSITORY_H__

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "pfill.h"


// close函数原型
typedef cp_t (*PfillOpenT)(Pfill *fill);
typedef cp_t (*PfillCloseT)(Pfill *fill);

// 空的close函数，有时候仅仅做登记，不需要调用fill的close
cp_t PfillCloseEmpty(Pfill *fill);

typedef struct 
{
	const char *name;
	Pfill *fill;
	PfillCloseT close;
	bool free_en;			// 销毁时，是否调用qfree()
} PfillRepositoryItem;
#define kPfillRepositoryItemDef		{ "no_name", NULL, PfillClose, false }
cp_t PfillRepositoryItemValid(const PfillRepositoryItem *item);
void PfillRepositoryItemPrint(const PfillRepositoryItem *item);


// 在变化的过程中，打印Item
struct PfillRepositoryS;
typedef void (*PfillRepositoryPrintT)(const struct PfillRepositoryS *fr, const char *action, int ix);	

typedef struct PfillRepositoryS
{
// public:
// private: 
	Qarray array;				// 存放item指针
	PfillRepositoryPrintT print;	// 用来打印array的变化
	int free_cnt;					// 统计调用qfree()次数
} PfillRepository;
#define kPfillRepositoryDef {	\
	kQarrayDef,	PfillRepositoryPrintNothing, 0,\
}

// class
cp_t PfillRepositoryOpen(PfillRepository *m);
cp_t PfillRepositoryClose(PfillRepository *m);
cp_t PfillRepositoryValid(const PfillRepository *m);

// print
void PfillRepositoryPrintNothing(const struct PfillRepositoryS *fr, const char *action, int ix);
void PfillRepositoryPrintNormal(const struct PfillRepositoryS *fr, const char *action, int ix);
void PfillRepositorySetPrint(PfillRepository *m, PfillRepositoryPrintT print);
cp_t PfillRepositoryPrint(const PfillRepository *m);

// array
cp_t PfillRepositoryIxValid(const PfillRepository *m, int ix);
int PfillRepositoryNum(const PfillRepository *m);
const PfillRepositoryItem *PfillRepositoryIxItemConst(const PfillRepository *m, int ix);
// 找名字，返回ix。没找到，返回-1
int PfillRepositoryFindName(const PfillRepository *m, const char *name);

// action
cp_t PfillRepositoryRegister(PfillRepository *m, const char *name, Pfill *fill, PfillCloseT close, bool free_en);
// 逆序调用register的fill指针，且调用qfree
cp_t PfillRepositoryRelease(PfillRepository *m, int ix);
cp_t PfillRepositoryDumpReverse(PfillRepository *m);
Pfill *PfillRepositoryFactory(PfillRepository *m, const char *name);
Pfill *PfillRepositoryFactoryEx(PfillRepository *m, const char *name, const void *def, int size, PfillOpenT open, PfillCloseT close);

// test
cp_t PfillRepositoryTest(QTEST_ARG);
#endif

