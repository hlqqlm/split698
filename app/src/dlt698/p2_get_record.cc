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

用来描述dlt698_45中的GetRecord
*/
#include "p2_get_record.h"
#include "p2_get_record.xcp.h"


#define TEST_EN				(0)
#define kThisCutNum			(kP2GetRecordCutNum)

// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2GetRecordOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2GetRecordOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ rsd
#define LenRsd			PcutItemLenBySub
static int OffsetRsd(Pcut *cut, int ix, const char *whole) { return kP2GetRecordRsdOffset; }
//#define OffsetGetResult			PcutItemOffsetDef
#define ValidRsd			PcutItemValidBySub
//}}}


// {{{ rcsd
#define LenRcsd				PcutItemLenBySub
#define OffsetRcsd			PcutItemOffsetDef
#define ValidRcsd			PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2GetRecordNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2GetRecordNameRsd, LenRsd, OffsetRsd, ValidRsd, NULL },
	{ kP2GetRecordNameRcsd, LenRcsd, OffsetRcsd, ValidRcsd, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetRecordCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetRecordCutIxRsd]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2GetRecordCutIxRcsd]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2GetRecordPcutOpen(P2GetRecordPcut *m)
{

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2RsdPcutOpen(&m->rsd_cut));
	PcutSubSet(&m->base, kP2GetRecordCutIxRsd, &m->rsd_cut.choice.base, NULL);

	ifer(P2RcsdPcutOpen(&m->rcsd_cut));
	PcutSubSet(&m->base, kP2GetRecordCutIxRcsd, &m->rcsd_cut.base, NULL);
	return 0;
}
cp_t P2GetRecordPcutClose(P2GetRecordPcut *m)
{
	dve(P2GetRecordPcutValid(m));

	PcutSubSet(&m->base, kP2GetRecordCutIxRsd, NULL, NULL);
	ifer(P2RsdPcutClose(&m->rsd_cut));

	PcutSubSet(&m->base, kP2GetRecordCutIxRcsd, NULL, NULL);
	ifer(P2RcsdPcutClose(&m->rcsd_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2GetRecordPcutValid(const P2GetRecordPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2RsdPcutValid(&m->rsd_cut));
	ifer(P2RcsdPcutValid(&m->rcsd_cut));
	return 0;
}


cp_t P2GetRecordPcutOpenBase(Pcut *base)
{
	P2GetRecordPcut *m = (P2GetRecordPcut*)base;
	return P2GetRecordPcutOpen(m);
}
cp_t P2GetRecordPcutCloseBase(Pcut *base)
{
	P2GetRecordPcut *m = (P2GetRecordPcut*)base;
	return P2GetRecordPcutClose(m);
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
cp_t P2GetRecordTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetRecordTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
