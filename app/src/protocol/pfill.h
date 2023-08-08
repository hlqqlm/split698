#ifndef  __PFILL_H__
#define  __PFILL_H__

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qarray.h"

// pfill拥有的item项最大数
#define PFILL_ITEM_NUM_MAX		(200)
#define PFILL_FRAME_SIZE_MAX	(10000)


// 用来代表所有部分的pack index索引号
#define kPfillIxAll				(-1)			



struct PfillS;
struct PfillItemS;

// 跟随上一次写的位置
int PfillItemOffsetFollow(struct PfillS *fill, int ix, const char *mem, int mem_size);
// 固定位置。即PfillItemS里填写的原始值
int PfillItemOffsetFix(struct PfillS *fill, int ix, const char *mem, int mem_size);

// 空的填充函数
cp_t PfillItemProcessEmpty(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
// 用sub，来填充自己
cp_t PfillItemProcessBySub(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);

// 返回填写位置偏移量
typedef int (*PfillItemOffsetT)(struct PfillS *fill, int ix, const char *mem, int mem_size);
// 填写一段报文
// mem			填写的目标内存
// fill_size	写入的字节数
typedef cp_t (*PfillItemProcessT)(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);

// 一段报文
typedef struct PfillItemS
{
// public:
	const char *name;
	PfillItemOffsetT offset;		// fill开始的位置
	PfillItemProcessT process;		// 填写
								
	int offset_value;				// 填写的偏移量。初值0
	int fill_size;					// 填写的长度。初值0
									
	uint8_t config;					// 配置
									// free_en，当1时，pfillitem close时，要调用free()来释放对应的内存
	uint8_t status;					// 错误状态记录，非0，即表示出现了错误，原则上退出填充过程
									
	struct PfillS *sub;
// private:
} PfillItem;
#define PFILL_ITEM(_name, _offset, _process, _offset_v, _sub) { \
	(_name), (_offset), (_process)					\
	, (_offset_v), 0								\
	, 0x00, 0x00									\
	, (_sub)										\
}
cp_t PfillItemValid(const PfillItem *item);
typedef PfillItem* PfillItemPtr;

// config bit 位
#define kPfillItemConfigBitFreeEn			(0)		// free_en
void PfillItemSetConfigBit(PfillItem *item, int bit, uint8_t value);
uint8_t PfillItemConfigBit(const PfillItem *item, int bit);
void PfillItemSetFreeEn(PfillItem *item);
													
// status bit位
#define kPfillItemStatusBitOffsetLessThan0	(0)		// offset计算错误, < 0
#define kPfillItemStatusBitOffsetOverflow	(1)		// offset计算错误, 溢出
#define kPfillItemStatusBitOversize			(2)		// 超过mem_size
#define kPfillItemStatusBitFillSizeNotSet	(3)		// fill_size在process()里没有设置
#define	kPfillItemStatusBitFillProccessError (4)	// fill中process返回错误
void PfillItemSetStatusBit(PfillItem *item, int bit, uint8_t value);
uint8_t PfillItemStatusBit(const PfillItem *item, int bit);


// 在填写帧的过程中，打印mem
typedef void (*PfillPrintFill)(const struct PfillS *fill, int level, int ix, cp_t fill_cp, int offset, int size, const char *mem, int mem_size);	


// 按照array顺序填充报文
// 因为填充报文，可能不是按照解帧的index顺序进行的。而且，某一部分，会通过过个函数来进行填充；甚至，某一部分是通过sub来填充也可以。
typedef struct PfillS
{
// public:
	//PfillItemArray array;
	//PfillItem *array;
	//int len;
	// bool free_en;	// 当true时，pfill close时，要调用free()来释放对应的内存
	Qarray array;	// 存放item指针

	cp_t last_cp;	// 填写过程中，最后一个错误位置
	int last_ix;	// 填写过程中，最后一个填写ix。如果为kPfillIxAll，表示还没有开始填写

	PfillPrintFill print_fill;		// 用来打印填充过程
	bool print_mem_size_en;			// mem打印长度，两种模式：
									// 1	true, 打印mem_size长度
									// 2	false,填充多少，打印多长。避免mem_size太长，打印尾部太多0
} Pfill;
#define kPfillDef {	\
	kQarrayDef,	\
	0, kPfillIxAll,			\
	PfillPrintFillNothing, false		\
}

// class
cp_t PfillOpen(Pfill *m);
cp_t PfillClose(Pfill *m);
cp_t PfillValid(const Pfill *m);
//void PfillSetFreeEn(Pfill *m);

// print
void PfillPrintFillNothing(const struct PfillS *fill, int level, int ix, cp_t fill_cp, int offset, int size, const char *mem, int mem_size);
void PfillPrintFillNormal(const struct PfillS *fill, int level, int ix, cp_t fill_cp, int offset, int size, const char *mem, int mem_size);
void PfillSetPrintFill(Pfill *m, PfillPrintFill print_fill);
void PfillSetPrintFillWithSubs(Pfill *m, PfillPrintFill print_fill);
// mem打印长度，两种模式：
// 1	true, 打印mem_size长度
// 2	false,填充多少，打印多长
void PfillSetPrintMemSizeEn(Pfill *m, bool print_mem_size);

// item
int PfillItemNum(const Pfill *m);
cp_t PfillAdd(Pfill *m, PfillItem *item);
cp_t PfillInsert(Pfill *m, int ix, PfillItem *item);
// cp_t PfillClearIx(Pfill *m, int ix);
cp_t PfillRemoveIx(Pfill *m, int ix);
// return: 动态申请的内存
void *PfillItemFactory(Pfill *m, int size, const void *def);



// ix
// 当ix=-1时，表示所有。
// 用-1表示all, 而不是用items总数len来表示，这样就不用记items总数了
cp_t PfillIndexValid(const Pfill *m, int ix);
cp_t PfillIndexAllValid(const Pfill *m, int ix);
const char *PfillIxStr(const Pfill *m, int ix);
const PfillItem *PfillIxItemConst(const Pfill *m, int ix);
PfillItem *PfillIxItem(Pfill *m, int ix);
// 找名字，没找到，返回-1
int PfillFindName(const Pfill *m, const char *name);

// offset
// 刷新item->offset，并返回
int PfillIxOffset(Pfill *m, int ix, const char *mem, int mem_size);

// len
// 第ix段填写的长度
int PfillIxLen(const Pfill *m, int ix);
// 当前填充最远的位置
int PfillOffsetMax(const Pfill *m);

// do
// mem_size		当mem_size=-1 < 0时，表示不检查mem_size，从前往后填充mem即可。例ClientApdu的
cp_t PfillDo(Pfill *m, int level, char *mem, int mem_size);

// sub
bool PfillIxHasSub(const Pfill *m, int ix);
Pfill *PfillSub(Pfill *m, int ix);
void PfillSubSet(Pfill *m, int ix, Pfill *sub);


// test
cp_t PfillTest(QTEST_ARG);
#endif

