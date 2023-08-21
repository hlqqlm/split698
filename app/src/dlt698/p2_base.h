#ifndef __P2_BASE_H_
#define __P2_BASE_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qtime.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "p2_addr.h"

// name
#define kP2NameApdu			"apdu"

// 因为DLT698_45前缀太长，因此，将程序内的协议编号
// 编号	协议名称		缩写
// 1	DLT645_07		P1	(protocol 1#)
// 2	DLT698_45		P2	(protocol 2#)

#define P2_INVALID_DATA		(0xEE)	// 无效数据
#define P2_LEAD_CH			(0xfe)	// 在有效数据帧前加 4 个 FEH 作为前导码；
#define P2_LEAD_NUM			(4)

#define P2_HEAD				(0x68)
#define P2_END				(0x16)

// 头起始符
#define P2_HEAD_OFFSET				(0)		
#define P2_HEAD_SIZE				(1)

// 长度域 L
#define P2_L_OFFSET					(P2_HEAD_OFFSET + P2_HEAD_SIZE)
#define P2_L_SIZE					(2)

// 控制域 C
#define P2_C_OFFSET					(P2_L_OFFSET + P2_L_SIZE)
#define P2_C_SIZE					(1)

// 地址域 A
#define P2_ADDR_OFFSET				(P2_C_OFFSET + P2_C_SIZE)
#define P2_ADDR_SIZE(_addr_size)	(_addr_size)

// 帧头校验 HCS
#define P2_HCS_OFFSET(_addr_size)	(P2_ADDR_OFFSET + P2_ADDR_SIZE(_addr_size))
#define P2_HCS_SIZE					(2)

#define P2_FCS_SIZE_				(2)

// 链路用户数据 (包含一个APDU或APDU分帧片段) LUD = Link User Data
// _l_value		长度域L算出来的值
#define P2_LUD_OFFSET(_addr_size)	(P2_HCS_OFFSET(_addr_size) + P2_HCS_SIZE)
#define P2_LUD_SIZE(_l_value, _addr_size)		\
		((_l_value) - P2_L_SIZE - P2_C_SIZE - P2_ADDR_SIZE(_addr_size) - P2_HCS_SIZE - P2_FCS_SIZE_)

// 帧校验 FCS
#define P2_FCS_OFFSET(_l_value)		(P2_HEAD_SIZE  + (_l_value) - P2_FCS_SIZE_)
#define P2_FCS_SIZE					(P2_FCS_SIZE_)

// 头结束符
#define P2_END_OFFSET(_l_value)		(P2_FCS_OFFSET(_l_value) + P2_FCS_SIZE)
#define P2_END_SIZE					(1)


#define P2_FRAME_SIZE(_l_value)		(P2_END_OFFSET(_l_value) + P2_END_SIZE)


// l_value <-> lud_size
#define P2_L_VALUE_2_LUD_SIZE(_l_value, _addr_size)			P2_LUD_SIZE(_l_value, _addr_size)
#define P2_LUD_SIZE_2_L_VALUE(_lud_size, _addr_size)		(P2_L_SIZE + P2_C_SIZE + P2_ADDR_SIZE(_addr_size) + P2_HCS_SIZE + (_lud_size) + P2_FCS_SIZE)

// frame_size <-> l_value
#define P2_FRAME_SIZE_2_L_VALUE(_frame_size)		((_frame_size) - P2_HEAD_SIZE - P2_END_SIZE)
#define P2_L_VALUE_2_FRAME_SIZE(_l_value)			P2_FRAME_SIZE(_l_value)



// LUD最大长度
#define P2_LUD_SIZE_MIN					(0)
#define P2_LUD_SIZE_MAX					(300)
// l_value最大最小
#define P2_L_VALUE_MIN					P2_LUD_SIZE_2_L_VALUE(P2_LUD_SIZE_MIN, kP2AddrSizeMin)
#define P2_L_VALUE_MAX					P2_LUD_SIZE_2_L_VALUE(P2_LUD_SIZE_MAX, kP2AddrSizeMax)
// 报文最小、最大长度
#define P2_FRAME_SIZE_MIN				P2_L_VALUE_2_FRAME_SIZE(P2_L_VALUE_MIN)
#define P2_FRAME_SIZE_MAX				P2_L_VALUE_2_FRAME_SIZE(P2_L_VALUE_MAX)


