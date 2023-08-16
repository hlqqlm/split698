/*
This program is licensed under the GPLv2 License:
Copyright 2023 Huanglin <hlqqlm@qq.com>


This file is part of Split698.

Split698 is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

Split698 is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Split698. If not, see <https://www.gnu.org/licenses/>.

20230407-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 data choice 变化部分报文解析

用来描述dlt698_45中的data, 将data看做choice
*/

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qmem.h"

// var
#include "p2_array.h"
#include "p2_double_long.h"
#include "p2_double_long_unsigned.h"
#include "p2_long_unsigned.h"
#include "p2_octet_string.h"
#include "p2_struct.h"
#include "p2_datetime.h"
#include "p2_datetimes.h"
#include "p2_tsa.h"

#include "p2_data_choice.h"
#include "p2_data_choice.xcp.h"


#define TEST_EN					(1)
#define PRINT_PART_IN_TEST_EN	(1)

//{{{ all
int P2DataChoiceWholeSize(const char *whole)
{
	P2DataChoicePcut dc = kP2DataChoicePcutDef;
	const cp_t cp = P2DataChoicePcutOpen(&dc);
	if (0 != cp)
	{
		dve(cp);
		return 0;
	}

	const int size = PcutIxLen(&dc.choice.base, kPcutIxAll, whole);
	ifed(P2DataChoicePcutClose(&dc));
	return size;
}
//}}}


