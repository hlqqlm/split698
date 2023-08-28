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

20230828-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的set request normal list
*/
#include "p2_compact_proxy_set_request_normal.h"
#include "p2_compact_proxy_set_request_normal.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2CompactProxySetRequestNormalCutNum)



// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetRequestNormalPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetRequestNormalPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ timeout
static int LenTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetRequestNormalTimeoutSize; }
static int OffsetTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetRequestNormalTimeoutOffset; }
static cp_t ValidTimeout(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ tsa
#define LenTsa		PcutItemLenBySub
static int OffsetTsa(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetRequestNormalTsaOffset; }
#define ValidTsa		PcutItemValidBySub
//}}}


// {{{ sequence_of_oad_data
#define LenSequenceOfOadData		PcutItemLenBySub
//static int OffsetSequenceOfOadData(Pcut *cut, int ix, const char *whole) { return kP2CompactProxySetRequestNormalSequenceOfOadDataOffset; }
#define OffsetSequenceOfOadData		PcutItemOffsetDef
#define ValidSequenceOfOadData		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactProxySetRequestNormalNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2CompactProxySetRequestNormalNameTimeout, LenTimeout, OffsetTimeout, ValidTimeout, NULL },
	{ kP2CompactProxySetRequestNormalNameTsa, LenTsa, OffsetTsa, ValidTsa, NULL },
	{ kP2CompactProxySetRequestNormalNameSequenceOfOadData, LenSequenceOfOadData, OffsetSequenceOfOadData, ValidSequenceOfOadData, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetRequestNormalCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetRequestNormalCutIxTimeout]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetRequestNormalCutIxTsa]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxySetRequestNormalCutIxSequenceOfOadData]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxySetRequestNormalPcutOpen(P2CompactProxySetRequestNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TsaPcutOpen(&m->tsa));
	PcutSubSet(&m->base, kP2CompactProxySetRequestNormalCutIxTsa, &m->tsa.base, NULL);

	ifer(P2OadDataPcutOpen(&m->oad_data_cut));
	ifer(P2SequenceOfVarLenPcutOpen(&m->sequence_of_oad_data_cut, &m->oad_data_cut.base, kP2CompactProxySetRequestNormalNameSequenceOfOadData));
	PcutSubSet(&m->base, kP2CompactProxySetRequestNormalCutIxSequenceOfOadData, &m->sequence_of_oad_data_cut.base, NULL);
	return 0;
}
cp_t P2CompactProxySetRequestNormalPcutClose(P2CompactProxySetRequestNormalPcut *m)
{
	dve(P2CompactProxySetRequestNormalPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxySetRequestNormalCutIxTsa, NULL, NULL);
	ifer(P2TsaPcutClose(&m->tsa));

	PcutSubSet(&m->base, kP2CompactProxySetRequestNormalCutIxSequenceOfOadData, NULL, NULL);
	ifer(P2SequenceOfVarLenPcutClose(&m->sequence_of_oad_data_cut));
	ifer(P2OadDataPcutClose(&m->oad_data_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxySetRequestNormalPcutValid(const P2CompactProxySetRequestNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TsaPcutValid(&m->tsa));
	ifer(P2SequenceOfVarLenPcutValid(&m->sequence_of_oad_data_cut));
	ifer(P2OadDataPcutValid(&m->oad_data_cut));
	return 0;
}


cp_t P2CompactProxySetRequestNormalPcutOpenBase(Pcut *base)
{
	P2CompactProxySetRequestNormalPcut *m = (P2CompactProxySetRequestNormalPcut*)base;
	return P2CompactProxySetRequestNormalPcutOpen(m);
}
cp_t P2CompactProxySetRequestNormalPcutCloseBase(Pcut *base)
{
	P2CompactProxySetRequestNormalPcut *m = (P2CompactProxySetRequestNormalPcut*)base;
	return P2CompactProxySetRequestNormalPcutClose(m);
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
cp_t P2CompactProxySetRequestNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxySetRequestNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
