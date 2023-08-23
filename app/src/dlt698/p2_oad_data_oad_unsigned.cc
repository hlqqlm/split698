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

20230821-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的
为了描述SetThenGetRequestNormalList中的sequence of，先描述oad + data + oad + unsigned
*/
#include "p2_oad_data_oad_unsigned.h"
#include "p2_oad_data_oad_unsigned.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2OadDataOadUnsignedCutNum)


// {{{ write_oad
static int LenWriteoad(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadUnsignedWriteoadSize; }
static int OffsetWriteoad(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadUnsignedWriteoadOffset; }
static cp_t ValidWriteoad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ data
#define LenData			PcutItemLenBySub
static int OffsetData(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadUnsignedDataOffset; }
#define ValidData			PcutItemValidBySub
//}}}


// {{{ read_oad
static int LenReadoad(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadUnsignedReadoadSize; }
#define OffsetReadoad			PcutItemOffsetDef
//static int OffsetReadoad(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadUnsignedOadOffset; }
static cp_t ValidReadoad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ unsigned
static int LenUnsigned(Pcut *cut, int ix, const char *whole) { return kP2OadDataOadUnsignedUnsignedSize; }
#define OffsetUnsigned			PcutItemOffsetDef
static cp_t ValidUnsigned(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2OadDataOadUnsignedNameWriteoad, LenWriteoad, OffsetWriteoad, ValidWriteoad, NULL },
	{ kP2OadDataOadUnsignedNameData, LenData, OffsetData, ValidData, NULL },
	{ kP2OadDataOadUnsignedNameReadoad, LenReadoad, OffsetReadoad, ValidReadoad, NULL },
	{ kP2OadDataOadUnsignedNameUnsigned, LenUnsigned, OffsetUnsigned, ValidUnsigned, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadDataOadUnsignedCutIxWriteoad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadDataOadUnsignedCutIxData]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadDataOadUnsignedCutIxReadoad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadDataOadUnsignedCutIxUnsigned]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2OadDataOadUnsignedPcutOpen(P2OadDataOadUnsignedPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2DataChoicePcutOpen(&m->data_cut));
	PcutSubSet(&m->base, kP2OadDataOadUnsignedCutIxData, &m->data_cut.choice.base, NULL);
	return 0;
}
cp_t P2OadDataOadUnsignedPcutClose(P2OadDataOadUnsignedPcut *m)
{
	dve(P2OadDataOadUnsignedPcutValid(m));

	PcutSubSet(&m->base, kP2OadDataOadUnsignedCutIxData, NULL, NULL);
	ifer(P2DataChoicePcutClose(&m->data_cut));

	ifer(PcutClose(&m->base));

	return 0;
}
cp_t P2OadDataOadUnsignedPcutValid(const P2OadDataOadUnsignedPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2DataChoicePcutValid(&m->data_cut));
	return 0;
}


cp_t P2OadDataOadUnsignedPcutOpenBase(Pcut *base)
{
	P2OadDataOadUnsignedPcut *m = (P2OadDataOadUnsignedPcut*)base;
	return P2OadDataOadUnsignedPcutOpen(m);
}
cp_t P2OadDataOadUnsignedPcutCloseBase(Pcut *base)
{
	P2OadDataOadUnsignedPcut *m = (P2OadDataOadUnsignedPcut*)base;
	return P2OadDataOadUnsignedPcutClose(m);
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
cp_t P2OadDataOadUnsignedTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2OadDataOadUnsignedTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


