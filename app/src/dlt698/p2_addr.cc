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
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

一段报文称为part
本文件实现了dlt698_45协议addr地址部分的接口

part是指完整帧frame中addr地址部分

6.2.4 地址域 A
6.2.4.1 地址域组成
地址域A由可变字节数的服务器地址SA和1字节的客户机地址CA组成，定义见图9 。

服务器地址 SA
客户机地址 CA
图9 地址域定义
6.2.4.2 服务器地址 SA
6.2.4.2.1 服务器地址定义
服务器地址SA由1字节地址特征和N个字节地址组成。定义见图10 。
地址特征定义：
a) bit0…bit3：为地址的字节数，取值范围：0…15，对应表示 1…16 个字节长度；
b) bit4…bit5：逻辑地址；
bit5=0 表示无扩展逻辑地址，bit4 取值 0 和 1 分别表示逻辑地址 0 和 1；
bit5=1 表示有扩展逻辑地址，bit4 备用；地址长度 N 包含 1 个字节的扩展逻辑地址，取值范围 2…255，表示逻辑地址 2…255；
c) bit6…bit7：为服务器地址的地址类型，0 表示单地址，1 表示通配地址，2 表示组地址，3表示广播地址。
扩展逻辑地址和地址要求如下：
a) 扩展逻辑地址取值范围 2…255；
b) 编码方式为压缩 BCD 码，0 保留；
c) 当服务器地址的十进制位数为奇数时，最后字节的 bit3…bit0 用 FH 表示。

*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
//#include <sstream>
#include <string.h>

#include "protocol/pcut.h"
#include "p2_addr.h"
#include "p2_addr.xcp.h"
//#define this_file_id	0x229a343b // echo -n p2_addr.cc | rhash --simple -


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(1)

#if 0
//{{{ A
//}}}
#endif


//{{{ sa
const char *P2AddrSaCut(const char *whole)
{
	return whole + P2_ADDR_FEATURE_OFFSET;
}
//}}}


