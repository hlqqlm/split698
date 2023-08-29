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

20230825-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的CompactProxyGetResponseRecord
*/
#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"

#include "p2_compact_proxy_get_response_record.h"
#include "p2_compact_proxy_get_response_record.xcp.h"


#define TEST_EN				(0)
#define kThisCutNum			(kP2CompactProxyGetResponseRecordCutNum)


// {{{ piid-acd
static int LenPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetResponseRecordPiidAcdSize; }
static int OffsetPiidAcd(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetResponseRecordPiidAcdOffset; }
static cp_t ValidPiidAcd(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ get_result
#define LenACompactResultRecord			PcutItemLenBySub
static int OffsetACompactResultRecord(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetResponseRecordACompactResultRecordOffset; }
//#define OffsetACompactResultRecord			PcutItemOffsetDef
#define ValidACompactResultRecord			PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2CompactProxyGetResponseRecordNamePiidAcd, LenPiidAcd, OffsetPiidAcd, ValidPiidAcd, NULL },
	{ kP2CompactProxyGetResponseRecordNameACompactResultRecord, LenACompactResultRecord, OffsetACompactResultRecord, ValidACompactResultRecord, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetResponseRecordCutIxPiidAcd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetResponseRecordCutIxACompactResultRecord]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxyGetResponseRecordPcutOpen(P2CompactProxyGetResponseRecordPcut *m)
{

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2ACompactResultRecordPcutOpen(&m->a_compact_result_record));
	PcutSubSet(&m->base, kP2CompactProxyGetResponseRecordCutIxACompactResultRecord, &m->a_compact_result_record.choice.base, NULL);
	return 0;
}
cp_t P2CompactProxyGetResponseRecordPcutClose(P2CompactProxyGetResponseRecordPcut *m)
{
	dve(P2CompactProxyGetResponseRecordPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxyGetResponseRecordCutIxACompactResultRecord, NULL, NULL);
	ifer(P2ACompactResultRecordPcutClose(&m->a_compact_result_record));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxyGetResponseRecordPcutValid(const P2CompactProxyGetResponseRecordPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2ACompactResultRecordPcutValid(&m->a_compact_result_record));
	return 0;
}


cp_t P2CompactProxyGetResponseRecordPcutOpenBase(Pcut *base)
{
	P2CompactProxyGetResponseRecordPcut *m = (P2CompactProxyGetResponseRecordPcut*)base;
	return P2CompactProxyGetResponseRecordPcutOpen(m);
}
cp_t P2CompactProxyGetResponseRecordPcutCloseBase(Pcut *base)
{
	P2CompactProxyGetResponseRecordPcut *m = (P2CompactProxyGetResponseRecordPcut*)base;
	return P2CompactProxyGetResponseRecordPcutClose(m);
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
cp_t P2CompactProxyGetResponseRecordTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyGetResponseRecordTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
