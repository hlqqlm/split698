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

20230407-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的GetRequest
*/
#include <string.h>

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "p2_get_request_normal.h"
#include "p2_get_request_normal_list.h"
#include "p2_get_request.h"
#include "p2_get_request.xcp.h"
//#define this_file_id	0x08d887d6	// echo -n dlt698_45_get_request.c | rhash --simple -


#define TEST_EN				(0)
#define PRINT_FILL_IN_TEST_EN	(0)

 
// {{{ choice
static const P2Choice kChoiceList[kP2GetRequestChoiceNum] = {
	// choice	name
	{ kP2GetRequestChoiceGetRequestNormal, "get_request_normal" },		// 请求读取一个对象属性 [1] GetRequestNormal，
	{ kP2GetRequestChoiceGetRequestNormalList, "get_request_normal_list" },	// 请求读取若干个对象属性 [2] GetRequestNormalList，
	{ kP2GetRequestChoiceGetRequestRecord, "get_request_record" },		// 请求读取一个记录型对象属性 [3] GetRequestRecord，
	{ kP2GetRequestChoiceGetRequestRecordList, "get_request_record_list" },	// 请求读取若干个记录型对象属性 [4] GetRequestRecordList，
	{ kP2GetRequestChoiceGetRequestNext, "get_request_next" },			// 请求读取分帧传输的下一帧 [5] GetRequestNext，
	{ kP2GetRequestChoiceGetRequestMD5, "get_request_md5" },			// 请求读取一个对象属性的 MD5 值 [6] GetRequestMD5，
	{ kP2GetRequestChoiceGetRequestSimplifyRecord, "get_request_simplify_record" },	// 读取一个精简的记录型对象属性请求 [23] GetRequestSimplifyRecord
};
int P2GetRequestChoice2Ix(uint8_t choice)
{
	return P2Choice2Ix(kChoiceList, kP2GetRequestChoiceNum, choice);
}
uint8_t P2GetRequestChoiceIx2Value(int choice_ix)
{
	return P2ChoiceIx2Value(kChoiceList, kP2GetRequestChoiceNum, choice_ix);
}
cp_t P2GetRequestChoiceIxValid(int choice_ix)
{
	return P2ChoiceIxValid(kP2GetRequestChoiceNum, choice_ix);
}
cp_t P2GetRequestChoiceValid(uint8_t choice)
{
	return P2ChoiceValid(kChoiceList, kP2GetRequestChoiceNum, choice);
}

uint8_t P2GetRequestChoice(const char *whole)
{
	const char *mem = whole + kP2GetRequestChoiceOffset;
	return *mem;
}
//}}}


//{{{ cut
static const P2GetRequestNormalPcut kGetRequestNormalPcutDefVar = kP2GetRequestNormalPcutDef;
static const P2GetRequestNormalListPcut kGetRequestNormalListPcutDefVar = kP2GetRequestNormalListPcutDef;
static const PcutFactoryInfo kVarFactoryInfoList[kP2GetRequestChoiceNum] = {
	{ kP2GetRequestNormalName, sizeof(P2GetRequestNormalPcut), &kGetRequestNormalPcutDefVar
		, P2GetRequestNormalPcutOpenBase, P2GetRequestNormalPcutCloseBase }, // 请求读取一个对象属性 [1] GetRequestNormal，
	{ kP2GetRequestNormalListName, sizeof(P2GetRequestNormalListPcut), &kGetRequestNormalListPcutDefVar
		, P2GetRequestNormalListPcutOpenBase, P2GetRequestNormalListPcutCloseBase }, // 请求读取若干个对象属性 [2] GetRequestNormalList，
	// PcutFactoryInfoDef, 

	kPcutFactoryInfoDef("GetRequestRecord"), // 请求读取一个记录型对象属性 [3] GetRequestRecord，
	kPcutFactoryInfoDef("GetRequestRecordList"), // 请求读取若干个记录型对象属性 [4] GetRequestRecordList，
	kPcutFactoryInfoDef("GetRequestNext"), // 请求读取分帧传输的下一帧 [5] GetRequestNext，
	kPcutFactoryInfoDef("GetRequestMD5"), // 请求读取一个对象属性的 MD5 值 [6] GetRequestMD5，
	kPcutFactoryInfoDef("GetRequestSimplifyRecord"), // 读取一个精简的记录型对象属性请求 [23] GetRequestSimplifyRecord
};
cp_t P2GetRequestPcutOpen(P2GetRequestPcut *m)
{
	return P2ChoicePcutOpen(&m->choice, kP2GetRequestChoiceName, kChoiceList, kP2GetRequestChoiceNum, kVarFactoryInfoList);
}
cp_t P2GetRequestPcutClose(P2GetRequestPcut *m)
{
	dve(P2GetRequestPcutValid(m));
	ifer(P2ChoicePcutClose(&m->choice));
	return 0;
}
cp_t P2GetRequestPcutValid(const P2GetRequestPcut *m)
{
	ifer(P2ChoicePcutValid(&m->choice));
	return 0;
}


cp_t P2GetRequestPcutOpenBase(Pcut *base)
{
	P2GetRequestPcut *m = (P2GetRequestPcut*)base;
	return P2GetRequestPcutOpen(m);
}
cp_t P2GetRequestPcutCloseBase(Pcut *base)
{
	P2GetRequestPcut *m = (P2GetRequestPcut*)base;
	return P2GetRequestPcutClose(m);
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
	P2GetRequestNormalQpack get_request_normal_pack = kP2GetRequestNormalQpackDef;
	ifer(P2GetRequestNormalQpackOpen(&get_request_normal_pack));
	{
	const P2GetRequestNormalValue value = {0, 0x40010200};
	// 下面两个方法等效，都是设置value的值
	ifer(QpackValueCopy(&get_request_normal_pack.base, &value, sizeof(value)));
	get_request_normal_pack.value = value;
	}

	P2GetRequestQpack get_request_pack = kP2GetRequestQpackDef;
	ifer(P2GetRequestQpackOpen(&get_request_pack));
	ifer(P2ChoiceQpackSetChoice(&get_request_pack.base, kP2GetRequestChoiceGetRequestNormal, &get_request_normal_pack.base));
	Qpack * const pack = &get_request_pack.base.base;

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char result[] = "\x01\x00\x40\x01\x02\x00";
	const int result_size = sizeof(result) - 1; //打印出来是8
	char mem[10] = { 0 };

	// 打开注释，可以测试组帧过程的打印
	if (PRINT_FILL_IN_TEST_EN)
		QpackSetPrintFill(pack, QpackPrintFillNormal);

	ifer(QpackDo(pack, mem, sizeof(mem)));
	const int fill_size = QpackIxLen(pack, kQpackIxAll);
	// qos_printf("fill_size=%d result_size=%d\n", fill_size, result_size);
	ifbr(fill_size == result_size);
	ifbr(0 == memcmp(mem, result, result_size));
 
	ifer(P2GetRequestQpackClose(&get_request_pack));
	ifer(P2GetRequestNormalQpackClose(&get_request_normal_pack));
	return 0;
}



static const QtestItem kTestItem[] = {
	TestPcut,
	TestQpack,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2GetRequestTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}

#else
cp_t P2GetRequestTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}


