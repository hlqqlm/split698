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

20230824-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

用来描述dlt698_45中的SecurityData
*/
#include <string.h>

#include "p2_security_data.h"
#include "p2_security_data.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2SecurityDataCutNum)

// {{{ random
#define LenRandom		PcutItemLenBySub
static int OffsetRandom(Pcut *cut, int ix, const char *whole) { return kP2SecurityDataRandomOffset; }
#define ValidRandom	PcutItemValidBySub
//}}}


// {{{ sign
#define LenSign				PcutItemLenBySub
#define OffsetSign			PcutItemOffsetDef
#define ValidSign			PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2SecurityDataNameRandom, LenRandom, OffsetRandom, ValidRandom, NULL },
	{ kP2SecurityDataNameSign, LenSign, OffsetSign, ValidSign, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SecurityDataCutIxRandom]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SecurityDataCutIxSign]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SecurityDataPcutOpen(P2SecurityDataPcut *m)
{

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2RnPcutOpen(&m->random));
	PcutSubSet(&m->base, kP2SecurityDataCutIxRandom, &m->random.base, NULL);

	ifer(P2OctetStringPcutOpen(&m->sign));
	PcutSubSet(&m->base, kP2SecurityDataCutIxSign, &m->sign.base, NULL);
	return 0;
}
cp_t P2SecurityDataPcutClose(P2SecurityDataPcut *m)
{
	dve(P2SecurityDataPcutValid(m));

	PcutSubSet(&m->base, kP2SecurityDataCutIxRandom, NULL, NULL);
	ifer(P2RnPcutClose(&m->random));

	PcutSubSet(&m->base, kP2SecurityDataCutIxSign, NULL, NULL);
	ifer(P2OctetStringPcutClose(&m->sign));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2SecurityDataPcutValid(const P2SecurityDataPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2RnPcutValid(&m->random));
	ifer(P2OctetStringPcutValid(&m->sign));
	return 0;
}


cp_t P2SecurityDataPcutOpenBase(Pcut *base)
{
	P2SecurityDataPcut *m = (P2SecurityDataPcut*)base;
	return P2SecurityDataPcutOpen(m);
}
cp_t P2SecurityDataPcutCloseBase(Pcut *base)
{
	P2SecurityDataPcut *m = (P2SecurityDataPcut*)base;
	return P2SecurityDataPcutClose(m);
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
cp_t P2SecurityDataTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SecurityDataTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


