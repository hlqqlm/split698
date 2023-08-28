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

用来描述dlt698_45中的compact_proxy_get_request_record
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qdlt698.h"
#include "p2_compact_proxy_get_request_record.h"
#include "p2_compact_proxy_get_request_record.xcp.h"


#define TEST_EN					(0)
#define kThisCutNum				(kP2CompactProxyGetRequestRecordCutNum)


// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestRecordPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestRecordPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ timeout
static int LenTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestRecordTimeoutSize; }
static int OffsetTimeout(Pcut *cut, int ix, const char *whole) { return kP2CompactProxyGetRequestRecordTimeoutOffset; }
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
#define LenOad			PcutItemLenBySub	
#define OffsetOad		PcutItemOffsetDef
#define ValidOad		PcutItemValidBySub
//}}}


// {{{ rsd
#define LenRsd			PcutItemLenBySub	
#define OffsetRsd		PcutItemOffsetDef
#define ValidRsd		PcutItemValidBySub
//}}}


// {{{ rcsd
#define LenRcsd			PcutItemLenBySub	
#define OffsetRcsd		PcutItemOffsetDef
#define ValidRcsd		PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "piid", LenPiid, OffsetPiid, ValidPiid, NULL },
	{ "timeout", LenTimeout, OffsetTimeout, ValidTimeout, NULL },
	{ "tsa", LenTsa, OffsetTsa, ValidTsa, NULL },
	{ "oad", LenOad, OffsetOad, ValidOad, NULL },
	{ "rsd", LenRsd, OffsetRsd, ValidRsd, NULL },
	{ "rcsd", LenRcsd, OffsetRcsd, ValidRcsd, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestRecordCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestRecordCutIxTimeout]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestRecordCutIxTsa]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestRecordCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestRecordCutIxRsd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CompactProxyGetRequestRecordCutIxRcsd]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2CompactProxyGetRequestRecordPcutOpen(P2CompactProxyGetRequestRecordPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2TsaPcutOpen(&m->tsa));
	PcutSubSet(&m->base, kP2CompactProxyGetRequestRecordCutIxTsa, &m->tsa.base, "tsa");

	ifer(P2RsdPcutOpen(&m->rsd));
	PcutSubSet(&m->base, kP2CompactProxyGetRequestRecordCutIxRsd, &m->rsd.choice.base, "rsd");

	ifer(P2RcsdPcutOpen(&m->rcsd));
	PcutSubSet(&m->base, kP2CompactProxyGetRequestRecordCutIxRcsd, &m->rcsd.base, "rsd");
	return 0;
}
cp_t P2CompactProxyGetRequestRecordPcutClose(P2CompactProxyGetRequestRecordPcut *m)
{
	dve(P2CompactProxyGetRequestRecordPcutValid(m));

	PcutSubSet(&m->base, kP2CompactProxyGetRequestRecordCutIxTsa, NULL, NULL);
	ifer(P2TsaPcutClose(&m->tsa));

	PcutSubSet(&m->base, kP2CompactProxyGetRequestRecordCutIxRsd, NULL, NULL);
	ifer(P2RsdPcutClose(&m->rsd));

	PcutSubSet(&m->base, kP2CompactProxyGetRequestRecordCutIxRcsd, NULL, NULL);
	ifer(P2RcsdPcutClose(&m->rcsd));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2CompactProxyGetRequestRecordPcutValid(const P2CompactProxyGetRequestRecordPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2TsaPcutValid(&m->tsa));
	ifer(P2RsdPcutValid(&m->rsd));
	ifer(P2RcsdPcutValid(&m->rcsd));
	return 0;
}


cp_t P2CompactProxyGetRequestRecordPcutOpenBase(Pcut *base)
{
	P2CompactProxyGetRequestRecordPcut *m = (P2CompactProxyGetRequestRecordPcut*)base;
	return P2CompactProxyGetRequestRecordPcutOpen(m);
}
cp_t P2CompactProxyGetRequestRecordPcutCloseBase(Pcut *base)
{
	P2CompactProxyGetRequestRecordPcut *m = (P2CompactProxyGetRequestRecordPcut*)base;
	return P2CompactProxyGetRequestRecordPcutClose(m);
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
cp_t P2CompactProxyGetRequestRecordTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2CompactProxyGetRequestRecordTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
