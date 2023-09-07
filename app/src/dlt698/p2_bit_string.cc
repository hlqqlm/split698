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

20230907-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

bit-string
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "qos/qmem.h"

#include "a_xdr.h"
#include "p2_datatype_with_content.h"

#include "p2_bit_string.h"
#include "p2_bit_string.xcp.h"


#define TEST_EN					(1)
#define kThisDatatype			(kDlt698DataTypeBitString)
#define kThisCutNum				(kP2BitStringCutNum)

//{{{ misc
static const P2BitStringPcut *ToDerive(const Pcut *cut)
{
	return (P2BitStringPcut*)(cut);
}
//}}}


//{{{ var_len
static int VarlenOffset(const char *whole)
{
	//const int datatype_size = DatatypeSize(cut, whole);
	return kP2BitStringVarlenOffset;
}
static const char *VarlenMem(const char *whole)
{
	const char *mem = whole + kP2BitStringVarlenOffset;
	return mem;
}
static int VarlenSize(const char *whole)
{
	const char *mem = VarlenMem(whole);
	return Qdlt698BitStringLenByte(mem);
	// return Qdlt698BitStringLenSize(mem);
}
static int LenVarlen(Pcut *cut, int ix, const char *whole) { return VarlenSize(whole); }
static int OffsetVarlen(Pcut *cut, int ix, const char *whole) { return VarlenOffset(whole); }
static cp_t ValidVarlen(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(whole);
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(mem));
	return 0; 
}
static cp_t ExplainVarlen(Pcut *cut, int ix, const char *whole) 
{
	const char *mem = VarlenMem(whole);
	const int value = VariableLenIntValue(mem);
	qos_printf("%dD", value);
	return 0;
}
//}}}


//{{{ content
static int LenContent(Pcut *cut, int ix, const char *whole) 
{ 
	const char *mem = VarlenMem(whole);
	// const int content_size = Qdlt698BitStringContentSize(mem);
	const int content_size = Qdlt698BitStringContentByte(mem);
	return content_size;
}
static int OffsetContent(Pcut *cut, int ix, const char *whole) 
{ 
	//const int datatype_size = DatatypeSize(cut, whole);
	const int varlen_size = VarlenSize(whole);
	return kP2BitStringContentOffset(varlen_size);
}
static cp_t ValidContent(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
//}}}


//{{{ all
int P2BitStringCutSize(const char *whole)
{
	//if (datatype_exist)
	//	return Qdlt698BitStringTypeLenContentSize(whole);
	return Qdlt698BitStringContentByte(whole);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "varlen", LenVarlen, OffsetVarlen, ValidVarlen, ExplainVarlen },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	// PCUT_ITEM_NO_SUB(&kCutFix[kP2BitStringCutIxDatatype]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2BitStringCutIxVarlen]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2BitStringCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2BitStringPcutOpen(P2BitStringPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2BitStringPcutClose(P2BitStringPcut *m)
{
	dve(P2BitStringPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2BitStringPcutValid(const P2BitStringPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
/*
void P2BitStringPcutConfigDatatypeExist(P2BitStringPcut *m, bool exist)
{
	dve(P2BitStringPcutValid(m));
	m->datatype_exist = exist;
	return;
}
*/

cp_t P2BitStringPcutOpenBase(Pcut *base)
{
	P2BitStringPcut *m = (P2BitStringPcut*)base;
	return P2BitStringPcutOpen(m);
}
cp_t P2BitStringPcutCloseBase(Pcut *base)
{
	P2BitStringPcut *m = (P2BitStringPcut*)base;
	return P2BitStringPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcutDatatypeNotExist(void)
{
	return 0;
}
static cp_t TestPcutDatatypeExist(void)
{
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcutDatatypeNotExist,
	TestPcutDatatypeExist,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2BitStringTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2BitStringTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


