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

用来描述dlt698_45中的SignatureSecurityS
*/
#include <string.h>

#include "p2_signature_security.h"
#include "p2_signature_security.xcp.h"


#define TEST_EN					(0)
#define PRINT_FILL_IN_TEST_EN	(0)
#define kThisCutNum				(kP2SignatureSecurityCutNum)


// {{{ ciphertext2
#define LenCiphertext2		PcutItemLenBySub
static int OffsetCiphertext2(Pcut *cut, int ix, const char *whole) { return kP2SignatureSecurityCiphertext2Offset; }
#define ValidCiphertext2	PcutItemValidBySub
//}}}


// {{{ sign2
#define LenSign2			PcutItemLenBySub
#define OffsetSign2			PcutItemOffsetDef
#define ValidSign2			PcutItemValidBySub
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2SignatureSecurityNameCiphertext2, LenCiphertext2, OffsetCiphertext2, ValidCiphertext2, NULL },
	{ kP2SignatureSecurityNameSign2, LenSign2, OffsetSign2, ValidSign2, NULL },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SignatureSecurityCutIxCiphertext2]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2SignatureSecurityCutIxSign2]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}

cp_t P2SignatureSecurityPcutOpen(P2SignatureSecurityPcut *m)
{
	ifer(P2OctetStringPcutOpen(&m->ciphertext2));
	ifer(P2OctetStringPcutOpen(&m->sign2));

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));

	PcutSubSet(&m->base, kP2SignatureSecurityCutIxCiphertext2, &m->ciphertext2.base, NULL);
	PcutSubSet(&m->base, kP2SignatureSecurityCutIxSign2, &m->sign2.base, NULL);
	return 0;
}
cp_t P2SignatureSecurityPcutClose(P2SignatureSecurityPcut *m)
{
	dve(P2SignatureSecurityPcutValid(m));

	PcutSubSet(&m->base, kP2SignatureSecurityCutIxSign2, NULL, NULL);
	PcutSubSet(&m->base, kP2SignatureSecurityCutIxCiphertext2, NULL, NULL);

	ifer(PcutClose(&m->base));

	ifer(P2OctetStringPcutClose(&m->sign2));
	ifer(P2OctetStringPcutClose(&m->ciphertext2));
	return 0;
}
cp_t P2SignatureSecurityPcutValid(const P2SignatureSecurityPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}


cp_t P2SignatureSecurityPcutOpenBase(Pcut *base)
{
	P2SignatureSecurityPcut *m = (P2SignatureSecurityPcut*)base;
	return P2SignatureSecurityPcutOpen(m);
}
cp_t P2SignatureSecurityPcutCloseBase(Pcut *base)
{
	P2SignatureSecurityPcut *m = (P2SignatureSecurityPcut*)base;
	return P2SignatureSecurityPcutClose(m);
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
cp_t P2SignatureSecurityTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2SignatureSecurityTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


