#ifndef __DLT698_45_ADDR_H_
#define __DLT698_45_ADDR_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"

// 因为DLT698_45前缀太长，因此，将程序内的协议编号
// 编号	协议名称		缩写
// 1	DLT645_07		P1	(protocol 1#)
// 2	DLT698_45		P2	(protocol 2#)


// feature
#define P2_ADDR_FEATURE_OFFSET		(0)
#define P2_ADDR_FEATURE_SIZE		(1)

// extern_logic
#define P2_ADDR_EXTERN_LOGIC_OFFSET			(P2_ADDR_FEATURE_OFFSET + P2_ADDR_FEATURE_SIZE)
#define P2_ADDR_EXTERN_LOGIC_SIZE(_extern_logic_size)	(_extern_logic_size)

// sa_naked	
// 裸的SA地址，即不包含 地址特征 + 扩展逻辑地址
// _sa_len_value是地址长度值，直接算出来的原始值，不做任何变更
#define P2_ADDR_SA_NAKED_OFFSET(_extern_logic_size)		\
				(P2_ADDR_EXTERN_LOGIC_OFFSET + P2_ADDR_EXTERN_LOGIC_SIZE(_extern_logic_size))
#define P2_ADDR_SA_NAKED_SIZE(_extern_logic_size, _sa_len_value)	((_sa_len_value) + 1 - (_extern_logic_size))

// ca
#define P2_ADDR_CA_OFFSET(_extern_logic_size, _sa_len_value)		\
			(P2_ADDR_SA_NAKED_OFFSET(_extern_logic_size) + P2_ADDR_SA_NAKED_SIZE(_extern_logic_size, _sa_len_value))
#define P2_ADDR_CA_SIZE				(1)



// SA
#define P2_ADDR_SA_SIZE(_extern_logic_size, _sa_len_value)	\
		(P2_ADDR_FEATURE_SIZE									\
		+ P2_ADDR_EXTERN_LOGIC_SIZE(_extern_logic_size)			\
		+ P2_ADDR_SA_NAKED_SIZE(_extern_logic_size, _sa_len_value))
// addr
#define P2_ADDR_PART_SIZE(_extern_logic_size, _sa_len_value)		\
		(P2_ADDR_SA_SIZE(_extern_logic_size, _sa_len_value) + P2_ADDR_CA_SIZE)

// 地址相关
#define kP2SaNakedSizeMin	(0)			// 服务器地址，最小字节数。不包含地址特征、扩展逻辑地址
#define kP2SaNakedSizeMax	(15)		// 服务器地址，最大字节数。不包含地址特征、扩展逻辑地址
#define kP2SaSizeMin		(1 + kP2SaNakedSizeMin)			// 加1字节地址特征，无扩展逻辑地址
#define kP2SaSizeMax		(1 + 1 + kP2SaNakedSizeMax)		// 加1字节地址特征，1字节扩展逻辑地址
#define kP2AddrSizeMin		(kP2SaSizeMin + P2_ADDR_CA_SIZE)
#define kP2AddrSizeMax		(kP2SaSizeMax + P2_ADDR_CA_SIZE)


// name
#define kP2AddrNameFeature			"feature"
#define kP2AddrNameExternLogic		"extern_logic"
#define kP2AddrNameSaNaked			"sa_naked"
#define kP2AddrNameCa				"ca"

// Addr
// 各部分掩码
#define kP2SaTypeMask		(0xC0)
#define kP2SaLogicExistMask	(0x20)
#define kP2SaLogicAddrMask	(0x10)
#define kP2SaLenMask		(0x0f)


#define kP2SaTypeShift		(6)
#define kP2SaLogicAddrShift (4)
// #define	kP2SaLogicShift		(4)		// sa地址特征.逻辑地址，移动到低位，右移bit数
enum 
{
	kP2AddrSaTypeSingle = 0,
	kP2AddrSaTypeWildcard = 1,
	kP2AddrSaTypeGroup = 2,
	kP2AddrSaTypeBroadcast = 3,
	kP2AddrSaTypeNum = 4,
};

typedef struct {
	uint8_t type;			// 地址类型
	uint8_t logic;			// 逻辑地址
	uint8_t len_value;		// 地址长度,取值范围：0…15，对应表示 1…16 个字节长度；
} P2AddrSaT;
#define P2_ADDR_SA_DEF {		\
	0, 0, 0						\
}


// sa
const char *P2AddrSaCut(const char *whole);

// feature
uint8_t P2AddrSaFeature(const char *whole);
int P2AddrSaLenValue(const char *whole);
int P2AddrSaNakedSize(const char *whole);

// extern_logic
// bit5=1 表示有扩展逻辑地址，bit4 备用；地址长度 N 包含 1 个字节的扩展逻辑地址，取值范围 2…255，表示逻辑地址 2…255；
#define kP2AddrExternLogicMin		(2)
#define kP2AddrExternLogicMax		(255)
bool P2AddrExternLogicExist(const char *whole);
int P2AddrExternLogicSize(const char *whole);
// 扩展逻辑地址，不一定存在。只有存在时，调用本函数才有意义
uint8_t P2AddrExternLogic(const char *whole);

// sa_naked

// ca


// all
int P2AddrCutSize(const char *whole);

// value
typedef struct
{
	uint8_t sa_type;
	char sa_naked[kP2SaNakedSizeMax];
	uint8_t sa_naked_size;
	uint8_t extern_logic;		// 范围 2…255，如果为0 ~ 1，表示不存在extern_logic
	uint8_t ca;
} P2AddrValue;
#define kP2AddrValueDef { 0, {0}, 0, 0, 0 }


