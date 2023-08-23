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

用来描述dlt698_45中的
记录型对象属性描述符ROAD（Record Object Attribute Descriptor）
*/
#include <string.h>

#include "p2_road.h"
#include "p2_road.xcp.h"


#define TEST_EN					(0)
#define PRINT_PACK_IN_TEST_EN	(0)
#define kThisCutNum				(kP2RoadCutNum)



// {{{ oad
static int LenOad(Pcut *cut, int ix, const char *whole) { return kP2RoadOadSize; }
static int OffsetOad(Pcut *cut, int ix, const char *whole) { return kP2RoadOadOffset; }
static cp_t ValidOad(Pcut *cut, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


// {{{ sequence_of_oad
#define LenSequenceOfOad		PcutItemLenBySub
static int OffsetSequenceOfOad(Pcut *cut, int ix, const char *whole) { return kP2RoadSequenceOfOadOffset; }
#define ValidSequenceOfOad		PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2RoadNameOad, LenOad, OffsetOad, ValidOad, NULL },
	{ kP2RoadNameSequenceOfOad, LenSequenceOfOad, OffsetSequenceOfOad, ValidSequenceOfOad, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2RoadCutIxOad]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2RoadCutIxSequenceOfOad]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2RoadPcutOpen(P2RoadPcut *m)
{
	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	ifer(P2SequenceOfPcutOpen(&m->sequence_of_oad_cut, OAD_MEM_SIZE, "oad"));
	PcutSubSet(&m->base, kP2RoadCutIxSequenceOfOad, &m->sequence_of_oad_cut.base, "sequence_of_oad");
	return 0;
}
cp_t P2RoadPcutClose(P2RoadPcut *m)
{
	dve(P2RoadPcutValid(m));

	PcutSubSet(&m->base, kP2RoadCutIxSequenceOfOad, NULL, NULL);
	ifer(P2SequenceOfPcutClose(&m->sequence_of_oad_cut));

	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2RoadPcutValid(const P2RoadPcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2SequenceOfPcutValid(&m->sequence_of_oad_cut));
	return 0;
}


cp_t P2RoadPcutOpenBase(Pcut *base)
{
	P2RoadPcut *m = (P2RoadPcut*)base;
	return P2RoadPcutOpen(m);
}
cp_t P2RoadPcutCloseBase(Pcut *base)
{
	P2RoadPcut *m = (P2RoadPcut*)base;
	return P2RoadPcutClose(m);
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
cp_t P2RoadTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2RoadTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}
