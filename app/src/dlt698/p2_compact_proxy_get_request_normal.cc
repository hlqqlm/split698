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

20230823-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的compact_proxy_get_request_normal
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"
#include "p2_compact_proxy_get_request_normal.h"
#include "p2_compact_proxy_get_request_normal.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2CompactProxyGetRequestNormalCutNum)


// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestNormalPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestNormalPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ timeout
static int LenTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestNormalTimeoutSize; }
static int OffsetTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestNormalTimeoutOffset; }
static cp_t ValidTimeout(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ tsa
#define LenTsa		PcutItemLenBySub	
#define OffsetTsa	PcutItemOffsetDef
#define ValidTsa	PcutItemValidBySub
//}}}


// {{{ oad
#define LenSequenceOfOad		PcutItemLenBySub	
#define OffsetSequenceOfOad		PcutItemOffsetDef
#define ValidSequenceOfOad		PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "piid", LenPiid, OffsetPiid, ValidPiid, NULL },
	{ "timeout", LenTimeout, OffsetTimeout, ValidTimeout, NULL },
	{ "tsa", LenTsa, OffsetTsa, ValidTsa, NULL },
	{ "sequence_of_oad", LenSequenceOfOad, OffsetSequenceOfOad, ValidSequenceOfOad, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestNormalCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestNormalCutIxTimeout]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestNormalCutIxTsa]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestNormalCutIxSequenceOfOad]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxyGetRequestNormalPcutOpen(P2CompactProxyGetRequestNormalPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TsaPcutOpen(&m->tsa_cut));
	PcutSubSet(&m->base, kP2CompactProxyGetRequestNormalCutIxTsa, &m->tsa_cut.base, "tsa");

	ifer(P2SequenceOfPcutOpen(&m->sequence_of_oad_cut, OAD_MEM_SIZE, "oad"));
	PcutSubSet(&m->base, kP2CompactProxyGetRequestNormalCutIxSequenceOfOad, &m->sequence_of_oad_cut.base, "sequence_of_oad");
	return 0;
}
cp_t P2CompactProxyGetRequestNormalPcutClose(P2CompactProxyGetRequestNormalPcut *m)
{
	dve(P2CompactProxyGetRequestNormalPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxyGetRequestNormalCutIxTsa, NULL, NULL);
	ifer(P2TsaPcutClose(&m->tsa_cut));

	PcutSubSet(&m->base, kP2CompactProxyGetRequestNormalCutIxSequenceOfOad, NULL, NULL);
	ifer(P2SequenceOfPcutClose(&m->sequence_of_oad_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxyGetRequestNormalPcutValid(const P2CompactProxyGetRequestNormalPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TsaPcutValid(&m->tsa_cut));
	ifer(P2SequenceOfPcutValid(&m->sequence_of_oad_cut));
	return 0;
}


cp_t P2CompactProxyGetRequestNormalPcutOpenBase(Pcut *base)
{
	P2CompactProxyGetRequestNormalPcut *m = (P2CompactProxyGetRequestNormalPcut*)base;
	return P2CompactProxyGetRequestNormalPcutOpen(m);
}
cp_t P2CompactProxyGetRequestNormalPcutCloseBase(Pcut *base)
{
	P2CompactProxyGetRequestNormalPcut *m = (P2CompactProxyGetRequestNormalPcut*)base;
	return P2CompactProxyGetRequestNormalPcutClose(m);
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
cp_t P2CompactProxyGetRequestNormalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyGetRequestNormalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
