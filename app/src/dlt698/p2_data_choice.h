#ifndef __DLT698_45_DATA_CHOICE_H_ 
#define __DLT698_45_DATA_CHOICE_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "p2_choice.h"


// name
#define kP2DataName					"data"
#define kP2DataNameChoice			"type"
#define kP2DataNameVar				"var"

// offset-size
#define kP2DataChoiceOffset			(0)
#define kP2DataChoiceSize			(1)

#define kP2DataVarOffset			(kP2DataChoiceOffset + kP2DataChoiceSize)
#define kP2DataVarSize(_var_size)	(_var_size)

// all
int P2DataChoiceWholeSize(const char *whole);


//{{{ choice
// choice表，即Qdlt698DateTypeT

typedef enum 
{
	kDlt698DataTypeIxNull,	// NULL 0 空
	kDlt698DataTypeIxArray,	// array 1 SEQUENCE OF Data（见 7.3.1 ） 数组的元素在对象属性或方法的描述中定义
	kDlt698DataTypeIxStructure,	//structure 2 SEQUENCE OF Data（见 7.3.1 ） 结构的元素在对象属性或方法的描述中定义
	kDlt698DataTypeIxBool,	// bool 3 布尔值 1 或 0
	kDlt698DataTypeIxBitString,	// bit-string 4 位串
	kDlt698DataTypeIxDoubleLong,	// double-long 5 32 位整数 -2^31…2^31-1
	kDlt698DataTypeIxDoubleLongUnsigned,	//double-long-unsigned 6 32 位正整数 0…2^32-1
											// 保留 7-8
	kDlt698DataTypeIxOctetString,			// octet-string 9 8 位字节串
	kDlt698DataTypeIxVisibleString,		// visible-string 10 ASCII 字符串
											// 保留 11
											/*
UTF8-string 12 UTF-8 编码的字符串
保留 13-14
*/

	kDlt698DataTypeIxInteger,		// integer 15 8 位整数 -128…127
	kDlt698DataTypeIxLong,			// long 16 16 位整数 -32768…32767
	kDlt698DataTypeIxUnsigned,		// unsigned 17 8 位正整数 0…255
	kDlt698DataTypeIxLongUnsigned,	// long-unsigned 18 16 位正整数 0…65535

/*
保留 19
long64 20 64 位整数 -2 63…2^63-1
long64-unsigned 21 64 位正整数 0…2^64-1
enum 22 枚举的元素在对象属性或方法的描述中定义 0…255
float32 23 32 位浮点数
float64 24 64 位浮点数
*/
	kDlt698DataTypeIxDatetime,		// date_time 25 octet-string（SIZE（10））
/*
date 26 octet-string（SIZE（5））
time 27 octet-string（SIZE（3））
*/
	kDlt698DataTypeIxDatetimeS,		// date_time_s 28 octet-string（SIZE（7））
	/*
保留 29-79
OI 80 见 0
OAD 81 见 0
ROAD 82 见 0
Q/GDW XXXXX—XXXX
15
类型描述 标记 定义 数值范围
OMD 83 见 0
TI 84 见 0
TSA 85 见 0
MAC 86 见 0
RN 87 见 0
Region 88 见 0
Scaler_Unit 89 见 0
RSD 90 见 0
CSD 91 见 0
MS 92 见 0
SID 93 见 0
SID_MAC 94 见 0
COMDCB 95 见 0
RCSD 96 见 0
VQDS 97 见 0
保留 98-255
*/
	kP2DataTypeNum
} P2DataChoiceIxT;
int P2DataChoice2Ix(uint8_t choice);
uint8_t P2DataChoiceIx2Value(int choice_ix);
cp_t P2DataChoiceIxValid(int choice_ix);
cp_t P2DataChoiceValid(uint8_t choice);
//}}}


//{{{ part
typedef struct
{
	P2ChoicePcut choice;
} P2DataChoicePcut;
#define kP2DataChoicePcutDef {		\
	kP2ChoicePcutDef,					\
}

cp_t P2DataChoicePcutOpen(P2DataChoicePcut *m);
cp_t P2DataChoicePcutClose(P2DataChoicePcut *m);
cp_t P2DataChoicePcutValid(const P2DataChoicePcut *m);
//}}}


cp_t P2DataChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var);

// test
cp_t P2DataChoiceTest(QTEST_ARG);
#endif

