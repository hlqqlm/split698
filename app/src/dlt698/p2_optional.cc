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

20230423-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 optional 部分报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "p2_optional.h"
#include "p2_optional.xcp.h"
//#define this_file_id	0x63910627		// echo -n dlt698_45_optional.c | rhash --simple -


#define TEST_EN				(0)


// {{{ optional
uint8_t P2OptionalValue(const char *whole)
{
	return whole[kP2OptionalOffset];
}

static int LenOptional(Pcut *part, int ix, const char *whole) { return kP2OptionalSize; }
static int OffsetOptional(Pcut *part, int ix, const char *whole) { return kP2OptionalOffset; }
static cp_t ValidOptional(Pcut *part, int ix, const char *whole) 
{ 
	const uint8_t optional = P2OptionalValue(whole);
	ifbr(0 == optional || 1 == optional);
	return 0; 
}
//}}}


//{{{ var
static void UpdateVarSub(Pcut *part, const char *whole) 
{
	P2OptionalPcut *m = (P2OptionalPcut*)part;
	if (0 == P2OptionalValue(whole))
	{
		PcutSubSet(part, kP2OptionalPartIxVar, NULL, NULL);
		return;
	}

	dve(PcutValid(m->var_part));
	PcutSubSet(part, kP2OptionalPartIxVar, m->var_part, m->var_name);
	return;
}
static int LenVar(Pcut *part, int ix, const char *whole) 
{ 
	UpdateVarSub(part, whole);
	if (0 == P2OptionalValue(whole))
		return 0;
	
	return PcutItemLenBySub(part, ix, whole);
}
static int OffsetVar(Pcut *part, int ix, const char *whole) 
{ 
	UpdateVarSub(part, whole);
	return kP2OptionalVarOffset; 
}
static cp_t ValidVar(Pcut *part, int ix, const char *whole) 
{
	UpdateVarSub(part, whole);
	if (0 == P2OptionalValue(whole))
		return 0;

	return PcutItemValidBySub(part, ix ,whole);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFix[kP2OptionalPartNum] = {
	// name len offset valid explain
	{ "optional", LenOptional, OffsetOptional, ValidOptional, NULL },
	{ "var", LenVar, OffsetVar, ValidVar, NULL },
};
	

static const PcutItem kPartItemsPattern[kP2OptionalPartNum] = {
	PCUT_ITEM_NO_SUB(&kPartFix[kP2OptionalPartIxOptional]),
	PCUT_ITEM_NO_SUB(&kPartFix[kP2OptionalPartIxVar]),
};
static void PcutItemsInit(PcutItem items[kP2OptionalPartNum])
{
	memcpy(items, kPartItemsPattern, sizeof(kPartItemsPattern));
}


cp_t P2OptionalPcutOpen(P2OptionalPcut *m, Pcut *var_part, const char *var_name)
{
	dve(PcutValid(var_part));

	m->var_part = var_part;
	m->var_name = var_name;

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2OptionalPartNum));
	return 0;
}
cp_t P2OptionalPcutClose(P2OptionalPcut *m)
{
	dve(P2OptionalPcutValid(m));

	PcutSubSet(&m->base, kP2OptionalPartIxVar, NULL, NULL);
	ifer(PcutClose(&m->base));
	m->var_part = NULL;
	m->var_name = NULL;
	return 0;
}
cp_t P2OptionalPcutValid(const P2OptionalPcut *m)
{
	ifer(PcutValid(&m->base));
	if (NULL != m->var_part)
		ifer(PcutValid(m->var_part));
	return 0;
}
//}}}


//{{{ fill
cp_t P2OptionalFillItemProcessOptional(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2OptionalFillItemOptional *derive = (P2OptionalFillItemOptional*)fi;

	mem[offset] = derive->optional;
	*fill_size = kP2OptionalSize;
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcut(void)
{
	return 0;
}
static cp_t TestQpack(void)
{
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2OptionalTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2OptionalTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

