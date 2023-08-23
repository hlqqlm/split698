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

oad
*/

#include "p2_oad.h"
#include "p2_oad.xcp.h"
// #define this_file_id	0x2a5cd78a  // echo -n dlt698_45_double_long.c | rhash --simple -


#define TEST_EN					(0)
#define kThisDatatype			(kDlt698DataTypeOad)
#define kThisCutNum				(kP2OadCutNum)


//{{{ main
static int LenMain(Pcut *cut, int ix, const char *whole) { return kP2OadSize; }
static int OffsetMain(Pcut *cut, int ix, const char *whole) { return kP2OadOffset; }
static cp_t ValidMain(Pcut *cut, int ix, const char *whole) 
{
	return 0;
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2OadName, LenMain, OffsetMain, ValidMain, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2OadCutIxMain]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2OadPcutOpen(P2OadPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	return 0;
}
cp_t P2OadPcutClose(P2OadPcut *m)
{
	dve(P2OadPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2OadPcutValid(const P2OadPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}

cp_t P2OadPcutOpenBase(Pcut *base)
{
	P2OadPcut *m = (P2OadPcut*)base;
	return P2OadPcutOpen(m);
}
cp_t P2OadPcutCloseBase(Pcut *base)
{
	P2OadPcut *m = (P2OadPcut*)base;
	return P2OadPcutClose(m);
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
cp_t P2OadTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2OadTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


