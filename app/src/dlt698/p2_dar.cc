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

20230512-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

dar
*/
#include "p2_dar.h"
#include "p2_dar.xcp.h"
//#define this_file_id	0xdced282c  // echo -n dlt698_45_dar.c | rhash --simple -


#define TEST_EN					(0)

//{{{ dar
static int LenDar(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2DarSize;
}
static int OffsetDar(Pcut *cut, int ix, const char *whole) 
{ 
	return kP2DarOffset;
}
static cp_t ValidDar(Pcut *cut, int ix, const char *whole) 
{ 
	const uint8_t dar = whole[kP2DarOffset];
	ifer(P2DarValid(dar));
	return 0; 
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kP2DarCutNum] = {
	// name len offset valid explain
	{ kP2DarName, LenDar, OffsetDar, ValidDar, NULL },
};
	

static const PcutItem kCutItemsPattern[kP2DarCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2DarCutIxDar]),
};
static void PcutItemsInit(PcutItem items[kP2DarCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2DarPcutOpen(P2DarPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2DarCutNum));
	return 0;
}
cp_t P2DarPcutClose(P2DarPcut *m)
{
	dve(P2DarPcutValid(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2DarPcutValid(const P2DarPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}

cp_t P2DarPcutOpenBase(Pcut *base)
{
	P2DarPcut *m = (P2DarPcut*)base;
	return P2DarPcutOpen(m);
}
cp_t P2DarPcutCloseBase(Pcut *base)
{
	P2DarPcut *m = (P2DarPcut*)base;
	return P2DarPcutClose(m);
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
cp_t P2DarTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2DarTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


