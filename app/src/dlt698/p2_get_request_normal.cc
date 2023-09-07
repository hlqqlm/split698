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

用来描述dlt698_45中的GetRequestNormal
*/
#include "string.h"

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qstr.h"
#include "qdlt698.h"
#include "p2_get_request_normal.h"
#include "p2_get_request_normal.xcp.h"
//#define this_file_id	0x117bab87 // echo -n dlt698_45_get_request_normal.c | rhash --simple -


#define TEST_EN				(0)
#define PRINT_PACK_IN_TEST_EN	(0)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2GetRequestNormalPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2GetRequestNormalPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
#if 0
static cp_t ValuePiid(Pcut *m, int ix, const char *whole, void *value, int value_size)
{
	dvb(sizeof(P2GetRequestNormalValue) == value_size);
	//dvb(ValueTypeGetRequestNormal == value_type);
	P2GetRequestNormalValue *grn = value;
	grn->piid = P2GetRequestNormalPiid(whole);
	return 0;
}
#endif



uint8_t P2GetRequestNormalPiid(const char *whole)
{
	return *(whole + kP2GetRequestNormalPiidOffset);
}
//}}}


// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2GetRequestNormalOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2GetRequestNormalOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}


OadT P2GetRequestNormalOad(const char *whole)
{
	const char *mem = whole + kP2GetRequestNormalOadOffset;
	const OadT oad = Dlt698Mem2Oad(mem);
printf_hex_ex("oad_mem: ", "\n", mem, 4, "");
qos_printf("oad_out=%08x\n", oad);
	return oad;
}

//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2GetRequestNormalCutNum] = {
	// name len offset valid explain
	{ "piid", LenPiid, OffsetPiid, ValidPiid, NULL },
	{ "oad", LenOad, OffsetOad, ValidOad, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2GetRequestNormalCutNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2GetRequestNormalCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2GetRequestNormalCutIxOad]),
};
static void PcutItemsInit(PcutItem items[kP2GetRequestNormalCutNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}

cp_t P2GetRequestNormalPcutOpen(P2GetRequestNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2GetRequestNormalCutNum));
	return 0;
}
cp_t P2GetRequestNormalPcutClose(P2GetRequestNormalPcut *m)
{
	dve(P2GetRequestNormalPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2GetRequestNormalPcutValid(const P2GetRequestNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2GetRequestNormalPcutOpenBase(Pcut *base)
{
	P2GetRequestNormalPcut *m = (P2GetRequestNormalPcut*)base;
	return P2GetRequestNormalPcutOpen(m);
}
cp_t P2GetRequestNormalPcutCloseBase(Pcut *base)
{
	P2GetRequestNormalPcut *m = (P2GetRequestNormalPcut*)base;
	return P2GetRequestNormalPcutClose(m);
}



//}}}


//{{{ value
cp_t P2GetRequestNormalPcutValue(P2GetRequestNormalValue *value, const char *whole)
{
	value->piid = P2GetRequestNormalPiid(whole);
	value->oad = P2GetRequestNormalOad(whole);
	return 0;
}
//}}}

#if 0
//{{{ qpack
static cp_t FillPiid(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len) 
{ 
	ifbr(sizeof(P2GetRequestNormalValue) == value_size);
	const P2GetRequestNormalValue * const val = value;
	mem[offset] = val->piid;
	*fill_len = kP2GetRequestNormalPiidSize;
	return 0; 
}
static cp_t FillOad(Qpack *pack, int ix, char *mem, int mem_size, int offset, void *value, int value_size, int *fill_len) 
{ 
	ifbr(sizeof(P2GetRequestNormalValue) == value_size);
	const P2GetRequestNormalValue * const val = value;
	Dlt698Oad2Mem(mem + offset, val->oad);
	*fill_len = kP2GetRequestNormalOadSize;
	return 0; 
}


// 为了节约内存，const部分集中在一起
// 固定部分
// pack顺序，要保证前一部分填写完毕后，要能推算出后一部分的偏移量。即后填的可以依赖先填的，但不能先填的依赖后填的
static const QpackItemFix kPackFix[kP2GetRequestNormalPackNum] = {
	// cut_ix	fill
	{ "piid", QpackItemOffsetFollow, FillPiid },
	{ "oad", QpackItemOffsetFollow, FillOad },
};
static const QpackItem kPackItemsPattern[kP2GetRequestNormalPackNum] = {
	QPACK_ITEM_NO_SUB(&kPackFix[kP2GetRequestNormalPackIxPiid]),
	QPACK_ITEM_NO_SUB(&kPackFix[kP2GetRequestNormalPackIxOad]),
};
static void QpackItemsInit(QpackItem items[kP2GetRequestNormalPackNum])
{
	memcpy(items, kPackItemsPattern, sizeof(kPackItemsPattern));
}


cp_t P2GetRequestNormalQpackValid(const P2GetRequestNormalQpack *m)
{
	ifer(QpackValid(&m->base));
	return 0;
}
cp_t P2GetRequestNormalQpackOpen(P2GetRequestNormalQpack *m)
{
	//ifer(P2GetRequestNormalPcutOpen(&m->cut.base));
	QpackItemsInit(m->items);
	ifer(QpackOpen(&m->base, m->items, kP2GetRequestNormalPackNum, &m->value, sizeof(P2GetRequestNormalValue)));
	//QpackSetValue(QpackBasePtr(&m->base), &m->value, sizeof(P2GetRequestNormalValue));
	return 0;
}
cp_t P2GetRequestNormalQpackClose(P2GetRequestNormalQpack *m)
{
	dve(P2GetRequestNormalQpackValid(m));
	ifer(QpackClose(&m->base));
	return 0;
}

//}}}
#endif

//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestQpack(void)
{
	P2GetRequestNormalQpack grn = kP2GetRequestNormalQpackDef;
	ifer(P2GetRequestNormalQpackOpen(&grn));
	Qpack * const pack = &grn.base;

	const P2GetRequestNormalValue value = {0, 0x40010200};
	// 下面两个方法等效，都是设置value的值
	ifer(QpackValueCopy(pack, &value, sizeof(value)));
	grn.value = value;

	if (PRINT_PACK_IN_TEST_EN)
		QpackSetPrintFill(pack, QpackPrintFillNormal);

	char mem[kP2GetRequestNormalWholeSize];
	ifer(QpackDo(pack, mem, sizeof(mem)));

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char result[] = "\x00\x40\x01\x02\x00";
	const int result_size = sizeof(result) - 1;

	ifbr(result_size == QpackIxLen(pack, kQpackIxAll));
	ifbr(0 == memcmp(result, mem, result_size));

	ifer(P2GetRequestNormalQpackClose(&grn));
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2GetRequestNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetRequestNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
