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

20230815-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的Comdcb
*/
#include <string.h>

#include "p2_datetime.h"

#include "p2_comdcb.h"
#include "p2_comdcb.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2ComdcbCutNum)


//{{{ bps
// 必须按大小顺序排
static const P2Enum kBpsEnumList[kP2ComdcbBpsNum] = {
	// enumv	name
	{ kP2ComdcbBps300, "300" },		// 300bps（0）
	{ kP2ComdcbBps600, "600" },		// 600bps（1）
	{ kP2ComdcbBps1200, "1200" },		// 1200bps（2）
	{ kP2ComdcbBps2400, "2400" },		// 2400bps（3）
	{ kP2ComdcbBps4800, "4800" },		// 4800bps（4）
	{ kP2ComdcbBps7200, "7200" },		// 7200bps（5）
	{ kP2ComdcbBps9600, "9600" },		// 9600bps（6）
	{ kP2ComdcbBps19200, "19200" },		// 19200bps（7）
	{ kP2ComdcbBps38400, "38400" },		// 38400bps（8）
	{ kP2ComdcbBps57600, "57600" }, 		// 57600bps（9）
	{ kP2ComdcbBps115200, "115200" },	// 115200bps（10）
	{ kP2ComdcbBpsAdaptive, "adaptive"},	// 自适应（255）
};
int P2ComdcbBpsEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kBpsEnumList, kP2ComdcbBpsNum, enumv);
}
uint8_t P2ComdcbBpsEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kBpsEnumList, kP2ComdcbBpsNum, enumv_ix);
}
cp_t P2ComdcbBpsEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2ComdcbBpsNum, enumv_ix);
}
cp_t P2ComdcbBpsEnumValid(uint8_t enumv)
{
	return P2EnumValid(kBpsEnumList, kP2ComdcbBpsNum, enumv);
}
const char *P2ComdcbBpsEnumStr(uint8_t enumv)
{
	return P2EnumStr(kBpsEnumList, kP2ComdcbBpsNum, enumv);
}



uint8_t P2ComdcbBpsValue(const char *whole)
{
	return whole[kP2ComdcbBpsOffset];
}
const char *P2ComdcbBpsMem(const char *whole)
{
	return whole + kP2ComdcbBpsOffset;
}

/*
static int LenBps(Pcut *cut, int ix, const char *whole) { return kP2ComdcbBpsSize; }
static int OffsetBps(Pcut *cut, int ix, const char *whole) { return kP2ComdcbBpsOffset; }
static cp_t ValidBps(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t bps = P2ComdcbBpsValue(whole);
	return P2ComdcbBpsEnumValid(bps); 
}
static cp_t ExplainBps(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t bps = P2ComdcbBpsValue(whole);
	qos_printf("%s", P2ComdcbBpsEnumStr(bps));
	return 0;
}
*/
#define LenBps		PcutItemLenBySub
#define OffsetBps	PcutItemOffsetDef
#define ValidBps	PcutItemValidBySub
//}}}
 

//{{{ parity
// 必须按大小顺序排
static const P2Enum kParityEnumList[kP2ComdcbParityNum] = {
	// enumv	name
	{ kP2ComdcbParityNone, "none" },	// 无校验（0）
	{ kP2ComdcbParityOdd, "odd" },		// 奇校验（1）
	{ kP2ComdcbParityEven, "even" },	// 偶校验（2）
};
int P2ComdcbParityEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kParityEnumList, kP2ComdcbParityNum, enumv);
}
uint8_t P2ComdcbParityEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kParityEnumList, kP2ComdcbParityNum, enumv_ix);
}
cp_t P2ComdcbParityEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2ComdcbParityNum, enumv_ix);
}
cp_t P2ComdcbParityEnumValid(uint8_t enumv)
{
	return P2EnumValid(kParityEnumList, kP2ComdcbParityNum, enumv);
}
const char *P2ComdcbParityEnumStr(uint8_t enumv)
{
	return P2EnumStr(kParityEnumList, kP2ComdcbParityNum, enumv);
}



uint8_t P2ComdcbParityValue(const char *whole)
{
	return whole[kP2ComdcbParityOffset];
}
const char *P2ComdcbParityMem(const char *whole)
{
	return whole + kP2ComdcbParityOffset;
}


/*
static int LenParity(Pcut *cut, int ix, const char *whole) { return kP2ComdcbParitySize; }
static int OffsetParity(Pcut *cut, int ix, const char *whole) { return kP2ComdcbParityOffset; }
static cp_t ValidParity(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t parity = P2ComdcbParityValue(whole);
	return P2ComdcbParityEnumValid(parity); 
}
static cp_t ExplainParity(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t parity = P2ComdcbParityValue(whole);
	qos_printf("%s", P2ComdcbParityEnumStr(parity));
	return 0;
}
*/
#define LenParity		PcutItemLenBySub
#define OffsetParity	PcutItemOffsetDef
#define ValidParity	PcutItemValidBySub
//}}}


