#ifndef __P2_COMDCB_H_ 
#define __P2_COMDCB_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_enum.h"
#include "qdlt698.h"




/*
串口控制块数据类型COMDCB定义见表52　。
COMDCB∷=SEQUENCE
{
波特率    ENUMERATED
{
300bps（0），   600bps（1），     1200bps（2），
2400bps（3），  4800bps（4），    7200bps（5），
9600bps（6），  19200bps（7），   38400bps（8），
57600bps（9）， 115200bps（10）， 自适应（255）
}，
校验位  ENUMERATED {无校验（0），奇校验（1），偶校验（2）}，
数据位  ENUMERATED {5（5），6（6），7（7），8（8）}，
停止位  ENUMERATED {1（1），2（2）}，
流控    ENUMERATED {无(0)，硬件(1)，软件(2)}
}
*/
#define kP2ComdcbName			"comdcb"
#define kP2ComdcbNameBps		"bps"
#define kP2ComdcbNameParity		"parity"
#define kP2ComdcbNameDataBit	"data_bit"
#define kP2ComdcbNameStopBit	"stop_bit"
#define kP2ComdcbNameFlow		"flow"




// basic information
#define kP2ComdcbBpsOffset			(0)
#define kP2ComdcbBpsSize			(1)

#define kP2ComdcbParityOffset		(kP2ComdcbBpsOffset + kP2ComdcbBpsSize)
#define kP2ComdcbParitySize			(1)

#define kP2ComdcbDataBitOffset		(kP2ComdcbParityOffset + kP2ComdcbParitySize)
#define kP2ComdcbDataBitSize		(1)

#define kP2ComdcbStopBitOffset		(kP2ComdcbDataBitOffset + kP2ComdcbDataBitSize)
#define kP2ComdcbStopBitSize		(1)

#define kP2ComdcbFlowOffset			(kP2ComdcbStopBitOffset + kP2ComdcbStopBitSize)
#define kP2ComdcbFlowSize			(1)

#define kP2ComdcbSize				(kP2ComdcbFlowOffset + kP2ComdcbFlowSize)


//{{{ bps
typedef enum 
{
	kP2ComdcbBps300 = 0,		// 300bps（0）
	kP2ComdcbBps600 = 1,		// 600bps（1）
	kP2ComdcbBps1200 = 2,		// 1200bps（2）
	kP2ComdcbBps2400 = 3,		// 2400bps（3）
	kP2ComdcbBps4800 = 4,		// 4800bps（4）
	kP2ComdcbBps7200 = 5,		// 7200bps（5）
	kP2ComdcbBps9600 = 6,		// 9600bps（6）
	kP2ComdcbBps19200 = 7,		// 19200bps（7）
	kP2ComdcbBps38400 = 8,		// 38400bps（8）
	kP2ComdcbBps57600 = 9,		// 57600bps（9）
	kP2ComdcbBps115200 = 10,	// 115200bps（10）
	kP2ComdcbBpsAdaptive = 255,	// 自适应（255）
} P2ComdcbBpsEnumT;

typedef enum 
{
	kP2ComdcbBpsIx300,		// 300bps（0）
	kP2ComdcbBpsIx600,		// 600bps（1）
	kP2ComdcbBpsIx1200,		// 1200bps（2）
	kP2ComdcbBpsIx2400,		// 2400bps（3）
	kP2ComdcbBpsIx4800,		// 4800bps（4）
	kP2ComdcbBpsIx7200,		// 7200bps（5）
	kP2ComdcbBpsIx9600,		// 9600bps（6）
	kP2ComdcbBpsIx19200,		// 19200bps（7）
	kP2ComdcbBpsIx38400,		// 38400bps（8）
	kP2ComdcbBpsIx57600,		// 57600bps（9）
	kP2ComdcbBpsIx115200,	// 115200bps（10）
	kP2ComdcbBpsIxAdaptive,	// 自适应（255）
	kP2ComdcbBpsNum
} P2ComdcbBpsIxT;
int P2ComdcbBpsEnum2Ix(uint8_t enumv);
uint8_t P2ComdcbBpsEnumIx2Value(int enumv_ix);
cp_t P2ComdcbBpsEnumIxValid(int enumv_ix);
cp_t P2ComdcbBpsEnumValid(uint8_t enumv);
const char *P2ComdcbBpsEnumStr(uint8_t enumv);

const char *P2ComdcbBpsMem(const char *whole);
uint8_t P2ComdcbBpsValue(const char *whole);
//}}}


//{{{ parity
typedef enum 
{
	kP2ComdcbParityNone = 0,		// 无校验（0）
	kP2ComdcbParityOdd = 1,			// 奇校验（1）
	kP2ComdcbParityEven = 2,		// 偶校验（2）
} P2ComdcbParityEnumT;

typedef enum 
{
	kP2ComdcbParityIxNone,		// 无校验（0）
	kP2ComdcbParityIxOdd,		// 奇校验（1）
	kP2ComdcbParityIxEven,		// 偶校验（2）
	kP2ComdcbParityNum
} P2ComdcbParityIxT;
int P2ComdcbParityEnum2Ix(uint8_t enumv);
uint8_t P2ComdcbParityEnumIx2Value(int enumv_ix);
cp_t P2ComdcbParityEnumIxValid(int enumv_ix);
cp_t P2ComdcbParityEnumValid(uint8_t enumv);
const char *P2ComdcbParityEnumStr(uint8_t enumv);

