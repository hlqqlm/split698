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

20230730-----------------------------------------------------------------------
Huanglin创建.
*/
#include "gtest/gtest.h"
#include "fileid.h"
#include "qcp.h"
#include "p2_addr.h"
#include "p2_addr_ut.xcp.h"
// #include "qstr_ut.xcp.h"
// FILEID_REGISTER();
//#define this_file_id	0x3627eae0	// echo -n p2_addr_ut.cc | rhash --simple -


#define PRINT_FILL_IN_TEST_EN	(1)


static const FileidItem kFileidItems[] = {
#include "id_filename_table_uniq.h"
	    //#include "../libctlcomm/id_filename_table_uniq.h"
	    //#include "../libctl698/id_filename_table_uniq.h"
};
static const int kFileidNum = sizeof(kFileidItems) / sizeof(kFileidItems[0]);
static const FileidTable kFileidTable = {kFileidItems, kFileidNum};
const FileidTable *MainFileidTable() { return &kFileidTable; }

namespace {


static cp_t TestPcut(void)
{
	P2AddrPcut qpart = kP2AddrPcutDef;
	Pcut * const m = &qpart.base;
	ifer(P2AddrPcutOpen(&qpart));
	ifer(PcutValid(m));

	// index valid
	ifer(PcutIndexAllValid(m, 0));
	ifer(PcutIndexAllValid(m, kP2AddrCutNum - 1));
	ifer(PcutIndexAllValid(m, kPcutIxAll));		// 表示所有部分
	ifbr(0 != PcutIndexValid(m, kP2AddrCutNum));
	
	// 45 01 23 45 67 89 0a 00
	// (0)45  (1)   (2)01 23 45 67 89 0a     (3)00
	const char whole[] = "\x45\x01\x23\x45\x67\x89\x0a\x00";
	ifbr(1 ==  PcutIxLen(m, kP2AddrCutIxFeature, whole));
	ifbr(0 ==  PcutIxLen(m, kP2AddrCutIxExternLogic, whole));
	ifbr(6 ==  PcutIxLen(m, kP2AddrCutIxSaNaked, whole));
	ifbr(1 ==  PcutIxLen(m, kP2AddrCutIxCa, whole));
	ifbr(8 ==  PcutIxLen(m, kPcutIxAll, whole));
	
	ifbr(1 == PcutRangeLen(m, kP2AddrCutIxFeature, kP2AddrCutIxExternLogic, whole));
	ifbr(1 == PcutRangeLen(m, kP2AddrCutIxFeature, kP2AddrCutIxSaNaked, whole));
	ifbr(7 == PcutRangeLen(m, kP2AddrCutIxFeature, kP2AddrCutIxCa, whole));
	ifbr(8 == PcutRangeLen(m, 0, kP2AddrCutNum, whole));

	ifbr(0 == PcutIxOffset(m, kP2AddrCutIxFeature, whole));
	ifbr(1 == PcutIxOffset(m, kP2AddrCutIxExternLogic, whole));
	ifbr(1 == PcutIxOffset(m, kP2AddrCutIxSaNaked, whole));
	ifbr(7 == PcutIxOffset(m, kP2AddrCutIxCa, whole));
	ifbr(8 == PcutIxOffset(m, kPcutIxAll, whole));

	ifbr(whole + 0 == PcutIxPtrConst(m, kP2AddrCutIxFeature, whole));
	ifbr(whole + 1 == PcutIxPtrConst(m, kP2AddrCutIxExternLogic, whole));
	ifbr(whole + 1 == PcutIxPtrConst(m, kP2AddrCutIxSaNaked, whole));
	ifbr(whole + 7 == PcutIxPtrConst(m, kP2AddrCutIxCa, whole));
	ifbr(whole + 8 == PcutIxPtrConst(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kP2AddrCutIxFeature, whole));
	ifer(PcutIxValid(m, kP2AddrCutIxExternLogic, whole));
	ifer(PcutIxValid(m, kP2AddrCutIxSaNaked, whole));
	ifer(PcutIxValid(m, kP2AddrCutIxCa, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));


	ifer(P2AddrPcutClose(&qpart));
	return 0;
}

// ../src/dlt698/dlt698_45_addr.cc
TEST(dlt698_45_addr, qpart) {
	EXPECT_EQ(0, TestPcut());
	return;
}



static cp_t TestPfill(void)
{
	/*
	uint8_t sa_type;
	char sa_naked[kP2SaNakedSizeMax];
	uint8_t sa_naked_size;
	uint8_t extern_logic;		// 范围 2…255，如果为0 ~ 1，表示不存在extern_logic
	uint8_t ca;
	*/
	const P2AddrValue value = {kP2AddrSaTypeWildcard, "\x01\x23\x45\x67\x89\x0a", 6, 0, 0};

	P2AddrPfill fill = kP2AddrPfillDef;
	ifer(P2AddrPfillOpen(&fill));
	fill.value = value;

	
	//P2AddrQpackSetData(&pack, 0, kP2AddrSaTypeWildcard, 0);
	//ifer(P2AddrQpackSetSaNaked(&pack, "\x01\x23\x45\x67\x89\x0a", 6));
	
	// 45 01 23 45 67 89 0a 00
	const char result[] = "\x45\x01\x23\x45\x67\x89\x0a\x00";
	const int result_size = sizeof(result) - 1;
	char mem[result_size];

	// 打开注释，可以测试组帧过程的打印
	if (PRINT_FILL_IN_TEST_EN)
		PfillSetPrintFillWithSubs(&fill.base, PfillPrintFillNormal);
	ifer(PfillDo(&fill.base, 0, mem, result_size));
	
	ifbr(0 == memcmp(mem, result, result_size));
	ifbr(result_size == P2AddrCutSize(mem));

	ifer(P2AddrPfillClose(&fill));
	return 0;
}
TEST(dlt698_45_addr, qfill) {
	EXPECT_EQ(0, TestPfill());
	return;
}




}  // namespace