//{{{ data_bit
// 必须按大小顺序排
static const P2Enum kDataBitEnumList[kP2ComdcbDataBitNum] = {
	// enumv	name
	{ kP2ComdcbDataBit5, "5" },		// 5（5）
	{ kP2ComdcbDataBit6, "6" },		// 6（6）
	{ kP2ComdcbDataBit7, "7" },		// 7（7）
	{ kP2ComdcbDataBit8, "8" },		// 8（8）
};
int P2ComdcbDataBitEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kDataBitEnumList, kP2ComdcbDataBitNum, enumv);
}
uint8_t P2ComdcbDataBitEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kDataBitEnumList, kP2ComdcbDataBitNum, enumv_ix);
}
cp_t P2ComdcbDataBitEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2ComdcbDataBitNum, enumv_ix);
}
cp_t P2ComdcbDataBitEnumValid(uint8_t enumv)
{
	return P2EnumValid(kDataBitEnumList, kP2ComdcbDataBitNum, enumv);
}
const char *P2ComdcbDataBitEnumStr(uint8_t enumv)
{
	return P2EnumStr(kDataBitEnumList, kP2ComdcbDataBitNum, enumv);
}



uint8_t P2ComdcbDataBitValue(const char *whole)
{
	return whole[kP2ComdcbDataBitOffset];
}
const char *P2ComdcbDataBitMem(const char *whole)
{
	return whole + kP2ComdcbDataBitOffset;
}

/*
static int LenDataBit(Pcut *cut, int ix, const char *whole) { return kP2ComdcbDataBitSize; }
static int OffsetDataBit(Pcut *cut, int ix, const char *whole) { return kP2ComdcbDataBitOffset; }
static cp_t ValidDataBit(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t data_bit = P2ComdcbDataBitValue(whole);
	return P2ComdcbDataBitEnumValid(data_bit); 
}
static cp_t ExplainDataBit(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t data_bit = P2ComdcbDataBitValue(whole);
	qos_printf("%s", P2ComdcbDataBitEnumStr(data_bit));
	return 0;
}
*/
#define LenDataBit		PcutItemLenBySub
#define OffsetDataBit	PcutItemOffsetDef
#define ValidDataBit	PcutItemValidBySub
//}}}


//{{{ stop_bit
// 必须按大小顺序排
static const P2Enum kStopBitEnumList[kP2ComdcbStopBitNum] = {
	// enumv	name
	{ kP2ComdcbStopBit1, "1" },		// 1（1）
	{ kP2ComdcbStopBit2, "2" },		// 2（2）
};
int P2ComdcbStopBitEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kStopBitEnumList, kP2ComdcbStopBitNum, enumv);
}
uint8_t P2ComdcbStopBitEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kStopBitEnumList, kP2ComdcbStopBitNum, enumv_ix);
}
cp_t P2ComdcbStopBitEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2ComdcbStopBitNum, enumv_ix);
}
cp_t P2ComdcbStopBitEnumValid(uint8_t enumv)
{
	return P2EnumValid(kStopBitEnumList, kP2ComdcbStopBitNum, enumv);
}
const char *P2ComdcbStopBitEnumStr(uint8_t enumv)
{
	return P2EnumStr(kStopBitEnumList, kP2ComdcbStopBitNum, enumv);
}



uint8_t P2ComdcbStopBitValue(const char *whole)
{
	return whole[kP2ComdcbStopBitOffset];
}
const char *P2ComdcbStopBitMem(const char *whole)
{
	return whole + kP2ComdcbStopBitOffset;
}

/*
static int LenStopBit(Pcut *cut, int ix, const char *whole) { return kP2ComdcbStopBitSize; }
static int OffsetStopBit(Pcut *cut, int ix, const char *whole) { return kP2ComdcbStopBitOffset; }
static cp_t ValidStopBit(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t stop_bit = P2ComdcbStopBitValue(whole);
	return P2ComdcbStopBitEnumValid(stop_bit); 
}
static cp_t ExplainStopBit(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t stop_bit = P2ComdcbStopBitValue(whole);
	qos_printf("%s", P2ComdcbStopBitEnumStr(stop_bit));
	return 0;
}
*/
#define LenStopBit		PcutItemLenBySub
#define OffsetStopBit	PcutItemOffsetDef
#define ValidStopBit	PcutItemValidBySub
//}}}


//{{{ flow
// 必须按大小顺序排
static const P2Enum kFlowEnumList[kP2ComdcbFlowNum] = {
	// enumv	name
	{ kP2ComdcbFlowNone, "none" },		// 无(0)
	{ kP2ComdcbFlowHardware, "hardware" },		// 硬件(1)
	{ kP2ComdcbFlowSoftware, "software" },		// 软件(2)
};
int P2ComdcbFlowEnum2Ix(uint8_t enumv)
{
	return P2Enum2Ix(kFlowEnumList, kP2ComdcbFlowNum, enumv);
}
uint8_t P2ComdcbFlowEnumIx2Value(int enumv_ix)
{
	return P2EnumIx2Value(kFlowEnumList, kP2ComdcbFlowNum, enumv_ix);
}
cp_t P2ComdcbFlowEnumIxValid(int enumv_ix)
{
	return P2EnumIxValid(kP2ComdcbFlowNum, enumv_ix);
}
cp_t P2ComdcbFlowEnumValid(uint8_t enumv)
{
	return P2EnumValid(kFlowEnumList, kP2ComdcbFlowNum, enumv);
}
const char *P2ComdcbFlowEnumStr(uint8_t enumv)
{
	return P2EnumStr(kFlowEnumList, kP2ComdcbFlowNum, enumv);
}



