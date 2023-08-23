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

20230811-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 Null 报文解析
*/
#include "p2_null.h"
#include "p2_null.xcp.h"


#define TEST_EN					(0)

//{{{ null
static int LenNull(Pcut *part, int ix, const char *whole) 
{ 
	return kP2NullSize;
}
static int OffsetNull(Pcut *part, int ix, const char *whole) 
{ 
	return kP2NullOffset;
}
static cp_t ValidNull(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kP2NullCutNum] = {
	// name len offset valid explain
	{ kP2NullName, LenNull, OffsetNull, ValidNull, NULL },
};
	

static const PcutItem kCutItemsPattern[kP2NullCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2NullCutIxNull]),
};
static void PcutItemsInit(PcutItem items[kP2NullCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2NullPcutOpen(P2NullPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2NullCutNum));
	return 0;
}
cp_t P2NullPcutClose(P2NullPcut *m)
{
	dve(P2NullPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2NullPcutValid(const P2NullPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}

cp_t P2NullPcutOpenBase(Pcut *base)
{
	P2NullPcut *m = (P2NullPcut*)base;
	return P2NullPcutOpen(m);
}
cp_t P2NullPcutCloseBase(Pcut *base)
{
	P2NullPcut *m = (P2NullPcut*)base;
	return P2NullPcutClose(m);
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
cp_t P2NullTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2NullTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


