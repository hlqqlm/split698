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
#define kThisCutNum			(kP2EnumCutNum)

//{{{ part_update_enum
static cp_t UpdateDo(P2EnumPcut *m, uint8_t enum_value);

static cp_t CutUpdateEnum(P2EnumPcut *m, const char *whole)
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
static int LenEnum(Pcut *cut, int ix, const char *whole) { return kP2EnumSize; }
static int OffsetEnum(Pcut *cut, int ix, const char *whole) { return kP2EnumOffset; }
static cp_t ValidEnum(Pcut *cut, int ix, const char *whole) 
{ 
	P2EnumPcut * const eq = (P2EnumPcut*)cut;

	const uint8_t enum_value = P2EnumValue(whole);
	ifer(P2EnumValid(eq->enum_list, eq->enum_num, enum_value));
	// 刷新enum对应的do_table等
	ifer(CutUpdateEnum(eq, whole));
	return 0; 
}
static cp_t ExplainEnum(Pcut *cut, int ix, const char *whole) 
{
	P2EnumPcut * const eq = (P2EnumPcut*)cut;

	const uint8_t enum_value = P2EnumValue(whole);
	const char *str = P2EnumStr(eq->enum_list, eq->enum_num, enum_value);
	dvb(NULL != str);
	qos_printf("%s", str);
	return 0;
}
//}}}


//{{{ cut
// 为了节约内存，const部分集中在一起
// 固定部分
static const PcutItemFix kCutFix[kThisCutNum] = {
	// name len offset valid explain
	{ kP2EnumName, LenEnum, OffsetEnum, ValidEnum, ExplainEnum },
};
	

static const PcutItem kCutItemsPattern[kThisCutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2EnumCutIxEnum]),
};
static void PcutItemsInit(PcutItem items[kThisCutNum])
{
	memcpy(items, kCutItemsPattern, sizeof(kCutItemsPattern));
}