// {{{ choice
// 必须按大小顺序排
// choice表，即Qdlt698DateTypeT
#define kChoiceNum kP2DataTypeNum
static const P2Choice kChoiceList[kChoiceNum] = {
	// choice	name
	{ kDlt698DataTypeNull, "null" },	// NULL 0 空
	{ kDlt698DataTypeArray, "array" },	// array 1 SEQUENCE OF Data（见 7.3.1 ） 数组的元素在对象属性或方法的描述中定义
	{ kDlt698DataTypeStructure, "structure" },	//structure 2 SEQUENCE OF Data（见 7.3.1 ） 结构的元素在对象属性或方法的描述中定义
	{ kDlt698DataTypeBool, "bool" },	// bool 3 布尔值 1 或 0
	{ kDlt698DataTypeBitString, "bit-string" },	// bit-string 4 位串
	{ kDlt698DataTypeDoubleLong, "double-long" },	// double-long 5 32 位整数 -2^31…2^31-1
	{ kDlt698DataTypeDoubleLongUnsigned, "double-long-unsigned" },	//double-long-unsigned 6 32 位正整数 0…2^32-1
											// 保留 7-8
	{ kDlt698DataTypeOctetString, "octet-string" },			// octet-string 9 8 位字节串
	{ kDlt698DataTypeVisibleString, "visible-string" },		// visible-string 10 ASCII 字符串
											// 保留 11
											/*
UTF8-string 12 UTF-8 编码的字符串
保留 13-14
*/

	{ kDlt698DataTypeInteger, "integer" },		// integer 15 8 位整数 -128…127
	{ kDlt698DataTypeLong, "long" },			// long 16 16 位整数 -32768…32767
	{ kDlt698DataTypeUnsigned, "unsigned" },		// unsigned 17 8 位正整数 0…255
	{ kDlt698DataTypeLongUnsigned, "long-unsigned" },	// long-unsigned 18 16 位正整数 0…65535

/*
保留 19
long64 20 64 位整数 -2 63…2^63-1
long64-unsigned 21 64 位正整数 0…2^64-1
enum 22 枚举的元素在对象属性或方法的描述中定义 0…255
float32 23 32 位浮点数
float64 24 64 位浮点数
*/
	{ kDlt698DataTypeDatetime, "datetime"},		// date_time 25 octet-string（SIZE（10））
	/*
date 26 octet-string（SIZE（5））
time 27 octet-string（SIZE（3））
*/
	{ kDlt698DataTypeDatetimeS, "date_time_s" },		// date_time_s 28 octet-string（SIZE（7））
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
*/
	{ kDlt698DataTypeTsa, "tsa" },	// TSA 85 见 0
	/*
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

};
int P2DataChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kChoiceNum, choice);
}
uint8_t P2DataChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kChoiceNum, choice_ix);
}
cp_t P2DataChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kChoiceNum, choice_ix);
}
cp_t P2DataChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kChoiceNum, choice);
}
//}}}


//{{{ var_factory_info
static const P2StructPcut kP2StructPcutVar = kP2StructPcutDef;
static const P2DoubleLongPcut kP2DoubleLongPcutVar = kP2DoubleLongPcutDef;
static const P2DoubleLongUnsignedPcut kP2DoubleLongUnsignedPcutVar = kP2DoubleLongUnsignedPcutDef;
static const P2LongUnsignedPcut kP2LongUnsignedPcutVar = kP2LongUnsignedPcutDef;
static const P2OctetStringPcut kP2OctetStringPcutVar = kP2OctetStringPcutDef;
static const P2DatetimePcut kP2DatetimePcutVar = kP2DatetimePcutDef;
static const P2DatetimesPcut kP2DatetimesPcutVar = kP2DatetimesPcutDef;
static const P2TsaPcut kP2TsaPcutVar = kP2TsaPcutDef;

static const PcutFactoryInfo kVarFactoryInfoList[kP2DataTypeNum] = {
	// name		size	init	derive_open		derive_close
	kPcutFactoryInfoDef("null"),	// NULL 0 空
	kPcutFactoryInfoDef("array"),	// array 1 SEQUENCE OF Data（见 7.3.1 ） 数组的元素在对象属性或方法的描述中定义
	{ "structure", sizeof(kP2StructPcutVar), &kP2StructPcutVar, P2StructPcutOpenBase, P2StructPcutCloseBase },	// structure 2 SEQUENCE OF Data（见 7.3.1 ） 结构的元素在对象属性或方法的描述中定义
	kPcutFactoryInfoDef("bool"),	// bool 3 布尔值 1 或 0
	kPcutFactoryInfoDef("bit-string"),	// bit-string 4 位串
	{ "double-long", sizeof(kP2DoubleLongPcutVar), &kP2DoubleLongPcutVar, P2DoubleLongPcutOpenBase, P2DoubleLongPcutCloseBase },	// double-long 5 32 位整数 -2^31…2^31-1
	{ "double-long-unsigned", sizeof(kP2DoubleLongUnsignedPcutVar), &kP2DoubleLongUnsignedPcutVar, P2DoubleLongUnsignedPcutOpenBase, P2DoubleLongUnsignedPcutCloseBase },	//double-long-unsigned 6 32 位正整数 0…2^32-1
											// 保留 7-8
	{ "octet-string", sizeof(kP2OctetStringPcutVar), &kP2OctetStringPcutVar, P2OctetStringPcutOpenBase, P2OctetStringPcutCloseBase },	// octet-string 9 8 位字节串
	kPcutFactoryInfoDef("visible-string"),		// visible-string 10 ASCII 字符串
											// 保留 11
											/*
UTF8-string 12 UTF-8 编码的字符串
保留 13-14
*/

	kPcutFactoryInfoDef("integer"),		// integer 15 8 位整数 -128…127
	kPcutFactoryInfoDef("long"),			// long 16 16 位整数 -32768…32767
	kPcutFactoryInfoDef("unsigned"),		// unsigned 17 8 位正整数 0…255
	{ "long-unsigned", sizeof(kP2LongUnsignedPcutVar), &kP2LongUnsignedPcutVar, P2LongUnsignedPcutOpenBase, P2LongUnsignedPcutCloseBase },						// long-unsigned 18 16 位正整数 0…65535

	/*
保留 19
long64 20 64 位整数 -2 63…2^63-1
long64-unsigned 21 64 位正整数 0…2^64-1
enum 22 枚举的元素在对象属性或方法的描述中定义 0…255
float32 23 32 位浮点数
float64 24 64 位浮点数
*/
	{ "datetime", sizeof(kP2DatetimePcutVar), &kP2DatetimePcutVar, P2DatetimePcutOpenBase, P2DatetimePcutCloseBase },	// date_time 25 octet-string（SIZE（10））
/*
date 26 octet-string（SIZE（5））
time 27 octet-string（SIZE（3））
*/
	{ "date_time_s", sizeof(kP2DatetimesPcutVar), &kP2DatetimesPcutVar, P2DatetimesPcutOpenBase, P2DatetimesPcutCloseBase },	// date_time_s 28 octet-string（SIZE（7））
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
*/
	{ "tsa", sizeof(kP2TsaPcutVar), &kP2TsaPcutVar, P2TsaPcutOpenBase, P2TsaPcutCloseBase },	// TSA 85 见 0
/*
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
};
//}}}


//{{{ pcut
cp_t P2DataChoicePcutOpen(P2DataChoicePcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2DataNameChoice, kChoiceList, kP2DataTypeNum, kVarFactoryInfoList);
}
cp_t P2DataChoicePcutClose(P2DataChoicePcut *m)
{
	return P2ChoicePcutClose(&m->choice);
}
cp_t P2DataChoicePcutValid(const P2DataChoicePcut *m)
{
	return P2ChoicePcutValid(&m->choice);
}


// 定义用子类base来open/close父类的函数
cp_t P2DataChoicePcutOpenBase(Pcut *base)
{
	P2DataChoicePcut *m = (P2DataChoicePcut*)base;
	return P2DataChoicePcutOpen(m);
}
cp_t P2DataChoicePcutCloseBase(Pcut *base)
{
	P2DataChoicePcut *m = (P2DataChoicePcut*)base;
	return P2DataChoicePcutClose(m);
}
//}}}


cp_t P2DataChoiceFillInit(Pfill *m, uint8_t choice, Pfill *sub_var)
{
	const P2ChoiceFillItemChoice kFiChoice = kP2ChoiceFillItemChoiceDef(kP2DataNameChoice, PfillItemOffsetFix, kP2DataChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	const PfillItem kFiVar = PFILL_ITEM(kP2DataNameVar, PfillItemOffsetFix, PfillItemProcessBySub, kP2DataVarOffset, sub_var);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiVar), &kFiVar));

	return 0;
}


//{{{ test
#if TEST_EN > 0
static cp_t TestWholeSize(void)
{
	// 09 06 10 00 00 00 22 21
	{
	const char whole[] = "\x09\x06\x10\x00\x00\x00\x22\x21";
	const int whole_size = sizeof(whole) - 1;
	ifbr(whole_size == P2DataChoiceWholeSize(whole));
	}

	{
	//		|-Data: double_long_unsigned, u32	06 00 00 00 02	类型:6,值:2 	
	const char whole[] = "\x06\x00\x00\x00\x02";
	const int whole_size = sizeof(whole) - 1;
	ifbr(whole_size == P2DataChoiceWholeSize(whole));
	}
	return 0;
}


static const QtestItem kTestItem[] = {
	TestWholeSize,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2DataChoiceTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2DataChoiceTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

