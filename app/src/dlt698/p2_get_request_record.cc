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

20230822-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的GetRequestRecord
*/

// for test

#include "p2_get_request_record.h"
#include "p2_get_request_record.xcp.h"


#define TEST_EN						(0)
#define TEST_PRINT_FILL_EN			(0)
#define kThisCutNum					(kP2GetRequestRecordCutNum)


// {{{ piid
static int LenPiid(Pcut *cut, int ix, const char *whole) { return kP2GetRequestRecordPiidSize; }
static int OffsetPiid(Pcut *cut, int ix, const char *whole) { return kP2GetRequestRecordPiidOffset; }
static cp_t ValidPiid(Pcut *cut, int ix, const char *whole) { return 0; }
//}}}


//{{{ get_record
#define LenGetRecord		PcutItemLenBySub
static int OffsetGetRecord(Pcut *cut, int ix, const char *whole) { return kP2GetRequestRecordGetRecordOffset; }
#define ValidGetRecord	PcutItemValidBySub
//}}}
 

//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2GetRequestRecordNamePiid, LenPiid, OffsetPiid, ValidPiid, NULL },
	{ kP2GetRequestRecordNameGetRecord, LenGetRecord, OffsetGetRecord, ValidGetRecord, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetRequestRecordCutIxPiid]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetRequestRecordCutIxGetRecord]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2GetRequestRecordPcutOpen(P2GetRequestRecordPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2GetRecordPcutOpen(&m->get_record_cut));
	PcutSubSet(&m->base, kP2GetRequestRecordCutIxGetRecord, &m->get_record_cut.base, kP2GetRequestRecordNameGetRecord);
	return 0;
}
cp_t P2GetRequestRecordPcutClose(P2GetRequestRecordPcut *m)
{
	dve(P2GetRequestRecordPcutValid(m));

	PcutSubSet(&m->base, kP2GetRequestRecordCutIxGetRecord, NULL, NULL);
	ifer(P2GetRecordPcutClose(&m->get_record_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2GetRequestRecordPcutValid(const P2GetRequestRecordPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2GetRecordPcutValid(&m->get_record_cut));
	return 0;
}


cp_t P2GetRequestRecordPcutOpenBase(Pcut *base)
{
	P2GetRequestRecordPcut *m = (P2GetRequestRecordPcut*)base;
	return P2GetRequestRecordPcutOpen(m);
}
cp_t P2GetRequestRecordPcutCloseBase(Pcut *base)
{
	P2GetRequestRecordPcut *m = (P2GetRequestRecordPcut*)base;
	return P2GetRequestRecordPcutClose(m);
}
//}}}


//{{{ test
#if TEST_EN > 0

static cp_t TestPfill(void)
{
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPfill,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2GetRequestRecordTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetRequestRecordTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