//{{{ cut
// addr_cut_ix
// 地址各部分索引号
typedef enum
{
	kP2AddrCutIxFeature,
	kP2AddrCutIxExternLogic,
	kP2AddrCutIxSaNaked,
	kP2AddrCutIxCa,
	kP2AddrCutNum				
} P2AddrCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2AddrCutNum];
} P2AddrPcut;
#define kP2AddrPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2AddrPcutOpen(P2AddrPcut *m);
cp_t P2AddrPcutClose(P2AddrPcut *m);
cp_t P2AddrPcutValid(const P2AddrPcut *m);
//}}}

#if 0
//{{{ qpack
// 注：QPackItemT的个数可以不是kP2AddrCutNum(cut num)，顺序也可以不是按照cut_ix顺序填充
typedef enum
{
	kP2AddrPackIxFeature,
	kP2AddrPackIxExternLogic,
	kP2AddrPackIxSaNaked,
	kP2AddrPackIxCa,
	kP2AddrPackNum				
} P2AddrPackIxT;
typedef struct P2AddrQpackS
{
	Qpack base;
	QpackItem items[kP2AddrPackNum];
	//P2AddrPcut cut;

	P2AddrValue value;
	/*
	uint8_t sa_type;
	char sa_naked[kP2SaNakedSizeMax];
	uint8_t sa_naked_size;
	uint8_t extern_logic;		// 范围 2…255，如果为0 ~ 1，表示不存在extern_logic
	uint8_t ca;
	*/
} P2AddrQpack;
#define kP2AddrQpackDef {								\
	kQpackDef, { kQpackItemDef }, 	\
	kP2AddrValueDef,								\
}

cp_t P2AddrQpackOpen(P2AddrQpack *m);
cp_t P2AddrQpackClose(P2AddrQpack *m);
cp_t P2AddrQpackValid(const P2AddrQpack *m);
Qpack *P2AddrQpackBase(P2AddrQpack *m);
// set
// extern_logic: 范围 0 ~ 1，表示不存在extern_logic; 范围 2…255，表示存在extern_logic
void P2AddrQpackSetData(P2AddrQpack *m, uint8_t extern_logic, uint8_t sa_type, uint8_t ca);
cp_t P2AddrQpackSetSaNaked(P2AddrQpack *m, const char *sa_naked, int sa_naked_size);

// valid_frame
// 一些针对有效帧的函数
//void P2PrintFrameEx(const char *prompt, const char *frame, const char *separator);
//void P2PrintFrame(const char *frame);
//void P2PrintFrameCut(const char *frame);
}}}
#endif


//{{{ fill
cp_t P2AddrFill(char *mem, const P2AddrValue *value);


typedef struct 
{
	PfillItem base;
} P2AddrFillItemFeature;
cp_t P2AddrFillItemProcessFeature(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2AddrFillItemFeatureDef {								\
	PFILL_ITEM(kP2AddrNameFeature, PfillItemOffsetFix, P2AddrFillItemProcessFeature, P2_ADDR_FEATURE_OFFSET, NULL),	\
}

typedef struct 
{
	PfillItem base;
} P2AddrFillItemExternLogic;
cp_t P2AddrFillItemProcessExternLogic(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2AddrFillItemExternLogicDef {								\
	PFILL_ITEM(kP2AddrNameExternLogic, PfillItemOffsetFix, P2AddrFillItemProcessExternLogic, P2_ADDR_EXTERN_LOGIC_OFFSET, NULL),	\
}

typedef struct 
{
	PfillItem base;
} P2AddrFillItemSaNaked;
cp_t P2AddrFillItemProcessSaNaked(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2AddrFillItemSaNakedDef {								\
	PFILL_ITEM(kP2AddrNameSaNaked, PfillItemOffsetFollow, P2AddrFillItemProcessSaNaked, -1, NULL),	\
}

typedef struct 
{
	PfillItem base;
} P2AddrFillItemCa;
cp_t P2AddrFillItemProcessCa(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2AddrFillItemCaDef {								\
	PFILL_ITEM(kP2AddrNameCa, PfillItemOffsetFollow, P2AddrFillItemProcessCa, -1, NULL),	\
}


typedef enum
{
	kP2AddrFillIxFeature,
	kP2AddrFillIxExternLogic,
	kP2AddrFillIxSaNaked,
	kP2AddrFillIxCa,
	kP2AddrFillNum				
} P2AddrFillIxT;
typedef struct 
{
	Pfill base;
	P2AddrFillItemFeature feature;
	P2AddrFillItemExternLogic extern_logic;
	P2AddrFillItemSaNaked sa_naked;
	P2AddrFillItemCa ca;

	P2AddrValue value;
} P2AddrPfill;
#define kP2AddrPfillDef {		\
	kPfillDef,				\
	kP2AddrFillItemFeatureDef, kP2AddrFillItemExternLogicDef, kP2AddrFillItemSaNakedDef, kP2AddrFillItemCaDef,	\
	kP2AddrValueDef			\
}
cp_t P2AddrPfillOpen(P2AddrPfill *m);
cp_t P2AddrPfillClose(P2AddrPfill *m);
cp_t P2AddrPfillValid(const P2AddrPfill *m);
//}}}

// test
cp_t P2AddrTest(QTEST_ARG);
#endif

