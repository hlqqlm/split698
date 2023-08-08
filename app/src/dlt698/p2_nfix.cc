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

20230522-----------------------------------------------------------------------
huanglin 创建.
DLT698_45 报文解析: n个固定长度的内容。用在解析sequence_of
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qmem.h"
#include "a_xdr.h"
#include "p2_nfix.h"
#include "p2_nfix.xcp.h"
//#define this_file_id	0x8df81206		// echo -n dlt698_45_nfix.c | rhash --simple -


#define TEST_EN				(0)



// {{{ one
#define TO_DERIVE(_part)		(P2NfixPcut*)(_part)

static int LenOne(Pcut *part, int ix, const char *whole) 
{ 
	const P2NfixPcut *derive = (P2NfixPcut*)part;
	return derive->one_size;
}
static int OffsetOne(Pcut *part, int ix, const char *whole) 
{ 
	const P2NfixPcut *derive = (P2NfixPcut*)part;
	dvb(ix < derive->num);
	const int offset = ix * (derive->one_size);
	return offset;
}
static cp_t ValidOne(Pcut *part, int ix, const char *whole) 
{ 
	return 0; 
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kPartFixOne = 
	// name len offset valid
	{ "one", LenOne, OffsetOne, ValidOne };
	

static const PcutItem kPartItemOnePattern = PCUT_ITEM_NO_SUB(&kPartFixOne);
static void PcutItemsInit(PcutItem *items, int num)
{
	for (int i = 0; i < num; ++i)
		memcpy(&items[i], &kPartItemOnePattern, sizeof(kPartItemOnePattern));
}
static cp_t SetName(P2NfixPcut *m, int num, const char *name) 
{
	for (int i = 0; i < num; ++i)
		ifer(PcutNameSetIx(&m->base, i, name));
	return 0;
}

cp_t P2NfixPcutOpen(P2NfixPcut *m, int num, int one_size, const char *name)
{
	dvb(0 <= num);
	dvb(0 <= one_size);

	m->num = num;
	m->one_size = one_size;

	const int size = num * sizeof(PcutItem);
	m->items = (PcutItem *)qmalloc(size);
	ifbr(NULL != m->items);

	PcutItemsInit(m->items, num);
	ifer(PcutOpen(&m->base, m->items, num));
	ifer(SetName(m, num, name));
	return 0;
}
cp_t P2NfixPcutClose(P2NfixPcut *m)
{
	dve(P2NfixPcutValid(m));

	ifer(SetName(m, m->num, NULL));
	ifer(PcutClose(&m->base));
	qfree(m->items);
	m->items = NULL;
	return 0;
}
cp_t P2NfixPcutValid(const P2NfixPcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(NULL != m->items);
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcutNfixOad(void)
{
	// 3个oad
	// 00 10 02 00 20 00 02 00 20 04 02 00
	const char whole[] = "\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00";
	const int whole_size = sizeof(whole) - 1;
	
	P2NfixPcut nfix = kP2NfixPcutDef;
	ifer(P2NfixPcutOpen(&nfix, 3, 4, "oad"));
	Pcut * const m = &nfix.base;		

	ifer(PcutValid(m));

	ifer(PcutIndexValid(m, 0));
	ifer(PcutIndexValid(m, 1));
	ifer(PcutIndexValid(m, 2));
	ifbr(0 != PcutIndexValid(m, 3));

	ifbr(4 ==  PcutIxLen(m, 0, whole));
	ifbr(4 ==  PcutIxLen(m, 1, whole));
	ifbr(4 ==  PcutIxLen(m, 2, whole));
	ifbr(12 ==  PcutIxLen(m, kPcutIxAll, whole));
	ifbr(whole_size ==  PcutIxLen(m, kPcutIxAll, whole));
	

	ifbr(0 == PcutIxOffset(m, 0, whole));
	ifbr(4 == PcutIxOffset(m, 1, whole));
	ifbr(12 == PcutIxOffset(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, 0, whole));
	ifer(PcutIxValid(m, 1, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	ifer(P2NfixPcutClose(&nfix));
	return 0;
}
static cp_t TestQpack(void)
{
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPcutNfixOad,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2NfixTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2NfixTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