uint8_t P2ComdcbFlowValue(const char *whole)
{
	return whole[kP2ComdcbFlowOffset];
}
const char *P2ComdcbFlowMem(const char *whole)
{
	return whole + kP2ComdcbFlowOffset;
}

/*
static int LenFlow(Pcut *cut, int ix, const char *whole) { return kP2ComdcbFlowSize; }
static int OffsetFlow(Pcut *cut, int ix, const char *whole) { return kP2ComdcbFlowOffset; }
static cp_t ValidFlow(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t flow = P2ComdcbFlowValue(whole);
	return P2ComdcbFlowEnumValid(flow); 
}
static cp_t ExplainFlow(Pcut *cut, int ix, const char *whole) 
{
	const uint8_t flow = P2ComdcbFlowValue(whole);
	qos_printf("%s", P2ComdcbFlowEnumStr(flow));
	return 0;
}
*/
#define LenFlow		PcutItemLenBySub
#define OffsetFlow	PcutItemOffsetDef
#define ValidFlow	PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2ComdcbNameBps, LenBps, OffsetBps, ValidBps, NULL },
	{ kP2ComdcbNameParity, LenParity, OffsetParity, ValidParity, NULL },
	{ kP2ComdcbNameDataBit, LenDataBit, OffsetDataBit, ValidDataBit, NULL },
	{ kP2ComdcbNameStopBit, LenStopBit, OffsetStopBit, ValidStopBit, NULL },
	{ kP2ComdcbNameFlow, LenFlow, OffsetFlow, ValidFlow, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ComdcbCutIxBps]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ComdcbCutIxParity]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ComdcbCutIxDataBit]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ComdcbCutIxStopBit]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2ComdcbCutIxFlow]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2ComdcbPcutOpen(P2ComdcbPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2EnumPcutOpen(&m->enum_bps, kP2ComdcbNameBps, kBpsEnumList, kP2ComdcbBpsNum));
	PcutSubSet(&m->base, kP2ComdcbCutIxBps, &m->enum_bps.base, kP2ComdcbNameBps);

	ifer(P2EnumPcutOpen(&m->enum_parity, kP2ComdcbNameParity, kParityEnumList, kP2ComdcbParityNum));
	PcutSubSet(&m->base, kP2ComdcbCutIxParity, &m->enum_parity.base, kP2ComdcbNameParity);

	ifer(P2EnumPcutOpen(&m->enum_data_bit, kP2ComdcbNameDataBit, kDataBitEnumList, kP2ComdcbDataBitNum));
	PcutSubSet(&m->base, kP2ComdcbCutIxDataBit, &m->enum_data_bit.base, kP2ComdcbNameDataBit);

	ifer(P2EnumPcutOpen(&m->enum_stop_bit, kP2ComdcbNameStopBit, kStopBitEnumList, kP2ComdcbStopBitNum));
	PcutSubSet(&m->base, kP2ComdcbCutIxStopBit, &m->enum_stop_bit.base, kP2ComdcbNameStopBit);

	ifer(P2EnumPcutOpen(&m->enum_flow, kP2ComdcbNameFlow, kFlowEnumList, kP2ComdcbFlowNum));
	PcutSubSet(&m->base, kP2ComdcbCutIxFlow, &m->enum_flow.base, kP2ComdcbNameFlow);
	return 0;
}
cp_t P2ComdcbPcutClose(P2ComdcbPcut *m)
{
	dve(P2ComdcbPcutValid(m));

	PcutSubSet(&m->base, kP2ComdcbCutIxBps, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_bps));

	PcutSubSet(&m->base, kP2ComdcbCutIxParity, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_parity));

	PcutSubSet(&m->base, kP2ComdcbCutIxDataBit, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_data_bit));

	PcutSubSet(&m->base, kP2ComdcbCutIxStopBit, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_stop_bit));

	PcutSubSet(&m->base, kP2ComdcbCutIxFlow, NULL, NULL);
	ifer(P2EnumPcutClose(&m->enum_flow));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2ComdcbPcutValid(const P2ComdcbPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2EnumPcutValid(&m->enum_bps));
	ifer(P2EnumPcutValid(&m->enum_parity));
	ifer(P2EnumPcutValid(&m->enum_data_bit));
	ifer(P2EnumPcutValid(&m->enum_stop_bit));
	ifer(P2EnumPcutValid(&m->enum_flow));
	return 0;
}


cp_t P2ComdcbPcutOpenBase(Pcut *base)
{
	P2ComdcbPcut *m = (P2ComdcbPcut*)base;
	return P2ComdcbPcutOpen(m);
}
cp_t P2ComdcbPcutCloseBase(Pcut *base)
{
	P2ComdcbPcut *m = (P2ComdcbPcut*)base;
	return P2ComdcbPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ComdcbTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2ComdcbTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


