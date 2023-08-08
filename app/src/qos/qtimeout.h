#ifndef  __QTIMEOUT_H__
#define  __QTIMEOUT_H__

#include "qcp.h"

// 数据项
typedef struct 
{
	int limit;		// 超过这个时间，即认为超时到
	int pass;		// 目前过了多少时间
} QtimeoutDataT;
#define kQtimeoutDataDef {	\
	0, 0,					\
}
extern const QtimeoutDataT kQtimeoutDataZero;
cp_t QtimeoutDataEqual(const QtimeoutDataT *x, const QtimeoutDataT *y);
// 还有多少时间到达限制，即剩余时间
int QtimeoutDataRemain(const QtimeoutDataT *d);
void QtimeoutDatePrint(const QtimeoutDataT *d);

// 接口函数
typedef cp_t (*qtimeout_init_t)(QtimeoutDataT *m, int limit);
typedef cp_t (*qtimeout_valid_t)(const QtimeoutDataT *m);
//typedef cp_t (*qtimeout_remain_t)(const QtimeoutDataT *m);
typedef cp_t (*qtimeout_update_t)(QtimeoutDataT *m, int pass);
typedef void (*qtimeout_clear_t)(QtimeoutDataT *m);

typedef struct {
	// date
	QtimeoutDataT data_;

	// class
	qtimeout_init_t init;
	qtimeout_valid_t valid;
	// info
	// op
	qtimeout_update_t update;
	qtimeout_clear_t clear;
} QtimeoutT;
// 禁止timeout功能
#define kQtimeoutDisable {	\
	kQtimeoutDataDef,		\
	QtimeoutDisableInit, QtimeoutDisableValid,		\
	QtimeoutDisableUpdate, QtimeoutDisableClear,	\
}
// 正常timeout功能
#define kQtimeoutNormal {	\
	kQtimeoutDataDef,		\
	QtimeoutNormalInit, QtimeoutNormalValid,		\
	QtimeoutNormalUpdate, QtimeoutNormalClear,		\
}


// 初始化
cp_t QtimeoutInit(QtimeoutT *m, int limit);
cp_t QtimeoutValid(const QtimeoutT *m);
cp_t QtimeoutEqual(const QtimeoutT *x, const QtimeoutT *y);
void QtimeoutPrint(const QtimeoutT *m);
// info
// 还剩余多少时间，到达超时
int QtimeoutRemain(const QtimeoutT *m);
// 经过pass时间
// return: 0还是时间范围内， 非0超过了时间范围
cp_t QtimeoutUpdate(QtimeoutT *m, int pass);
// 清除时间
void QtimeoutClear(QtimeoutT *m);



// 禁止使能
cp_t QtimeoutDisableInit(QtimeoutDataT *m, int limit);
cp_t QtimeoutDisableValid(const QtimeoutDataT *m);
cp_t QtimeoutDisableUpdate(QtimeoutDataT *m, int pass);
void QtimeoutDisableClear(QtimeoutDataT *m);

// 正常判断超时
cp_t QtimeoutNormalInit(QtimeoutDataT *m, int limit);
cp_t QtimeoutNormalValid(const QtimeoutDataT *m);
cp_t QtimeoutNormalUpdate(QtimeoutDataT *m, int pass);
void QtimeoutNormalClear(QtimeoutDataT *m);

#endif

