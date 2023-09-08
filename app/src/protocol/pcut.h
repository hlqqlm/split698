#ifndef  __PCUT_H__
#define  __PCUT_H__

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "pdo.h"

//{{{ fix
#define kPcutIxAll		(-1)			// 用来代表所有部分的part index索引号

struct PcutS;
// 为什么需要part? choice后面解析对象part，是根据choice值生成的，因此，要传入解析的cut对象。
int PcutItemLenDef(struct PcutS *part, int ix, const char *whole);
int PcutItemOffsetDef(struct PcutS *part, int ix, const char *whole);
cp_t PcutItemValidDef(struct PcutS *part, int ix, const char *whole);
//cp_t PcutItemValueDef(struct PcutS *part, int ix, const char *whole, void *value, int value_size);

int PcutItemLenBySub(struct PcutS *part, int ix, const char *whole);
// 用前面的长度，计算当前的offset
int PcutItemOffsetByPreviousLen(struct PcutS *part, int ix, const char *whole);
cp_t PcutItemValidBySub(struct PcutS *part, int ix, const char *whole);
//cp_t PcutItemExplainNull(struct PcutS *part, int ix, const char *whole);


// 辅助解析一段报文
typedef int (*PcutItemLenT)(struct PcutS *part, int ix, const char *whole);
// 其实offset可以通过len推算出来，len也可以通过offset推算出来，即offset是前面所有部分len的和。为什么使用独立函数计算？因为实际解帧过程中，offset/len有些简便算法，可以提高计算效率。如果offset通过前面len推算，用PcutItemOffsetByPreviousLen。
typedef int (*PcutItemOffsetT)(struct PcutS *part, int ix, const char *whole);
typedef cp_t (*PcutItemValidT)(struct PcutS *part, int ix, const char *whole);
// 解释性打印。额外打印一些解释信息。例如：long_unsigned，打印出10进制值; datetime_s，打印出2023-7-7 11:52:00
// return: 如果解释打印过程中出错，返回非0
typedef cp_t (*PcutItemExplainT)(struct PcutS *part, int ix, const char *whole);


// typedef cp_t (*PcutItemValueT)(struct PcutS *part, int ix, const char *whole, void *value, int value_size);
// 固定部分
typedef struct 
{
	const char *fix_name;		// 名称
	PcutItemLenT len;			// 长度
	PcutItemOffsetT offset;	// offset可以通过前面部分长度和计算，保留，是因为有一些便利算法，效率更高
	PcutItemValidT valid;		// 有效性
	PcutItemExplainT explain;	// 对当前条目，解释性打印。例如：long_unsigned打印十进制值
	// PcutItemValueT value;
} PcutItemFix;
#define kPcutItemFixDef { "no_name", PcutItemLenDef, PcutItemOffsetDef, PcutItemValidDef, PcutItemExplainNull }
void PcutItemFixPrint(const PcutItemFix *cif);
//}}}


//{{{ item
// 一段报文
typedef struct PcutItemS
{
	// public:
	const PcutItemFix *fix;		// 为了节约内存，const部分集中在一起，从而多占用flash空间，少占用ram
	const char *name;				// choice, 随着其值的不同，对应的名称也要发生变化。
	Pdo *doa;						// 附加动作。用来完成报文数据转换、执行
	//Qchoice *choice;				// 执行选择动作。如果非NULL，表示当前item为choice。
	struct PcutS *sub;				// 子部分，其包含解析函数数组。这一部分可能随着解析，发生变化。例如：解析出命令码后，就可以根据命令码来解析了。当sub=NULL时，表示item为叶子节点，无需解析了。

	// private:
} PcutItem;
#define kPcutItemDef {				\
	NULL, NULL, NULL, NULL		\
}
#define PCUT_ITEM_NO_SUB(_fix) { (_fix), NULL, NULL, NULL }
#define PCUT_ITEM_WITH_SUB(_fix, _sub) { (_fix), NULL, NULL, (_sub) }
void PcutItemPrint(const PcutItem *ci);
//}}}


//{{{ cut
// cut即PcutItem数组，用来辅助解析一串报文成多段
typedef struct PcutS
{
	// public:
	PcutItem *array;
	int len;

	const char *name;				// choice, 随着其值的不同，对应的名称也要发生变化。
	// private:
} Pcut;
#define kPcutDef	{ NULL, 0, NULL }

// 初始化
cp_t PcutOpen(Pcut *m, PcutItem *array, int len);
cp_t PcutClose(Pcut *m);
cp_t PcutValid(const Pcut *m);
// 复制后，array必须存在，不能销毁
void PcutDup(Pcut *m, const Pcut *src);
void PcutPrint(const Pcut *m);

// info
int PcutItemNum(const Pcut *m);