// name
#define kP2NameLead		"lead"
#define kP2NameHead		"head"
#define kP2NameL		"L"
#define kP2NameC		"C"
#define kP2NameAddr		"A"
#define kP2NameHcs		"HCS"
#define kP2NameLud		"LUD"
#define kP2NameFcs		"FCS"
#define kP2NameEnd		"end"


// frame_size
int P2FrameSize(const char *frame);
cp_t P2FrameSizeValid(int frame_size);


// lead
// return: lead num
int P2FrameFillLead(char *mem);

// 长度L
// 长度域L由2字节组成，采用BIN编码，定义见图7 。
// bit15 bit14 bit13 bit12 bit11 bit10 bit9 bit8 bit7 bit6 bit5 bit4 bit3 bit2 bit1 bit0
// 保留 帧数据长度单位 帧数据长度值
// 图7 长度域 L 定义
// 其中：
// a) bit0…bit13：帧数据长度值，是传输帧中不包含起始字符和结束字符的数据长度；
// b) bit14：帧数据长度单位，0 表示帧数据长度单位为字节，1 表示帧数据长度单位为千字节；
// c) bit15：保留
int P2LValueCut(const char l_cut[P2_L_SIZE]);
int P2LValue(const char *frame);
cp_t P2LValuePack(char l_cut[P2_L_SIZE], int l_value);


// 功能码
// 功能码 服务类型 应用说明
// 0 保留
// 1 链路管理 链路连接管理（登录，心跳，退出登录）
// 2 保留
// 3 用户数据 应用连接管理及数据交换服务
// 4…7 保留
// 功能码值
#define kP2FunCodeKeep0						(0)
#define kP2FunCodeLink						(1)
#define kP2FunCodeKeep2						(2)
#define kP2FunCodeUserData					(3)	// 用户数据 应用层
#define kP2FunCodeKeep4						(4)
#define kP2FunCodeKeep5						(5)
#define kP2FunCodeKeep6						(6)
#define kP2FunCodeKeep7						(7)
cp_t P2FunCodeValid(uint8_t fc);


// 控制各部分mask
#define kP2ControlDirFromServerMask		(0x80)
#define kP2ControlPrmFromClientMask		(0x40)
#define kP2ControlSplitMask				(0x20)	// 分帧标志
#define kP2ControlScambleMask			(0x08)	// 扰码标志位sc	Scramble 
#define kP2ControlFuncCodeMask			(0x07)	// 功能码
// dir_prm
#define kP2DirPrmClientResponse			(0)		// 0 0 客户机对服务器上报的响应
#define kP2DirPrmClientRequest			(1)		// 0 1 客户机发起的请求
#define kP2DirPrmServerReport			(2)		// 1 0 服务器发起的上报
#define kP2DirPrmServerResponse			(3)		// 1 1 服务器对客户机请求的响应

#define kP2ControlDirPrmMask			(0xc0)
#define kP2ControlDirPrmShift			(6)

#define kP2ControlSplitShift			(5)
#define kP2ControlScambleShift			(3)
typedef struct {
								// 传输方向位及启动标志位定义：
								// a) 传输方向位：bit7=0 表示此帧是由客户机发出的；bit7=1 表示此帧是由服务器发出的；
								// b) 启动标志位：bit6=0 表示此帧是由服务器发起的；bit6=1 表示此帧是由客户机发起的。
								// DIR PRM 组合意义
								// 0 0 客户机对服务器上报的响应
								// 0 1 客户机发起的请求
								// 1 0 服务器发起的上报
								// 1 1 服务器对客户机请求的响应
	//uint8_t dir_from_server;	
	//uint8_t prm_form_client;	
	uint8_t dir_prm;
	uint8_t split;				// 分帧标志位
								// 分帧标志位: bit5=0，表示此帧链路用户数据为完整APDU；bit5=1，表示此帧链路用户数据为APDU片段
	uint8_t	scamble;			// 扰码标志位
								// 扰码标志位: bit3=0，表示此帧链路用户数据不加扰码；bit3=1，表示此帧链路用户数据加扰码，发送时链路用户数据按字节加33H。
								// todo: 程序里没有处理该位
	uint8_t func_code;			// 功能码
} P2Control;
#define kP2ControlDef {		\
	0, 0, 0, 0				\
}
// 返回帧里的控制码
uint8_t P2ControlInFrame(const char *frame);
// 拆分控制码
void P2ControlSplit(P2Control *dst, uint8_t control);
uint8_t P2ControlPack(const P2Control *control);
uint8_t P2ControlDirPrm(uint8_t control);
void P2ControlPrint(uint8_t control);


