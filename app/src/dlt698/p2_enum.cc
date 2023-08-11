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

20230707-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 enum 部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/
#include <string.h>

#include "qos/qmem.h"
#include "qos/qstr.h"

#include "p2_enum.h"
#include "p2_enum.xcp.h"
//#define this_file_id	0x1696bff6	// echo -n dlt698_45_enum.c | rhash --simple -


#define TEST_EN				(0)


//{{{ part_update_enum
static cp_t UpdateDo(P2EnumPcut *m, uint8_t enum_value);

static cp_t PartUpdateEnum(P2EnumPcut *m, const char *whole)
{
	dve(P2EnumPcutValid(m));
	const uint8_t enum_value = P2EnumValue(whole);
	ifer(P2EnumValid(m->enum_list, m->enum_num, enum_value));

	ifer(UpdateDo(m, enum_value));
	return 0;
}
//}}}


// {{{ enumv
int P2Enum2Ix(const P2Enum list[], int num, uint8_t enumv)
{
	for (int i = 0; i < num; ++i)
	{
		const uint8_t curr = list[i].enumv;
		if (enumv == curr)
			return i;		
		// list是从小往大排序的
		if (enumv < curr)
			return -1;
	}

	return -1;
}
uint8_t P2EnumIx2Value(const P2Enum list[], int num, int enum_ix)
{
	dve(P2EnumIxValid(num, enum_ix));
	return list[enum_ix].enumv;
}
cp_t P2EnumIxValid(int num, int enum_ix)
{
	ifbr(0 <= enum_ix);
	ifbr(enum_ix < num);
	return 0;
}
cp_t P2EnumValid(const P2Enum list[], int num, uint8_t enumv)
{
	const int enum_ix = P2Enum2Ix(list, num, enumv);
	ifer(P2EnumIxValid(num, enum_ix));
	return 0;
}
const char *P2EnumStr(const P2Enum list[], int num, uint8_t enumv)
{
	const int enum_ix = P2Enum2Ix(list, num, enumv);
	if (0 != P2EnumIxValid(num, enum_ix))
		return "unknow";
	return list[enum_ix].name;
}

uint8_t P2EnumValue(const char *whole)
{
	return whole[kP2EnumOffset];
}
static int LenEnum(Pcut *part, int ix, const char *whole) { return kP2EnumSize; }
static int OffsetEnum(Pcut *part, int ix, const char *whole) { return kP2EnumOffset; }
static cp_t ValidEnum(Pcut *part, int ix, const char *whole) 
{ 
	P2EnumPcut * const eq = (P2EnumPcut*)part;

	const uint8_t enum_value = P2EnumValue(whole);
	ifer(P2EnumValid(eq->enum_list, eq->enum_num, enum_value));
	// 刷新enum对应的do_table等
	ifer(PartUpdateEnum(eq, whole));
	return 0; 
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2EnumPartNum] = {
	// name len offset valid explain
	{ kP2EnumName, LenEnum, OffsetEnum, ValidEnum, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2EnumPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2EnumPartIxEnum]),
};
static void PcutItemsInit(PcutItem items[kP2EnumPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}


cp_t P2EnumPcutValid(const P2EnumPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
cp_t P2EnumPcutClose(P2EnumPcut *m)
{
	dve(P2EnumPcutValid(m));

	PcutDoSet(&m->base, kP2EnumPartIxEnum, NULL);
	ifer(PcutClose(&m->base));
	m->enum_do_table = NULL;
	return 0;
}
cp_t P2EnumPcutOpen(P2EnumPcut *m, const char *enum_name, const P2Enum enum_list[], int enum_num)
{
	m->enum_list = enum_list;
	m->enum_num = enum_num;

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2EnumPartNum));
	ifer(PcutNameSetIx(&m->base, kP2EnumPartIxEnum, enum_name));

	m->enum_do_table = NULL;
	return 0;
}
// 需要刷新enum对应的do对象：
// 1 enum_value变化
static cp_t UpdateDo(P2EnumPcut *m, uint8_t enum_value)
{
	// 当前还没有选择任何choice
	if (kP2EnumInvalid == enum_value)
		return 0;

	dve(P2EnumValid(m->enum_list, m->enum_num, enum_value));
	const int enum_ix = P2Enum2Ix(m->enum_list, m->enum_num, enum_value);
	ifer(P2EnumIxValid(m->enum_num, enum_ix));
	
	Pdo * const doa = (NULL == m->enum_do_table) ? NULL : m->enum_do_table[enum_ix];
	PcutDoSet(&m->base, kP2EnumPartIxEnum, doa);
	return 0;
}	


cp_t P2EnumDoTableSet(P2EnumPcut *m, Pdo * const *enum_do_table)
{
	dve(P2EnumPcutValid(m));
	if (NULL != enum_do_table)
	{
		dve(PdoTableValid(enum_do_table, m->enum_num));
	}

	m->enum_do_table = enum_do_table;
	return 0;
}
//}}}


//{{{ fill
cp_t P2EnumFillItemProcessEnum(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2EnumFillItemEnum *derive = (P2EnumFillItemEnum*)fi;

	mem[offset] = derive->enum_value;
	*fill_size = kP2EnumSize;
	return 0;
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
void P2EnumTest(void)
{
	QtestTest(__FILE__, TEST_RUN_TIMES, kTestItem, kTestItemNum);
}

#else
void P2EnumTest(void)
{
	return;
}
#endif
//}}}

