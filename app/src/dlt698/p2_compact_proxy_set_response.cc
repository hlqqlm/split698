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

20230829-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的CompactProxySetResponse
*/
#include <string.h>

#include "p2_compact_proxy_set_response.h"
#include "p2_compact_proxy_set_response.xcp.h"


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)
#define kThisCutNum				(kP2CompactProxySetResponseCutNum)

// {{{ piid-acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetResponsePiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetResponsePiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}



uint8_t P2CompactProxySetResponsePiidAcd(const char *whole)
{
	return *(whole + kP2CompactProxySetResponsePiidAcdOffset);
}
//}}}


// {{{ dar
#define LenSequenceOfDar		PcutItemLenBySub	
static int OffsetSequenceOfDar(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetResponseSequenceOfDarOffset; }
#define ValidSequenceOfDar		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "piid", LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ "sequence_of_dar", LenSequenceOfDar, OffsetSequenceOfDar, ValidSequenceOfDar, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetResponseCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetResponseCutIxSequenceOfDar]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxySetResponsePcutOpen(P2CompactProxySetResponsePcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2SequenceOfPcutOpen(&m->sequence_of_dar, kP2DarSize, "dar"));
	PcutSubSet(&m->base, kP2CompactProxySetResponseCutIxSequenceOfDar, &m->sequence_of_dar.base, "sequence_of_dar");
	return 0;
}
cp_t P2CompactProxySetResponsePcutClose(P2CompactProxySetResponsePcut *m)
{
	dve(P2CompactProxySetResponsePcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxySetResponseCutIxSequenceOfDar, NULL, NULL);
	ifer(P2SequenceOfPcutClose(&m->sequence_of_dar));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxySetResponsePcutValid(const P2CompactProxySetResponsePcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfPcutValid(&m->sequence_of_dar));
	return 0;
}


cp_t P2CompactProxySetResponsePcutOpenBase(Pcut *base)
{
	P2CompactProxySetResponsePcut *m = (P2CompactProxySetResponsePcut*)base;
	return P2CompactProxySetResponsePcutOpen(m);
}
cp_t P2CompactProxySetResponsePcutCloseBase(Pcut *base)
{
	P2CompactProxySetResponsePcut *m = (P2CompactProxySetResponsePcut*)base;
	return P2CompactProxySetResponsePcutClose(m);
}
//}}}


//{{{ value
typedef struct {
	Pdo doa;
	P2CompactProxySetResponseDarValue dar;
} PdoSequenceOfDar;
static cp_t PdoProcessSequenceOfDar(struct PdoS *doa, Pcut *cut, int ix, const char *whole)
{
	PdoSequenceOfDar *derive = (PdoSequenceOfDar*)doa;
	const char * const sequence_of_dar_mem = PcutIxPtrConst(cut, ix, whole);

	const int num = P2SequenceOfNum(sequence_of_dar_mem);
	// 有上限限制
	ifbr(num <= kP2CompactProxySetResponseDarNumMax);
	
	for (int i = 0; i < num; ++i)
	{
		const char *dar_ix_mem = P2SequenceOfContentIx(kP2DarSize, sequence_of_dar_mem, i);
		derive->dar.list[i] = *dar_ix_mem;
	}
	derive->dar.num = num;
	// dlt698_45_sequence_of.h
	return 0;
}
#define kPdoSequenceOfDarDef { PDO_INIT(PdoProcessSequenceOfDar), kP2CompactProxySetResponseDarValueDef }
static cp_t ValueInOpen(Pcut *cut, P2CompactProxySetResponseValue *value, const char *whole)
{
	PdoSequenceOfDar do_soo = kPdoSequenceOfDarDef;
	PcutDoSet(cut, kP2CompactProxySetResponseCutIxSequenceOfDar, &do_soo.doa);
	ifer(PcutIxDo(cut, 0, 0, kPcutIxAll, whole));
	PcutDoSet(cut, kP2CompactProxySetResponseCutIxSequenceOfDar, NULL);

	value->piid_acd = P2CompactProxySetResponsePiidAcd(whole);
	value->dar = do_soo.dar;
	return 0;
}
cp_t P2CompactProxySetResponsePcutValue(P2CompactProxySetResponseValue *value, const char *whole)
{
	P2CompactProxySetResponsePcut grnl = kP2CompactProxySetResponsePcutDef;
	ifer(P2CompactProxySetResponsePcutOpen(&grnl));
	const cp_t cp = ValueInOpen(&grnl.base, value, whole);
	ifer(P2CompactProxySetResponsePcutClose(&grnl));
	return cp;
}


void P2CompactProxySetResponsePcutValuePrint(const P2CompactProxySetResponseValue *value)
{
	qos_printf("%02xH,%u,[", value->piid_acd, value->dar.num);
	for (int i = 0; i < value->dar.num; ++i)
	{
		const char *split = (i == (value->dar.num - 1)) ? "" : ",";
		qos_printf("%08x%s", value->dar.list[i], split);
	}
	qos_printf("]");

	return;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestValue(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestValue,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2CompactProxySetResponseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxySetResponseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