// Addr


// HCS
// 计算HCS的偏移量以及长度
#define P2_HCS_CALC_OFFSET				(P2_L_OFFSET)
#define P2_HCS_CALC_LEN(_addr_size)		(P2_L_SIZE + P2_C_SIZE + P2_ADDR_SIZE(_addr_size))
uint16_t P2HcsInFrame(const char *frame);
uint16_t P2HcsCalc(const char *frame);
void P2HcsPack(char *frame);

// LUD = APDU
int P2LudSize(const char *frame);
int P2LudOffset(const char *frame);
const char *P2Lud(const char *frame);
uint8_t P2LudChoice(const char *frame);
const char *P2LudChoiceStr(uint8_t choice);

// FCS
// 6.2.7 帧校验 FCS
// 帧校验FCS为2字节，是对整帧不包含起始字符、结束字符和FCS本身的所有字节的校验，校验算法见附 录 D。
// 计算FCS的偏移量以及长度
#define P2_FCS_CALC_OFFSET				(P2_L_OFFSET)
#define P2_FCS_CALC_LEN(_l_value)		((_l_value) - P2_FCS_SIZE)
uint16_t P2FcsInFrame(const char *frame);
uint16_t P2FcsCalc(const char *frame);
void P2FcsPack(char *frame);

// end
uint8_t P2End(const char *frame);

// cut_ix
// 协议各部分索引号
typedef enum
{
	kP2CutIxHead,
	kP2CutIxL,
	kP2CutIxC,
	kP2CutIxA,
	kP2CutIxHcs,
	kP2CutIxLud,			// 链路用户数据
	kP2CutIxFcs,
	kP2CutIxEnd,
	// kP2CutIxAll,			// 当cut_ix=kP2CutAll，可以表示整个帧
	kP2CutNum				
} P2CutIx;


// pcut
typedef struct 
{
	Pcut base;
	P2AddrPcut addr_cut;
	PcutItem items[kP2CutNum];
} P2Pcut;
#define kP2PcutDef { kPcutDef, kP2AddrPcutDef, { kPcutItemDef } }

cp_t P2PcutOpen(P2Pcut *m);
cp_t P2PcutClose(P2Pcut *m);
cp_t P2PcutValid(const P2Pcut *m);

#if 0
// qpack
// 组帧用
// 注：QPackItemT的个数可以不是kP2AddrCutNum(cut num)，顺序也可以不是按照cut_ix顺序填充
#define kP2PackNum		(kP2CutNum)
typedef struct P2QpackS
{
	Qpack base;
	QpackItem items[kP2PackNum];
	P2Pcut cut;

	// sub
	P2AddrQpack addr_pack;

	// C
	//uint8_t dir_prm;
	//bool split;
	//bool scamble;
	//uint8_t fun_code
	P2Control control;
} P2Qpack;
#define kP2QpackDef {									\
	kQpackDef, { kQpackItemDef }, kP2AddrPcutDef,	\
	kP2AddrQpackDef, NULL,								\
	kP2ControlDef,										\
}

//	kP2DirPrmClientResponse, false, false, kP2FunCodeUserData,	
cp_t P2QpackOpen(P2Qpack *m, Qpack *lud_pack);
cp_t P2QpackClose(P2Qpack *m);
cp_t P2QpackValid(const P2Qpack *m);
Qpack *P2QpackBase(P2Qpack *m);
// get
Qpack *P2QpackAddr(P2Qpack *m);
// set
void P2QpackSetC(P2Qpack *m, const P2Control *control);
#endif



// print
// 一些针对有效帧的函数
void P2PrintFrameEx(const char *prompt, const char *frame, const char *separator);
void P2PrintFrame(const char *frame);
void P2PrintFrameCut(const char *frame);


