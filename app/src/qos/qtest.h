#ifndef  __QTEST_H__
#define  __QTEST_H__

#include "qcp.h"

typedef cp_t (*QtestItem)(void);

cp_t QtestTest(const char *name, int run_times, const QtestItem *item_list, int num, int verbose);


// run_times		运行次数
// verbose			0,1,2，值越大，显示信息越多
#define QTEST_ARG		int run_times, int verbose
// test
cp_t QtestEntry(QTEST_ARG);
#endif

