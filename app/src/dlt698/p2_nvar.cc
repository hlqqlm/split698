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
DLT698_45 报文解析: n个变化长度的内容。用在解析sequence_of var len
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "qos/qmem.h"
#include "a_xdr.h"
#include "p2_nvar.h"
#include "p2_nvar.xcp.h"
//#define this_file_id	0x253b28bd		// echo -n dlt698_45_nvar.c | rhash --simple -


#define TEST_EN				(0)



// {{{ one
#define TO_DERIVE(_part)		(P2NvarPcut*)(_part)
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutVarOne = 
	// name len offset valid explain
	{ "one", PcutItemLenBySub, PcutItemOffsetByPreviousLen, PcutItemValidBySub, NULL };
	

static const PcutItem kCutItemOnePattern = PCUT_ITEM_NO_SUB(&kCutVarOne);
static void PcutItemsInit(PcutItem *items, int num)
{
	for (int i = 0; i < num; ++i)
		memcpy(&items[i], &kCutItemOnePattern, sizeof(kCutItemOnePattern));
}
int P2NvarPcutNum(const P2NvarPcut *m)
{
	return m->num;
}
cp_t P2NvarPcutSetSub(P2NvarPcut *m, int ix, Pcut *sub_cut, const char *name) 
{
	dve(P2NvarPcutValid(m));

	ifbr(0 <= ix);
	ifbr(ix < m->num);

	PcutSubSet(&m->base, ix, sub_cut, name);
	return 0;
}
static cp_t SetSub(P2NvarPcut *m, int num, Pcut *sub_cut, const char *name) 
{
	for (int i = 0; i < num; ++i)
		PcutSubSet(&m->base, i, sub_cut, name);
	return 0;
}

cp_t P2NvarPcutOpen(P2NvarPcut *m, int num, Pcut *sub_cut, const char *name)
{
	dvb(0 <= num);
	dvb(NULL != sub_cut);
	dve(PcutValid(sub_cut));

	m->num = num;

	const int size = num * sizeof(PcutItem);
	m->items = (PcutItem *)qmalloc(size);
	ifbr(NULL != m->items);

	PcutItemsInit(m->items, num);
	ifer(PcutOpen(&m->base, m->items, num));
	ifer(SetSub(m, num, sub_cut, name));
	return 0;
}
cp_t P2NvarPcutClose(P2NvarPcut *m)
{
	dve(P2NvarPcutValid(m));

	ifer(SetSub(m, m->num, NULL, NULL));
	ifer(PcutClose(&m->base));
	qfree(m->items);
	m->items = NULL;
	return 0;
}
cp_t P2NvarPcutValid(const P2NvarPcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(NULL != m->items);
	ifbr(0 <= m->num);
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcutNvarOad(void)
{
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcutNvarOad,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2NvarTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2NvarTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