cp_t P2EnumPcutValid(const P2EnumPcut *m)
{
	ifer(PcutValid(&m->base));
	return 0;
}
cp_t P2EnumPcutClose(P2EnumPcut *m)
{
	dve(P2EnumPcutValid(m));

	PcutDoSet(&m->base, kP2EnumCutIxEnum, NULL);
	ifer(PcutClose(&m->base));
	m->enum_do_table = NULL;
	return 0;
}
cp_t P2EnumPcutOpen(P2EnumPcut *m, const char *enum_name, const P2Enum enum_list[], int enum_num)
{
	m->enum_list = enum_list;
	m->enum_num = enum_num;

	PcutItemsInit(m->items);
	ifer(PcutOpen(&m->base, m->items, kThisCutNum));
	ifer(PcutNameSetIx(&m->base, kP2EnumCutIxEnum, enum_name));

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
	PcutDoSet(&m->base, kP2EnumCutIxEnum, doa);
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


//{{{ enum-one
#define kEnumListLen		(256)
static const P2Enum kEnumList[kEnumListLen] = {
	// enumv	name
	{ 0, "0" },
	{ 1, "1" },
	{ 2, "2" },
	{ 3, "3" },
	{ 4, "4" },
	{ 5, "5" },
	{ 6, "6" },
	{ 7, "7" },
	{ 8, "8" },
	{ 9, "9" },
	{ 10, "10" },
	{ 11, "11" },
	{ 12, "12" },
	{ 13, "13" },
	{ 14, "14" },
	{ 15, "15" },
	{ 16, "16" },
	{ 17, "17" },
	{ 18, "18" },
	{ 19, "19" },
	{ 20, "20" },
	{ 21, "21" },
	{ 22, "22" },
	{ 23, "23" },
	{ 24, "24" },
	{ 25, "25" },
	{ 26, "26" },
	{ 27, "27" },
	{ 28, "28" },
	{ 29, "29" },
	{ 30, "30" },
	{ 31, "31" },
	{ 32, "32" },
	{ 33, "33" },
	{ 34, "34" },
	{ 35, "35" },
	{ 36, "36" },
	{ 37, "37" },
	{ 38, "38" },
	{ 39, "39" },
	{ 40, "40" },
	{ 41, "41" },
	{ 42, "42" },
	{ 43, "43" },
	{ 44, "44" },
	{ 45, "45" },
	{ 46, "46" },
	{ 47, "47" },
	{ 48, "48" },
	{ 49, "49" },
	{ 50, "50" },
	{ 51, "51" },
	{ 52, "52" },
	{ 53, "53" },
	{ 54, "54" },
	{ 55, "55" },
	{ 56, "56" },
	{ 57, "57" },
	{ 58, "58" },
	{ 59, "59" },
	{ 60, "60" },
	{ 61, "61" },
	{ 62, "62" },
	{ 63, "63" },
	{ 64, "64" },
	{ 65, "65" },
	{ 66, "66" },
	{ 67, "67" },
	{ 68, "68" },
	{ 69, "69" },
	{ 70, "70" },
	{ 71, "71" },
	{ 72, "72" },
	{ 73, "73" },
	{ 74, "74" },
	{ 75, "75" },
	{ 76, "76" },
	{ 77, "77" },
	{ 78, "78" },
	{ 79, "79" },
	{ 80, "80" },
	{ 81, "81" },
	{ 82, "82" },
	{ 83, "83" },
	{ 84, "84" },
	{ 85, "85" },
	{ 86, "86" },
	{ 87, "87" },
	{ 88, "88" },
	{ 89, "89" },
	{ 90, "90" },
	{ 91, "91" },
	{ 92, "92" },
	{ 93, "93" },
	{ 94, "94" },
	{ 95, "95" },
	{ 96, "96" },
	{ 97, "97" },
	{ 98, "98" },
	{ 99, "99" },
	{ 100, "100" },
	{ 101, "101" },
	{ 102, "102" },
	{ 103, "103" },
	{ 104, "104" },
	{ 105, "105" },
	{ 106, "106" },
	{ 107, "107" },
	{ 108, "108" },
	{ 109, "109" },
	{ 110, "110" },
	{ 111, "111" },
	{ 112, "112" },
	{ 113, "113" },
	{ 114, "114" },
	{ 115, "115" },
	{ 116, "116" },
	{ 117, "117" },
	{ 118, "118" },
	{ 119, "119" },
	{ 120, "120" },
	{ 121, "121" },
	{ 122, "122" },
	{ 123, "123" },
	{ 124, "124" },
	{ 125, "125" },
	{ 126, "126" },
	{ 127, "127" },
	{ 128, "128" },
	{ 129, "129" },
	{ 130, "130" },
	{ 131, "131" },
	{ 132, "132" },
	{ 133, "133" },
	{ 134, "134" },
	{ 135, "135" },
	{ 136, "136" },
	{ 137, "137" },
	{ 138, "138" },
	{ 139, "139" },
	{ 140, "140" },
	{ 141, "141" },
	{ 142, "142" },
	{ 143, "143" },
	{ 144, "144" },
	{ 145, "145" },
	{ 146, "146" },
	{ 147, "147" },
	{ 148, "148" },
	{ 149, "149" },
	{ 150, "150" },
	{ 151, "151" },
	{ 152, "152" },
	{ 153, "153" },
	{ 154, "154" },
	{ 155, "155" },
	{ 156, "156" },
	{ 157, "157" },
	{ 158, "158" },
	{ 159, "159" },
	{ 160, "160" },
	{ 161, "161" },
	{ 162, "162" },
	{ 163, "163" },
	{ 164, "164" },
	{ 165, "165" },
	{ 166, "166" },
	{ 167, "167" },
	{ 168, "168" },
	{ 169, "169" },
	{ 170, "170" },
	{ 171, "171" },
	{ 172, "172" },
	{ 173, "173" },
	{ 174, "174" },
	{ 175, "175" },
	{ 176, "176" },
	{ 177, "177" },
	{ 178, "178" },
	{ 179, "179" },
	{ 180, "180" },
	{ 181, "181" },
	{ 182, "182" },
	{ 183, "183" },
	{ 184, "184" },
	{ 185, "185" },
	{ 186, "186" },
	{ 187, "187" },
	{ 188, "188" },
	{ 189, "189" },
	{ 190, "190" },
	{ 191, "191" },
	{ 192, "192" },
	{ 193, "193" },
	{ 194, "194" },
	{ 195, "195" },
	{ 196, "196" },
	{ 197, "197" },
	{ 198, "198" },
	{ 199, "199" },
	{ 200, "200" },
	{ 201, "201" },
	{ 202, "202" },
	{ 203, "203" },
	{ 204, "204" },
	{ 205, "205" },
	{ 206, "206" },
	{ 207, "207" },
	{ 208, "208" },
	{ 209, "209" },
	{ 210, "210" },
	{ 211, "211" },
	{ 212, "212" },
	{ 213, "213" },
	{ 214, "214" },
	{ 215, "215" },
	{ 216, "216" },
	{ 217, "217" },
	{ 218, "218" },
	{ 219, "219" },
	{ 220, "220" },
	{ 221, "221" },
	{ 222, "222" },
	{ 223, "223" },
	{ 224, "224" },
	{ 225, "225" },
	{ 226, "226" },
	{ 227, "227" },
	{ 228, "228" },
	{ 229, "229" },
	{ 230, "230" },
	{ 231, "231" },
	{ 232, "232" },
	{ 233, "233" },
	{ 234, "234" },
	{ 235, "235" },
	{ 236, "236" },
	{ 237, "237" },
	{ 238, "238" },
	{ 239, "239" },
	{ 240, "240" },
	{ 241, "241" },
	{ 242, "242" },
	{ 243, "243" },
	{ 244, "244" },
	{ 245, "245" },
	{ 246, "246" },
	{ 247, "247" },
	{ 248, "248" },
	{ 249, "249" },
	{ 250, "250" },
	{ 251, "251" },
	{ 252, "252" },
	{ 253, "253" },
	{ 254, "254" },
	{ 255, "255" },
};
cp_t P2EnumOnePcutOpen(P2EnumOnePcut *m)
{
	ifer(P2EnumPcutOpen(&m->base, "enum", kEnumList, kEnumListLen));
	return 0;
}
cp_t P2EnumOnePcutClose(P2EnumOnePcut *m)
{
	ifer(P2EnumPcutClose(&m->base));
	return 0;
}
cp_t P2EnumOnePcutValid(const P2EnumOnePcut *m)
{
	ifer(P2EnumPcutValid(&m->base));
	return 0;
}

cp_t P2EnumOnePcutOpenBase(Pcut *base)
{
	P2EnumOnePcut *m = (P2EnumOnePcut*)base;
	return P2EnumOnePcutOpen(m);
}
cp_t P2EnumOnePcutCloseBase(Pcut *base)
{
	P2EnumOnePcut *m = (P2EnumOnePcut*)base;
	return P2EnumOnePcutClose(m);
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