const char *P2ComdcbParityMem(const char *whole);
uint8_t P2ComdcbParityValue(const char *whole);
//}}}


//{{{ data_bit
typedef enum 
{
	kP2ComdcbDataBit5 = 5,		// 5（5）
	kP2ComdcbDataBit6 = 6,		// 6（6）
	kP2ComdcbDataBit7 = 7,		// 7（7）
	kP2ComdcbDataBit8 = 8,		// 8（8）}
} P2ComdcbDataBitEnumT;

typedef enum 
{
	kP2ComdcbDataBitIx5,		// 5（5）
	kP2ComdcbDataBitIx6,		// 6（6）
	kP2ComdcbDataBitIx7,		// 7（7）
	kP2ComdcbDataBitIx8,		// 8（8）}
	kP2ComdcbDataBitNum
} P2ComdcbDataBitIxT;
int P2ComdcbDataBitEnum2Ix(uint8_t enumv);
uint8_t P2ComdcbDataBitEnumIx2Value(int enumv_ix);
cp_t P2ComdcbDataBitEnumIxValid(int enumv_ix);
cp_t P2ComdcbDataBitEnumValid(uint8_t enumv);
const char *P2ComdcbDataBitEnumStr(uint8_t enumv);

const char *P2ComdcbDataBitMem(const char *whole);
uint8_t P2ComdcbDataBitValue(const char *whole);
//}}}


//{{{ stop_bit
typedef enum 
{
	kP2ComdcbStopBit1 = 1,		// 1（1）
	kP2ComdcbStopBit2 = 2,		// 2（2）
} P2ComdcbStopBitEnumT;

typedef enum 
{
	kP2ComdcbStopBitIx1,		// 1（1）
	kP2ComdcbStopBitIx2,		// 2（2）
	kP2ComdcbStopBitNum
} P2ComdcbStopBitIxT;
int P2ComdcbStopBitEnum2Ix(uint8_t enumv);
uint8_t P2ComdcbStopBitEnumIx2Value(int enumv_ix);
cp_t P2ComdcbStopBitEnumIxValid(int enumv_ix);
cp_t P2ComdcbStopBitEnumValid(uint8_t enumv);
const char *P2ComdcbStopBitEnumStr(uint8_t enumv);

const char *P2ComdcbStopBitMem(const char *whole);
uint8_t P2ComdcbStopBitValue(const char *whole);
//}}}


//{{{ flow
typedef enum 
{
	kP2ComdcbFlowNone = 0,		// 无(0)
	kP2ComdcbFlowHardware = 1,		// 硬件(1)
	kP2ComdcbFlowSoftware = 2,		// 软件(2)
} P2ComdcbFlowEnumT;

typedef enum 
{
	kP2ComdcbFlowIxNone,		// 无(0)
	kP2ComdcbFlowIxHardware,		// 硬件(1)
	kP2ComdcbFlowIxSoftware,		// 软件(2)
	kP2ComdcbFlowNum
} P2ComdcbFlowIxT;
int P2ComdcbFlowEnum2Ix(uint8_t enumv);
uint8_t P2ComdcbFlowEnumIx2Value(int enumv_ix);
cp_t P2ComdcbFlowEnumIxValid(int enumv_ix);
cp_t P2ComdcbFlowEnumValid(uint8_t enumv);
const char *P2ComdcbFlowEnumStr(uint8_t enumv);

const char *P2ComdcbFlowMem(const char *whole);
uint8_t P2ComdcbFlowValue(const char *whole);
//}}}


//{{{ cut
typedef enum
{
	kP2ComdcbCutIxBps,
	kP2ComdcbCutIxParity,
	kP2ComdcbCutIxDataBit,
	kP2ComdcbCutIxStopBit,
	kP2ComdcbCutIxFlow,
	kP2ComdcbCutNum				
} P2ComdcbCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2ComdcbCutNum];

	// 模仿P2ClientApduPcut初始化
	P2EnumPcut enum_bps;
	P2EnumPcut enum_parity;
	P2EnumPcut enum_data_bit;
	P2EnumPcut enum_stop_bit;
	P2EnumPcut enum_flow;
} P2ComdcbPcut;
#define kP2ComdcbPcutDef {				\
	kPcutDef, { kPcutItemDef }					\
	, kP2EnumPcutDef, kP2EnumPcutDef, kP2EnumPcutDef, kP2EnumPcutDef, kP2EnumPcutDef	\
}

cp_t P2ComdcbPcutOpen(P2ComdcbPcut *m);
cp_t P2ComdcbPcutClose(P2ComdcbPcut *m);
cp_t P2ComdcbPcutValid(const P2ComdcbPcut *m);

cp_t P2ComdcbPcutOpenBase(Pcut *base);
cp_t P2ComdcbPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ComdcbTest(QTEST_ARG);
#endif