// fill
typedef struct 
{
	PfillItem base;
} P2FillItemLead;
cp_t P2FillItemProcessLead(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemLeadDef {								\
	PFILL_ITEM(kP2NameLead, PfillItemOffsetFix, P2FillItemProcessLead, 0, NULL),	\
}

typedef struct 
{
	PfillItem base;
} P2FillItemHead;
cp_t P2FillItemProcessHead(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemHeadDef {								\
	PFILL_ITEM(kP2NameHead, PfillItemOffsetFix, P2FillItemProcessHead, (P2_HEAD_OFFSET), NULL),	\
}

typedef struct 
{
	PfillItem base;
	// 不需要传入addr_size + lud_size，从fill中就可以得到
	//int addr_size;
	//int lud_size; 
} P2FillItemL;
cp_t P2FillItemProcessL(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemLDef {								\
	PFILL_ITEM(kP2NameL, PfillItemOffsetFix, P2FillItemProcessL, (P2_L_OFFSET), NULL),	\
}

typedef struct 
{
	PfillItem base;
	P2Control control;
} P2FillItemC;
cp_t P2FillItemProcessC(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemCDef {								\
	PFILL_ITEM(kP2NameC, PfillItemOffsetFix, P2FillItemProcessC, (P2_C_OFFSET), NULL),	\
	kP2ControlDef,		\
}


typedef struct 
{
	PfillItem base;
	P2AddrValue addr;
} P2FillItemAddr;
cp_t P2FillItemProcessAddr(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemAddrDef {			\
	PFILL_ITEM(kP2NameAddr, PfillItemOffsetFix, P2FillItemProcessAddr, (P2_ADDR_OFFSET), NULL),	\
	kP2AddrValueDef					\
}


typedef struct 
{
	PfillItem base;
} P2FillItemHcs;
int P2FillItemOffsetHcs(struct PfillS *fill, int ix, const char *mem, int mem_size);
cp_t P2FillItemProcessHcs(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemHcsDef {			\
	PFILL_ITEM(kP2NameHcs, P2FillItemOffsetHcs, P2FillItemProcessHcs, -1, NULL),	\
}

typedef struct 
{
	PfillItem base;
} P2FillItemLud;
int P2FillItemOffsetLud(struct PfillS *fill, int ix, const char *mem, int mem_size);
// 注： LUD/APDU由sub填写
#define kP2FillItemLudDef(_sub_lud) {			\
	PFILL_ITEM(kP2NameLud, P2FillItemOffsetLud, PfillItemProcessBySub, -1, (_sub_lud)),	\
}


typedef struct 
{
	PfillItem base;
} P2FillItemFcs;
int P2FillItemOffsetFcs(struct PfillS *fill, int ix, const char *mem, int mem_size);
cp_t P2FillItemProcessFcs(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemFcsDef {			\
	PFILL_ITEM(kP2NameFcs, P2FillItemOffsetFcs, P2FillItemProcessFcs, -1, NULL),	\
}


typedef struct 
{
	PfillItem base;
} P2FillItemEnd;
int P2FillItemOffsetEnd(struct PfillS *fill, int ix, const char *mem, int mem_size);
cp_t P2FillItemProcessEnd(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size);
#define kP2FillItemEndDef {			\
	PFILL_ITEM(kP2NameEnd, P2FillItemOffsetEnd, P2FillItemProcessEnd, -1, NULL),	\
}



typedef enum
{
	kP2FillIxA,
	kP2FillIxLud,
	kP2FillIxL,
	kP2FillIxC,
	kP2FillIxHcs,
	kP2FillIxFcs,
	kP2FillIxHead,
	kP2FillIxEnd,
	//kP2FillIxLead,		// 4个前导符0xfe
	kP2FillNum				
} P2FillIxT;
typedef struct 
{
	Pfill base;
	// P2FillItemLead lead;
	P2FillItemHead head;
	P2FillItemL l;
	P2FillItemC c;
	P2FillItemAddr addr;
	P2FillItemHcs hcs;
	P2FillItemLud lud;
	P2FillItemFcs fcs;
	P2FillItemEnd end;
} P2Pfill;
#define kP2PfillDef(_sub_lud) {		\
	kPfillDef,				\
	kP2FillItemHeadDef, kP2FillItemLDef, kP2FillItemCDef, kP2FillItemAddrDef,	\
	kP2FillItemHcsDef, kP2FillItemLudDef(_sub_lud), kP2FillItemFcsDef, kP2FillItemEndDef	\
}
// control, 如果为NULL，则不设置，后面可以用fill.c.control = control;进行设置
// addr，如果为NULL，则不设置，后面可以用fill.addr.addr = addr;进行设置
cp_t P2PfillOpen(P2Pfill *m, const P2Control *control, const P2AddrValue *addr);
cp_t P2PfillClose(P2Pfill *m);
cp_t P2PfillValid(const P2Pfill *m);

// 设置lud，在kP2PfillDef()中填写了sub_lud，可以不调用本函数
cp_t P2PfillSetLudSub(P2Pfill *m, Pfill *sub_lud);

// test
cp_t P2BaseTest(QTEST_ARG);
#endif

