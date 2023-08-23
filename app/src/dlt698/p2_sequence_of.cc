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
DLT698_45 sequence of 部分报文解析

*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "a_xdr.h"
#include "p2_sequence_of.h"
#include "p2_sequence_of.xcp.h"
//#define this_file_id	0xddeef74d		// echo -n dlt698_45_sequence_of.c | rhash --simple -


#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)
#define kThisCutNum				(kP2SequenceOfCutNum)

//{{{ nfix
static cp_t NfixOpen(P2SequenceOfPcut *m, int num)
{
	dve(P2SequenceOfPcutValid(m));
	dvb(0 == m->nfix_open_cnt);
	++m->nfix_open_cnt;
	ifer(P2NfixPcutOpen(&m->nfix_cut, num, m->one_size, m->content_name));
	PcutSubSet(&m->base, kP2SequenceOfCutIxContent, &m->nfix_cut.base, NULL);
	return 0;
}
static cp_t NfixClose(P2SequenceOfPcut *m)
{
	dve(P2SequenceOfPcutValid(m));
	if (0 == m->nfix_open_cnt)
		return 0;

	dvb(1 == m->nfix_open_cnt);
	--m->nfix_open_cnt;
	PcutSubSet(&m->base, kP2SequenceOfCutIxContent, NULL, NULL);
	ifer(P2NfixPcutClose(&m->nfix_cut));
	return 0;
}
//}}}


// {{{ num
int P2SequenceOfNum(const char *whole)
{
	return VariableLenIntValue(whole);
}
static int NumSize(const char *whole)
{
	return VariableLenIntByteNum(*whole);
}

static int LenNum(Pcut *part, int ix, const char *whole) { return NumSize(whole); }
static int OffsetNum(Pcut *part, int ix, const char *whole) { return kP2SequenceOfNumOffset; }
static cp_t ValidNum(Pcut *part, int ix, const char *whole) 
{ 
	ifbr(kVariableLenInvalidLen != VariableLenIntValue(whole));
	return 0; 
}
//}}}


//{{{ content
static int LenContent(Pcut *part, int ix, const char *whole) 
{ 
	P2SequenceOfPcut *derive = (P2SequenceOfPcut*)part;
	const int num = P2SequenceOfNum(whole);
	ifed(NfixClose(derive));
	ifed(NfixOpen(derive, num));
	return PcutItemLenBySub(part, ix, whole);
}
static int OffsetContent(Pcut *part, int ix, const char *whole) 
{ 
	return P2SequenceOfContentOffset(whole);
}
static cp_t ValidContent(Pcut *part, int ix, const char *whole)
{
	P2SequenceOfPcut *derive = (P2SequenceOfPcut*)part;
	const int num = P2SequenceOfNum(whole);
	ifed(NfixClose(derive));
	ifed(NfixOpen(derive, num));
	return PcutItemValidBySub(part, ix, whole);
}


cp_t P2SequenceOfContentIxValid(const char *whole, int content_ix)
{
	ifbr(0 <= content_ix);
	ifbr(content_ix < P2SequenceOfNum(whole));
	return 0;
}
int P2SequenceOfContentOffset(const char *whole)
{
	return kP2SequenceOfContentOffset(NumSize(whole));
}
int P2SequenceOfContentIxOffset(int one_size, const char *whole, int content_ix)
{
	dve(P2SequenceOfContentIxValid(whole, content_ix));
	const int content_offset = P2SequenceOfContentOffset(whole);
	return content_offset + (content_ix * one_size);
}
const char *P2SequenceOfContentIx(int one_size, const char *whole, int content_ix)
{
	dve(P2SequenceOfContentIxValid(whole, content_ix));
	return whole + P2SequenceOfContentIxOffset(one_size, whole, content_ix);
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ "num", LenNum, OffsetNum, ValidNum, NULL },
	{ "content", LenContent, OffsetContent, ValidContent, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SequenceOfCutIxNum]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SequenceOfCutIxContent]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SequenceOfPcutOpen(P2SequenceOfPcut *m, int one_size, const char *content_name)
{
	m->one_size = one_size;	
	m->content_name = content_name;

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	dvb(0 == m->nfix_open_cnt);
	return 0;
}
cp_t P2SequenceOfPcutClose(P2SequenceOfPcut *m)
{
	dve(P2SequenceOfPcutValid(m));

	//PcutSubSet(&m->base, kP2SequenceOfCutIxContent, NULL, NULL);
	ifer(NfixClose(m));
	ifer(PcutClose(&m->base));
	return 0;
}
cp_t P2SequenceOfPcutValid(const P2SequenceOfPcut *m)
{
	ifer(PcutValid(&m->base));
	ifbr(0 == m->nfix_open_cnt || 1 == m->nfix_open_cnt);
	if (1 == m->nfix_open_cnt)
		ifer(P2NfixPcutValid(&m->nfix_cut));
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestPcutSequenceOfOad(void)
{
	// 03 00 10 02 00 20 00 02 00 20 04 02 00
	// 3个oad
	const char whole[] = "\x03\x00\x10\x02\x00\x20\x00\x02\x00\x20\x04\x02\x00";
	const int whole_size = sizeof(whole) - 1;

	P2SequenceOfPcut so = kP2SequenceOfPcutDef;
	ifer(P2SequenceOfPcutOpen(&so, 4, "oad"));
	Pcut * const m = &so.base;		

	ifbr(1 == PcutIxLen(m, kP2SequenceOfCutIxNum, whole));
	ifbr(12 == PcutIxLen(m, kP2SequenceOfCutIxContent, whole));
	ifbr(whole_size == PcutIxLen(m, kPcutIxAll, whole));
	ifbr(whole_size == 13);

	ifbr(0 == PcutIxOffset(m, kP2SequenceOfCutIxNum, whole));
	ifbr(1 == PcutIxOffset(m, kP2SequenceOfCutIxContent, whole));
	ifbr(13 == PcutIxOffset(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kP2SequenceOfCutIxNum, whole));
	ifer(PcutIxValid(m, kP2SequenceOfCutIxContent, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	if (PRINT_PART_IN_TEST_EN)
		PcutAllPrint(m, 0, whole);

	ifer(P2SequenceOfPcutClose(&so));
	return 0;
}

static const QtestItem kTestItem[] = {
	TestPcutSequenceOfOad,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2SequenceOfTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SequenceOfTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