// ix
// 当ix=-1时，表示所有。
// 用-1表示all, 而不是用items总数len来表示，这样就不用记items总数了
cp_t PcutIndexValid(const Pcut *m, int ix);
cp_t PcutIndexAllValid(const Pcut *m, int ix);
const char *PcutIxStr(const Pcut *m, int ix);

// name
cp_t PcutNameSetIx(Pcut *m, int ix, const char *name);

// len
int PcutIxLen(Pcut *m, int verbose, int ix, const char *whole);
// 一段范围内的长度，范围为：[start_ix, end_ix)
int PcutRangeLen(Pcut *m, int verbose, int start_ix, int end_ix, const char *whole);

// offset
int PcutIxOffset(Pcut *m, int verbose, int ix, const char *whole);
char *PcutIxPtr(Pcut *m, int ix, char *whole);						// 非const版本
const char *PcutIxPtrConst(Pcut *m, int ix, const char *whole);		// const版本


// valid
// 检查是否有错误
cp_t PcutIxValid(Pcut *m, int ix, const char *whole);
// 检查帧合法性，返回第一个错误的索引号。全部合法，返回-1
int PcutInvalidIx(Pcut *m, const char *whole);

// value
// cp_t PcutIxValue(Pcut *m, int ix, const char *whole, void *value, int value_size);

// explain
cp_t PcutIxExplain(Pcut *m, int ix, const char *whole);

// do
void PcutDoSet(Pcut *m, int ix, Pdo *doa);
// verbose: 调试时设置 > 0，值越大，打印信息越多
// level: 传入0。调试时用，表示调用层数
cp_t PcutIxDo(Pcut *m, int verbose, int level, int ix, const char *whole);

// sub
bool PcutIxHasSub(const Pcut *m, int ix);
Pcut *PcutSub(Pcut *m, int ix);
void PcutSubSet(Pcut *m, int ix, Pcut *sub, const char *name);
PcutItem *PcutFindItemRecursionDepth(Pcut *m, const char *name);
PcutItem *PcutFindItemRecursionBreadth(Pcut *m, const char *name);
Pcut *PcutFindSubRecursionDepth(Pcut *m, const char *name);
const char *PcutFindMem(Pcut *m, const char *name, const char *whole, PcutItem **item, int *len);


// print
int PcutHexIx(Pcut *m, char *buf, int buf_size, int ix, const char *whole);
void PcutIxPrint(Pcut *m, int verbose, int level, int ix, const char *whole);
void PcutRangePrint(Pcut *m, int verbose, int level, int start_ix, int end_ix, const char *whole);
void PcutAllPrint(Pcut *m, int verbose, int level, const char *whole);
//}}}


//{{{ factory
// 如果为choice，其choice.var部分的cut需要动态生成。下面函数，在堆上辅助动态生成cut.
cp_t PcutDeriveCloseDef(struct PcutS *base);		
cp_t PcutDeriveValidDef(const struct PcutS *base);		
cp_t PcutDeriveOpenDef(struct PcutS *base);		

typedef cp_t (*PcutDeriveOpenT)(struct PcutS *base);		// 父类open函数
typedef cp_t (*PcutDeriveCloseT)(struct PcutS *base);		// 父类close函数
typedef cp_t (*PcutDeriveValidT)(const struct PcutS *base);

typedef struct 
{
	const char *name;
	int size;	
	const void *init;
	PcutDeriveOpenT derive_open;
	PcutDeriveCloseT derive_close;
} PcutFactoryInfo;
#define kPcutFactoryInfoDef(_name) { (_name), 0, "", PcutDeriveOpenDef, PcutDeriveCloseDef }
cp_t PcutFactoryInfoValid(const PcutFactoryInfo *info);
cp_t PcutDeriveFactoryByInfo(Pcut **m, const PcutFactoryInfo *info);
cp_t PcutDeriveDestory(Pcut *m, const PcutFactoryInfo *info);


// 声明用子类base来open/close父类的函数
#define PCUT_DECLEAR_OPEN_BASE(_class_name) cp_t _class_name##OpenBase(Pcut *base)
#define PCUT_DECLEAR_CLOSE_BASE(_class_name) cp_t _class_name##CloseBase(Pcut *base)

// 定义用子类base来open/close父类的函数
#define PCUT_DEFINE_OPEN_BASE(_class_name)	\
cp_t _class_name##OpenBase(Pcut *base) {	\
	_class_name *m = (_class_name*)base;	\
	return _class_name##Open(m);			\
}
#define PCUT_DEFINE_CLOSE_BASE(_class_name)	\
cp_t _class_name##CloseBase(Pcut *base) {	\
	_class_name *m = (_class_name*)base;	\
	return _class_name##Close(m);			\
}
//}}}


// test
cp_t PcutTest(QTEST_ARG);
#endif

