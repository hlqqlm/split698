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

用来描述dlt698_45中的GetRequestNormalList
*/
#include "string.h"

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"
#include "p2_get_request_normal_list.h"
#include "p2_get_request_normal_list.xcp.h"
//#define this_file_id	0x8e562b7c // echo -n dlt698_45_get_request_normal_list.c | rhash --simple -

#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)


// {{{ piid
static int LenPiid(Pcut *part, int ix, const char *whole) { return P2_GRNL_PIID_SIZE; }
static int OffsetPiid(Pcut *part, int ix, const char *whole) { return P2_GRNL_PIID_OFFSET; }
static cp_t ValidPiid(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}



uint8_t P2GetRequestNormalListPiid(const char *whole)
{
	return *(whole + P2_GRNL_PIID_OFFSET);
}
//}}}


// {{{ oad
#define LenSequenceOfOad		PcutItemLenBySub	
static int OffsetSequenceOfOad(Pcut *part, int ix, const char *whole) { return P2_GRNL_SEQUENCE_OAD_OFFSET; }
#define ValidSequenceOfOad		PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2GetRequestNormalListPartNum] = {
	// name len offset valid explain
	{ "piid", LenPiid, OffsetPiid, ValidPiid, NULL },
	{ "sequence_of_oad", LenSequenceOfOad, OffsetSequenceOfOad, ValidSequenceOfOad, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2GetRequestNormalListPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2GetRequestNormalListPartIxPiid]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2GetRequestNormalListPartIxSequenceOfOad]),
};
static void PcutItemsInit(PcutItem items[kP2GetRequestNormalListPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}

cp_t P2GetRequestNormalListPcutOpen(P2GetRequestNormalListPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2GetRequestNormalListPartNum));

	ifer(P2SequenceOfPcutOpen(&m->sequence_of_oad, OAD_MEM_SIZE, "oad"));
	PcutSubSet(&m->base, kP2GetRequestNormalListPartIxSequenceOfOad, &m->sequence_of_oad.base, "sequence_of_oad");
	return 0;
}
cp_t P2GetRequestNormalListPcutClose(P2GetRequestNormalListPcut *m)
{
	dve(P2GetRequestNormalListPcutValid(m));

	PcutSubSet(&m->base, kP2GetRequestNormalListPartIxSequenceOfOad, NULL, NULL);
	ifer(P2SequenceOfPcutClose(&m->sequence_of_oad));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2GetRequestNormalListPcutValid(const P2GetRequestNormalListPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfPcutValid(&m->sequence_of_oad));
	return 0;
}


cp_t P2GetRequestNormalListPcutOpenBase(Pcut *base)
{
	P2GetRequestNormalListPcut *m = (P2GetRequestNormalListPcut*)base;
	return P2GetRequestNormalListPcutOpen(m);
}
cp_t P2GetRequestNormalListPcutCloseBase(Pcut *base)
{
	P2GetRequestNormalListPcut *m = (P2GetRequestNormalListPcut*)base;
	return P2GetRequestNormalListPcutClose(m);
}
//}}}


//{{{ value
typedef struct {
	Pdo doa;
	P2GetRequestNormalListOadValue oad;
} PdoSequenceOfOad;
static cp_t PdoProcessSequenceOfOad(struct PdoS *doa, Pcut *part, int ix, const char *whole)
{
	PdoSequenceOfOad *derive = (PdoSequenceOfOad*)doa;
	const char * const sequence_of_oad_mem = PcutIxPtrConst(part, ix, whole);

	const int num = P2SequenceOfNum(sequence_of_oad_mem);
	// 有上限限制
	ifbr(num <= kP2GetRequestNormalListOadNumMax);
	
	for (int i = 0; i < num; ++i)
	{
		const char *oad_ix_mem = P2SequenceOfContentIx(OAD_MEM_SIZE, sequence_of_oad_mem, i);
		derive->oad.list[i] = Dlt698Mem2Oad(oad_ix_mem);
	}
	derive->oad.num = num;
	// dlt698_45_sequence_of.h
	return 0;
}
#define kPdoSequenceOfOadDef { PDO_INIT(PdoProcessSequenceOfOad), kP2GetRequestNormalListOadValueDef }
static cp_t ValueInOpen(Pcut *part, P2GetRequestNormalListValue *value, const char *whole)
{
	PdoSequenceOfOad do_soo = kPdoSequenceOfOadDef;
	PcutDoSet(part, kP2GetRequestNormalListPartIxSequenceOfOad, &do_soo.doa);
	ifer(PcutIxDo(part, 0, 0, kPcutIxAll, whole));
	PcutDoSet(part, kP2GetRequestNormalListPartIxSequenceOfOad, NULL);

	value->piid = P2GetRequestNormalListPiid(whole);
	value->oad = do_soo.oad;
	return 0;
}
cp_t P2GetRequestNormalListPcutValue(P2GetRequestNormalListValue *value, const char *whole)
{
	P2GetRequestNormalListPcut grnl = kP2GetRequestNormalListPcutDef;
	ifer(P2GetRequestNormalListPcutOpen(&grnl));
	const cp_t cp = ValueInOpen(&grnl.base, value, whole);
	ifer(P2GetRequestNormalListPcutClose(&grnl));
	return cp;
}


void P2GetRequestNormalListPcutValuePrint(const P2GetRequestNormalListValue *value)
{
	qos_printf("%02xH,%d,[", value->piid, value->oad.num);
	for (int i = 0; i < value->oad.num; ++i)
	{
		const char *split = (i == (value->oad.num - 1)) ? "" : ",";
		qos_printf("%08x%s", value->oad.list[i], split);
	}
	qos_printf("]");

	return;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	// 00 03 00 10 02 00 20 00 02 00 20 04 02 00 00 
	// 3个oad
	const char whole[] = "\x00\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00";
	const int whole_size = sizeof(whole) - 1;

	P2GetRequestNormalListPcut grnl = kP2GetRequestNormalListPcutDef;
	ifer(P2GetRequestNormalListPcutOpen(&grnl));
	Pcut * const m = &grnl.base;		

	ifbr(13 == PcutIxLen(m, kP2GetRequestNormalListPartIxSequenceOfOad, whole));
	ifbr(whole_size == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(whole_size == 14);

	ifbr(1 == PcutIxOffset(m, kP2GetRequestNormalListPartIxSequenceOfOad, whole));
	ifbr(14 == PcutIxOffset(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kPcutIxAll, whole));

	if (PRINT_PART_IN_TEST_EN)
		PcutAllPrint(m, 0, whole);

	ifer(P2GetRequestNormalListPcutClose(&grnl));
	return 0;
}
static cp_t TestValue(void)
{
	// 00 03 00 10 02 00 20 00 02 00 20 04 02 00 00 
	// 3个oad
	const char whole[] = "\x05\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00";
	// const int whole_size = sizeof(whole) - 1;

	P2GetRequestNormalListValue value = kP2GetRequestNormalListValueDef;
	ifer(P2GetRequestNormalListPcutValue(&value, whole));
	ifbr(0x05 == value.piid);
	ifbr(3 == value.oad.num);
	ifbr(0x00100200 == value.oad.list[0]);
	ifbr(0x20000200 == value.oad.list[1]);
	ifbr(0x20040200 == value.oad.list[2]);
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2GetRequestNormalListTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetRequestNormalListTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
