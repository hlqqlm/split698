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

20230526-----------------------------------------------------------------------
huanglin 创建.
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

用来描述dlt698_45中的GetResult
*/
#include "qdlt698.h"
#include "p2_get_result.h"
#include "p2_get_result.xcp.h"
//#define this_file_id	0xc766fbff // echo -n dlt698_45_get_result.c | rhash --simple -


#define TEST_EN				(0)


cp_t P2GetResultFillItemProcessChoice(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2GetResultFillItemChoice *derive = (P2GetResultFillItemChoice*)fi;

	mem[offset] = derive->choice;
	*fill_size = 1;
	return 0;
}


cp_t P2GetResultFillItemProcessDar(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2GetResultFillItemDar *derive = (P2GetResultFillItemDar*)fi;

	mem[offset] = derive->dar;
	*fill_size = 1;
	return 0;
}


cp_t P2GetResultFillInit(Pfill *m, uint8_t dar, Pfill *sub_data)
{
	const uint8_t choice = ((NULL == sub_data) ? 0 : 1);

	const P2GetResultFillItemChoice kFiChoice = kP2GetResultFillItemChoiceDef(PfillItemOffsetFix, kP2GetResultChoiceOffset, choice);
	ifbr(NULL != PfillItemFactory(m, sizeof(kFiChoice), &kFiChoice));

	if (0 == choice)
	{
		// dar
		const P2GetResultFillItemDar kFiDar = kP2GetResultFillItemDarDef(dar);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiDar), &kFiDar));
	}
	else
	{
		// data
		const PfillItem kFiData = PFILL_ITEM(kP2GetResultNameData, PfillItemOffsetFix, PfillItemProcessBySub, kP2GetResultVarOffset, sub_data);
		ifbr(NULL != PfillItemFactory(m, sizeof(kFiData), &kFiData));
	}

	return 0;
}

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
cp_t P2GetResultTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2GetResultTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

