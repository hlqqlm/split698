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
DLT698_45报文解析

用来描述dlt698_45中的SymmetrySecurityS
*/
#include <string.h>

#include "p2_symmetry_security.h"
#include "p2_symmetry_security.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)


// {{{ ciphertext1
#define LenCiphertext1		PcutItemLenBySub
static int OffsetCiphertext1(Pcut *part, int ix, const char *whole) { return kP2SymmetrySecurityCiphertext1Offset; }
#define ValidCiphertext1	PcutItemValidBySub
//}}}


// {{{ sign1
#define LenSign1			PcutItemLenBySub
#define OffsetSign1			PcutItemOffsetDef
#define ValidSign1			PcutItemValidBySub
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kP2SymmetrySecurityCutNum] = {
	// name len offset valid explain
	{ kP2SymmetrySecurityNameCiphertext1, LenCiphertext1, OffsetCiphertext1, ValidCiphertext1, NULL },
	{ kP2SymmetrySecurityNameSign1, LenSign1, OffsetSign1, ValidSign1, NULL },
};
	

static const PcutItem kCutItemsPattern[kP2SymmetrySecurityCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SymmetrySecurityCutIxCiphertext1]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SymmetrySecurityCutIxSign1]),
};
static void PcutItemsInit(PcutItem items[kP2SymmetrySecurityCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SymmetrySecurityPcutOpen(P2SymmetrySecurityPcut *m)
{
	ifer(P2OctetStringPcutOpen(&m->ciphertext1));
	ifer(P2OctetStringPcutOpen(&m->sign1));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kP2SymmetrySecurityCutNum));

	PcutSubSet(&m->base, kP2SymmetrySecurityCutIxCiphertext1, &m->ciphertext1.base, NULL);
	PcutSubSet(&m->base, kP2SymmetrySecurityCutIxSign1, &m->sign1.base, NULL);
	return 0;
}
cp_t P2SymmetrySecurityPcutClose(P2SymmetrySecurityPcut *m)
{
	dve(P2SymmetrySecurityPcutValid(m));

	PcutSubSet(&m->base, kP2SymmetrySecurityCutIxSign1, NULL, NULL);
	PcutSubSet(&m->base, kP2SymmetrySecurityCutIxCiphertext1, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2OctetStringPcutClose(&m->sign1));
	ifer(P2OctetStringPcutClose(&m->ciphertext1));
	return 0;
}
cp_t P2SymmetrySecurityPcutValid(const P2SymmetrySecurityPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2SymmetrySecurityPcutOpenBase(Pcut *base)
{
	P2SymmetrySecurityPcut *m = (P2SymmetrySecurityPcut*)base;
	return P2SymmetrySecurityPcutOpen(m);
}
cp_t P2SymmetrySecurityPcutCloseBase(Pcut *base)
{
	P2SymmetrySecurityPcut *m = (P2SymmetrySecurityPcut*)base;
	return P2SymmetrySecurityPcutClose(m);
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
cp_t P2SymmetrySecurityTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SymmetrySecurityTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