//{{{ feature
uint8_t P2AddrSaFeature(const char *whole)
{
	const char *sa = P2AddrSaCut(whole);
	const uint8_t sa_feature = (uint8_t)sa[0];
	return sa_feature;
}
int P2AddrSaLenValue(const char *whole)
{
	const uint8_t sa_feature = P2AddrSaFeature(whole);
	const uint8_t len_value = sa_feature & kP2SaLenMask;
	return len_value;
}
int P2AddrSaNakedSize(const char *whole)
{
	const uint8_t sa_feature = P2AddrSaFeature(whole);
	const bool extern_logic_exist = P2AddrExternLogicExist(whole);
	const int sa_len_value = P2AddrSaLenValue(whole);
	const uint8_t extern_logic_size = extern_logic_exist ? 1 : 0;
	const int sa_naked_size = sa_len_value + 1 - extern_logic_size;
	return sa_naked_size;
}
static int LenFeature(Pcut *cut, int ix, const char *whole) { return P2_ADDR_FEATURE_SIZE; }
static int OffsetFeature(Pcut *cut, int ix, const char *whole) { return P2_ADDR_FEATURE_OFFSET; }
static cp_t ValidFeature(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
static cp_t ExplainFeature(struct PcutS *cut, int ix, const char *whole) { return 0; }
static bool ExternLogicExist(uint8_t extern_logic)
{
	const bool extern_logic_exist = (extern_logic >= kP2AddrExternLogicMin);
	return extern_logic_exist;
}
/*
struct PdoValueFeatureS;
static cp_t ValueFeature(struct PdoS *doa, int ix, const char *whole)
{
	struct PdoValueFeatureS *value = (struct PdoValueFeatureS *)doa;
	const uint8_t sa_feature = P2AddrSaFeature(whole);
	value->sa_type = (sa_feature >> kP2SaTypeShift) & 0x03;
	value->extern_logic_exist = P2AddrExternLogicExist(whole);
	value->logic_addr = (sa_feature >> kP2SaLogicAddrShift) & 0x01;
	value->sa_naked_size = P2AddrSaNakedSize(whole);
	return 0;
}
typedef struct PdoValueFeatureS
{
	Pdo doa;
	uint8_t sa_type;
	bool extern_logic_exist;
	uint8_t logic_addr;
	int sa_naked_size;
} PdoValueFeature;
#define kPdoValueFeatureDef { PDO_INIT(ValueFeature), 0, false, 0, 0 }
*/
//}}}


//{{{ extern_logic
/*
b) bit4…bit5：逻辑地址；
bit5=0 表示无扩展逻辑地址，bit4 取值 0 和 1 分别表示逻辑地址 0 和 1；
bit5=1 表示有扩展逻辑地址，bit4 备用；地址长度 N 包含 1 个字节的扩展逻辑地址，取值范
围 2…255，表示逻辑地址 2…255；
*/
bool P2AddrExternLogicExist(const char *whole)
{
	const uint8_t sa_feature = P2AddrSaFeature(whole);
	const uint8_t logic_exist = sa_feature & kP2SaLogicExistMask;
	return !!(logic_exist);
}
int P2AddrExternLogicSize(const char *whole)
{
	return P2AddrExternLogicExist(whole) ? 1 : 0;
}
// 扩展逻辑地址，不一定存在。只有存在时，调用本函数才有意义
uint8_t P2AddrExternLogic(const char *whole)
{
	dvb(P2AddrExternLogicExist(whole));
	const char *el = whole + P2_ADDR_EXTERN_LOGIC_OFFSET;
	return *el;
}
static int LenExternLogic(Pcut *cut, int ix, const char *whole) { return P2_ADDR_EXTERN_LOGIC_SIZE(P2AddrExternLogicSize(whole)); }
static int OffsetExternLogic(Pcut *cut, int ix, const char *whole) { return P2_ADDR_EXTERN_LOGIC_OFFSET; }
// bit5=1 表示有扩展逻辑地址，bit4 备用；地址长度 N 包含 1 个字节的扩展逻辑地址，取值范围 2…255，表示逻辑地址 2…255；
static cp_t ValidExternLogic(Pcut *cut, int ix, const char *whole) 
{
	if (!P2AddrExternLogicExist(whole))
		return 0;

	const uint8_t extern_logic = P2AddrExternLogic(whole);
	ifbr(2 <= extern_logic);
	// ifbr(extern_logic <= 255);		// 一定成立
	return 0;
}
//}}}


//{{{ sa_naked
static int LenSaNaked(Pcut *cut, int ix, const char *whole) { return P2_ADDR_SA_NAKED_SIZE(P2AddrExternLogicSize(whole), P2AddrSaLenValue(whole)); }
static int OffsetSaNaked(Pcut *cut, int ix, const char *whole) { return P2_ADDR_SA_NAKED_OFFSET(P2AddrExternLogicSize(whole)); }
static cp_t ValidSaNaked(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


//{{{ ca
static int LenCa(Pcut *cut, int ix, const char *whole) { return P2_ADDR_CA_SIZE; }
static int OffsetCa(Pcut *cut, int ix, const char *whole) { return P2_ADDR_CA_OFFSET(P2AddrExternLogicSize(whole), P2AddrSaLenValue(whole)); }
static cp_t ValidCa(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
/*
uint8_t P2AddrCa(const char *whole)
{
	const char *ca = P2AddrCutIxPtrConst(kP2AddrCutIxCa, cut);		// const版本
	return *ca;
}
*/
//}}}


//{{{ all
int P2AddrCutSize(const char *whole)
{
	return P2_ADDR_PART_SIZE(P2AddrExternLogicSize(whole), P2AddrSaLenValue(whole));
}
//}}}


//{{{ addr_cut_ix
#if 0
cp_t P2AddrCutIxValid(int ix)
{
	ifbr(0 <= ix);
	ifbr(ix < kP2AddrCutNum);
	return 0;
}
static const char *kP2AddrCutIxStr[kP2AddrCutNum] = {
	"feature",		// kP2AddrCutIxFeature,
	"extern_logic",	// kP2AddrCutIxExternLogic,
	"sa_naked",		// kP2AddrCutIxSaNaked,
	"ca",			// kP2AddrCutIxCa,
	//"all"			// kP2AddrCutIxAll
};
const char *P2AddrCutIxStr(int ix)
{
	if (0 == P2AddrCutIxValid(ix))
		return kP2AddrCutIxStr[ix];
	return "invalid_addr_cut_ix";
}
#endif
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kP2AddrCutNum] = {
	// name len offset valid explain
	{ kP2AddrNameFeature, LenFeature, OffsetFeature, ValidFeature, ExplainFeature },
	{ kP2AddrNameExternLogic, LenExternLogic, OffsetExternLogic, ValidExternLogic, NULL },
	{ kP2AddrNameSaNaked, LenSaNaked, OffsetSaNaked, ValidSaNaked, NULL },
	{ kP2AddrNameCa, LenCa, OffsetCa, ValidCa, NULL },
};
	

static const PcutItem kCutItemsPattern[kP2AddrCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2AddrCutIxFeature]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2AddrCutIxExternLogic]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2AddrCutIxSaNaked]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2AddrCutIxCa])
};
static void PcutItemsInit(PcutItem items[kP2AddrCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


/*
//static cp_t P2AddrPcutValid(const P2AddrPcut *m)
static cp_t AddrPcutValid(const P2AddrPcut *m)
{
	ifer(PcutBaseValid(&m->base));
	return 0;
}
//static cp_t P2AddrPcutClose(P2AddrPcut *m)
static cp_t AddrPcutClose(P2AddrPcut *m)
{
	dve(AddrPcutValid(m));
	ifer(PcutBaseClose(&m->base));
	return 0;
}
*/
/*
static Pcut *P2AddrPcutBase(P2AddrPcut *m)
{
	dve(P2AddrPcutValid(m));
	return &m->base;
}
*/
cp_t P2AddrPcutOpen(P2AddrPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2AddrCutNum));
	return 0;
}
cp_t P2AddrPcutClose(P2AddrPcut *m)
{
	dve(P2AddrPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2AddrPcutValid(const P2AddrPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
//}}}

#if 0
//{{{ qpack

// pack_ix
static cp_t FillFeature(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len) 
{ 
	//P2AddrQpack * const paq = (P2AddrQpack*)pack;
	dvb(sizeof(P2AddrValue) == value_size);
	const P2AddrValue * const val = value;

	const bool extern_logic_exist = ExternLogicExist(val->extern_logic);
	const uint8_t extern_logic_size = extern_logic_exist ? 1 : 0;
	const uint8_t sa_len_value = val->sa_naked_size + extern_logic_size - 1;
	uint8_t feature = sa_len_value + (val->sa_type << kP2SaTypeShift);
	if (!extern_logic_exist)
		feature += (val->extern_logic << 4);
	else
		feature += (1 << 5);

	//char *dst = PcutIxPtrConst(&paq->cut, kP2AddrCutIxFeature, mem);
	//char * const dst = mem + offset;
	//*dst = feature;
	mem[offset] = feature;
	*fill_len = P2_ADDR_FEATURE_SIZE;
	return 0; 
}
static cp_t FillExternLogic(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len)
{
	//P2AddrQpack * const paq = (P2AddrQpack*)pack;
	dvb(sizeof(P2AddrValue) == value_size);
	const P2AddrValue * const val = value;

	const bool extern_logic_exist = ExternLogicExist(val->extern_logic);
	if (!extern_logic_exist)
	{
		*fill_len = 0;
		return 0;
	}
	//char *dst = PcutIxPtrConst(&paq->cut, kP2AddrCutIxExternLogic, mem);
	//char * const dst = mem + offset;
	//*dst = paq->extern_logic;
	mem[offset] = val->extern_logic;
	*fill_len = 1;
	return 0;
}
static cp_t FillSaNaked(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len) 
{
	//P2AddrQpack * const paq = (P2AddrQpack*)pack;
	dvb(sizeof(P2AddrValue) == value_size);
	const P2AddrValue * const val = value;

	//char *dst = PcutIxPtrConst(&paq->cut, kP2AddrCutIxSaNaked, mem);
	memcpy(mem + offset, val->sa_naked, val->sa_naked_size);
	*fill_len = val->sa_naked_size;
	return 0;
}
static cp_t FillCa(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len)
{
	//P2AddrQpack * const paq = (P2AddrQpack*)pack;
	dvb(sizeof(P2AddrValue) == value_size);
	const P2AddrValue * const val = value;
	// char *dst = PcutIxPtrConst(&paq->cut, kP2AddrCutIxCa, mem);
	//*dst = paq->ca;
	mem[offset] = val->ca;
	*fill_len = P2_ADDR_CA_SIZE;
	return 0;
}




// 为了节约内存，const部分集中在一起
// 固定部分
// pack顺序，要保证前一部分填写完毕后，要能推算出后一部分的偏移量。即后填的可以依赖先填的，但不能先填的依赖后填的
static const QpackItemFix kPackFix[kP2AddrPackNum] = {
	// cut_ix	fill
	{ "feature", QpackItemOffsetFollow, FillFeature },
	{ "extern_logic", QpackItemOffsetFollow, FillExternLogic },
	{ "sa_naked", QpackItemOffsetFollow, FillSaNaked },
	{ "ca", QpackItemOffsetFollow, FillCa }
};
static const QpackItem kPackItemsPattern[kP2AddrPackNum] = {
	QPACK_ITEM_NO_SUB(&kPackFix[kP2AddrPackIxFeature]),
	QPACK_ITEM_NO_SUB(&kPackFix[kP2AddrPackIxExternLogic]),
	QPACK_ITEM_NO_SUB(&kPackFix[kP2AddrPackIxSaNaked]),
	QPACK_ITEM_NO_SUB(&kPackFix[kP2AddrPackIxCa])
};
static void QpackItemsInit(QpackItem items[kP2AddrPackNum])
{
	memcpy(items, kPackItemsPattern, sizeof(kPackItemsPattern));
}


cp_t P2AddrQpackOpen(P2AddrQpack *m)
{
	//ifer(P2AddrPcutOpen(&m->cut));
	QpackItemsInit(m->items);
	ifer(QpackOpen(&m->base, m->items, kP2AddrPackNum, &m->value, sizeof(P2AddrValue)));
	return 0;
}
cp_t P2AddrQpackClose(P2AddrQpack *m)
{
	dve(P2AddrQpackValid(m));
	ifer(QpackClose(&m->base));
	//ifer(P2AddrPcutClose(&m->cut));
	return 0;
}
cp_t P2AddrQpackValid(const P2AddrQpack *m)
{
	ifer(QpackValid(&m->base));
	return 0;
}
Qpack *P2AddrQpackBase(P2AddrQpack *m)
{
	dve(P2AddrQpackValid(m));
	return &m->base;
}
/*
void P2AddrQpackSetData(P2AddrQpack *m, uint8_t extern_logic, uint8_t sa_type, uint8_t ca)
{
	m->extern_logic = extern_logic;
	m->sa_type = sa_type;
	m->ca = ca;
}
cp_t P2AddrQpackSetSaNaked(P2AddrQpack *m, const char *sa_naked, int sa_naked_size)
{
	ifbr(sa_naked_size <= kP2SaNakedSizeMax);
	memcpy(m->sa_naked, sa_naked, sa_naked_size);
	m->sa_naked_size = sa_naked_size;
	return 0;
}
*/
//}}}
#endif

//{{{ fill
cp_t P2AddrFill(char *mem, const P2AddrValue *value)
{
	P2AddrPfill fill = kP2AddrPfillDef;
	fill.value = *value;
	ifer(P2AddrPfillOpen(&fill));
	const cp_t cp = PfillDo(&fill.base, 0, mem, -1);
	ifer(P2AddrPfillClose(&fill));
	return cp;
}
cp_t P2AddrFillItemProcessFeature(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const P2AddrPfill *derive = (P2AddrPfill*)fill;
	const P2AddrValue * const val = &derive->value;

	const bool extern_logic_exist = ExternLogicExist(val->extern_logic);
	const uint8_t extern_logic_size = extern_logic_exist ? 1 : 0;
	const uint8_t sa_len_value = val->sa_naked_size + extern_logic_size - 1;
	uint8_t feature = sa_len_value + (val->sa_type << kP2SaTypeShift);
	if (!extern_logic_exist)
		feature += (val->extern_logic << 4);
	else
		feature += (1 << 5);

	//char *dst = PcutIxPtrConst(&paq->cut, kP2AddrCutIxFeature, mem);
	//char * const dst = mem + offset;
	//*dst = feature;
	mem[offset] = feature;
	*fill_size = P2_ADDR_FEATURE_SIZE;
	return 0; 
}
cp_t P2AddrFillItemProcessExternLogic(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const P2AddrPfill *derive = (P2AddrPfill*)fill;
	const P2AddrValue * const val = &derive->value;

	const bool extern_logic_exist = ExternLogicExist(val->extern_logic);
	if (!extern_logic_exist)
	{
		*fill_size = 0;
		return 0;
	}
	//char *dst = PcutIxPtrConst(&paq->cut, kP2AddrCutIxExternLogic, mem);
	//char * const dst = mem + offset;
	//*dst = paq->extern_logic;
	mem[offset] = val->extern_logic;
	*fill_size = 1;
	return 0;
}
cp_t P2AddrFillItemProcessSaNaked(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const P2AddrPfill *derive = (P2AddrPfill*)fill;
	const P2AddrValue * const val = &derive->value;

	memcpy(mem + offset, val->sa_naked, val->sa_naked_size);
	*fill_size = val->sa_naked_size;
	return 0;
}
cp_t P2AddrFillItemProcessCa(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const P2AddrPfill *derive = (P2AddrPfill*)fill;
	const P2AddrValue * const val = &derive->value;

	mem[offset] = val->ca;
	*fill_size = P2_ADDR_CA_SIZE;
	return 0;
}


cp_t P2AddrPfillOpen(P2AddrPfill *m)
{
	ifer(PfillOpen(&m->base));

	ifer(PfillAdd(&m->base, &m->feature.base));
	ifer(PfillAdd(&m->base, &m->extern_logic.base));
	ifer(PfillAdd(&m->base, &m->sa_naked.base));
	ifer(PfillAdd(&m->base, &m->ca.base));
	dvb(kP2AddrFillNum == PfillItemNum(&m->base));
	return 0;
}
cp_t P2AddrPfillClose(P2AddrPfill *m)
{
	dve(P2AddrPfillValid(m));
	ifer(PfillClose(&m->base));
	return 0;
}
cp_t P2AddrPfillValid(const P2AddrPfill *m)
{
	ifer(PfillValid(&m->base));
	ifbr(kP2AddrFillNum == PfillItemNum(&m->base));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	P2AddrPcut pcut = kP2AddrPcutDef;
	Pcut * const m = &pcut.base;
	ifer(P2AddrPcutOpen(&pcut));
	ifer(PcutValid(m));

	// index valid
	ifer(PcutIndexAllValid(m, 0));
	ifer(PcutIndexAllValid(m, kP2AddrCutNum - 1));
	ifer(PcutIndexAllValid(m, kPcutIxAll));		// 表示所有部分
	ifbr(0 != PcutIndexValid(m, kP2AddrCutNum));
	
	// 45 01 23 45 67 89 0a 00
	// (0)45  (1)   (2)01 23 45 67 89 0a     (3)00
	const char whole[] = "\x45\x01\x23\x45\x67\x89\x0a\x00";
	ifbr(1 ==  PcutIxLen(m, kP2AddrCutIxFeature, whole));
	ifbr(0 ==  PcutIxLen(m, kP2AddrCutIxExternLogic, whole));
	ifbr(6 ==  PcutIxLen(m, kP2AddrCutIxSaNaked, whole));
	ifbr(1 ==  PcutIxLen(m, kP2AddrCutIxCa, whole));
	ifbr(8 ==  PcutIxLen(m, kPcutIxAll, whole));
	
	ifbr(1 == PcutRangeLen(m, kP2AddrCutIxFeature, kP2AddrCutIxExternLogic, whole));
	ifbr(1 == PcutRangeLen(m, kP2AddrCutIxFeature, kP2AddrCutIxSaNaked, whole));
	ifbr(7 == PcutRangeLen(m, kP2AddrCutIxFeature, kP2AddrCutIxCa, whole));
	ifbr(8 == PcutRangeLen(m, 0, kP2AddrCutNum, whole));

	ifbr(0 == PcutIxOffset(m, kP2AddrCutIxFeature, whole));
	ifbr(1 == PcutIxOffset(m, kP2AddrCutIxExternLogic, whole));
	ifbr(1 == PcutIxOffset(m, kP2AddrCutIxSaNaked, whole));
	ifbr(7 == PcutIxOffset(m, kP2AddrCutIxCa, whole));
	ifbr(8 == PcutIxOffset(m, kPcutIxAll, whole));

	ifbr(whole + 0 == PcutIxPtrConst(m, kP2AddrCutIxFeature, whole));
	ifbr(whole + 1 == PcutIxPtrConst(m, kP2AddrCutIxExternLogic, whole));
	ifbr(whole + 1 == PcutIxPtrConst(m, kP2AddrCutIxSaNaked, whole));
	ifbr(whole + 7 == PcutIxPtrConst(m, kP2AddrCutIxCa, whole));
	ifbr(whole + 8 == PcutIxPtrConst(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kP2AddrCutIxFeature, whole));
	ifer(PcutIxValid(m, kP2AddrCutIxExternLogic, whole));
	ifer(PcutIxValid(m, kP2AddrCutIxSaNaked, whole));
	ifer(PcutIxValid(m, kP2AddrCutIxCa, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));


	ifer(P2AddrPcutClose(&pcut));
	return 0;
}
#if 0
static cp_t TestQpack(void)
{
	P2AddrQpack pack = kP2AddrQpackDef;
	Qpack *m = &pack.base;
	P2AddrQpackOpen(&pack);
	P2AddrQpackValid(&pack);

	/*
	uint8_t sa_type;
	char sa_naked[kP2SaNakedSizeMax];
	uint8_t sa_naked_size;
	uint8_t extern_logic;		// 范围 2…255，如果为0 ~ 1，表示不存在extern_logic
	uint8_t ca;
	*/
	const P2AddrValue value = {kP2AddrSaTypeWildcard, "\x01\x23\x45\x67\x89\x0a", 6, 0, 0};
	ifer(QpackValueCopy(m, &value, sizeof(value)));
	
	//P2AddrQpackSetData(&pack, 0, kP2AddrSaTypeWildcard, 0);
	//ifer(P2AddrQpackSetSaNaked(&pack, "\x01\x23\x45\x67\x89\x0a", 6));
	
	// 45 01 23 45 67 89 0a 00
	const char result[] = "\x45\x01\x23\x45\x67\x89\x0a\x00";
	const int result_size = sizeof(result) - 1;
	char mem[result_size];

	// 打开注释，可以测试组帧过程的打印
	if (PRINT_FILL_IN_TEST_EN)
		QpackSetPrintFill(m, QpackPrintFillNormal);
	ifer(QpackDo(m, mem, result_size));
	
	ifbr(0 == memcmp(mem, result, result_size));

	ifer(P2AddrQpackClose(&pack));
	return 0;
}
#endif

static cp_t TestPfill(void)
{
	/*
	uint8_t sa_type;
	char sa_naked[kP2SaNakedSizeMax];
	uint8_t sa_naked_size;
	uint8_t extern_logic;		// 范围 2…255，如果为0 ~ 1，表示不存在extern_logic
	uint8_t ca;
	*/
	const P2AddrValue value = {kP2AddrSaTypeWildcard, "\x01\x23\x45\x67\x89\x0a", 6, 0, 0};

	P2AddrPfill fill = kP2AddrPfillDef;
	ifer(P2AddrPfillOpen(&fill));
	fill.value = value;

	
	//P2AddrQpackSetData(&pack, 0, kP2AddrSaTypeWildcard, 0);
	//ifer(P2AddrQpackSetSaNaked(&pack, "\x01\x23\x45\x67\x89\x0a", 6));
	
	// 45 01 23 45 67 89 0a 00
	const char result[] = "\x45\x01\x23\x45\x67\x89\x0a\x00";
	const int result_size = sizeof(result) - 1;
	char mem[result_size];

	// 打开注释，可以测试组帧过程的打印
	if (PRINT_FILL_IN_TEST_EN)
		PfillSetPrintFillWithSubs(&fill.base, PfillPrintFillNormal);
	ifer(PfillDo(&fill.base, 0, mem, result_size));
	
	ifbr(0 == memcmp(mem, result, result_size));
	ifbr(result_size == P2AddrCutSize(mem));

	ifer(P2AddrPfillClose(&fill));
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	//TestQpack
	TestPfill
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2AddrTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2AddrTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

