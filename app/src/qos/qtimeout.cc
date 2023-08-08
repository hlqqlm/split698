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

20220303-----------------------------------------------------------------------
huanglin 创建.
用来检查输入报文，是否超时。如果超时，应该将报文全部删除

本模块为一个通用的超时判断
没有给时间定单位，只要输入的时间都是一个单位即可
*/
#include <limits.h>
#include "qtimeout.h"
#include "qtimeout.xcp.h"
//#define this_file_id 0x4d5c7e7d

//{{{ data
const QtimeoutDataT kQtimeoutDataZero = kQtimeoutDataDef;
cp_t QtimeoutDataEqual(const QtimeoutDataT *x, const QtimeoutDataT *y)
{
	ifbr(x->limit == y->limit);
	ifbr(x->pass == y->pass);
	return 0;
}
int QtimeoutDataRemain(const QtimeoutDataT *d)
{
	return d->limit - d->pass;
}
void QtimeoutDatePrint(const QtimeoutDataT *d)
{
	qos_printf("limit=%d pass=%d", d->limit, d->pass);
}
//}}}



//{{{ class
cp_t QtimeoutInit(QtimeoutT *m, int limit)
{
	ifer(m->init(&m->data_, limit));
	return 0;
}
cp_t QtimeoutValid(const QtimeoutT *m)
{
	ifer(m->valid(&m->data_));
	return 0;
}
cp_t QtimeoutEqual(const QtimeoutT *x, const QtimeoutT *y)
{
	return QtimeoutDataEqual(&x->data_, &y->data_);
}
void QtimeoutPrint(const QtimeoutT *m)
{
	QtimeoutDatePrint(&m->data_);
}
int QtimeoutRemain(const QtimeoutT *m)
{
	return QtimeoutDataRemain(&m->data_);
	// return m->data.limit - m->data.pass;
}
cp_t QtimeoutUpdate(QtimeoutT *m, int pass)
{
	return m->update(&m->data_, pass);
}
void QtimeoutClear(QtimeoutT *m)
{
	m->clear(&m->data_);
}
//}}}



//{{{ disable禁止使能
cp_t QtimeoutDisableInit(QtimeoutDataT *m, int limit)
{
	m->limit = INT_MAX;	// 无论传入的值是什么，都将limit设置成，表示是disable
	m->pass = 0;
	return 0;
}
cp_t QtimeoutDisableValid(const QtimeoutDataT *m)
{
	return 0;
}
cp_t QtimeoutDisableUpdate(QtimeoutDataT *m, int pass)
{
	return 0;
}
void QtimeoutDisableClear(QtimeoutDataT *m)
{
	return;
}
//}}}



//{{{ normal正常判断超时
cp_t QtimeoutNormalInit(QtimeoutDataT *m, int limit)
{
	m->limit = limit;
	m->pass = 0;
	ifer(QtimeoutNormalValid(m));
	return 0;
}
cp_t QtimeoutNormalValid(const QtimeoutDataT *m)
{
	ifbr(0 <= m->limit);
	ifbr(0 <= m->pass);
	return 0;
}
cp_t QtimeoutNormalUpdate(QtimeoutDataT *m, int pass)
{
	m->pass += pass;
	// 还在限时内
	if (m->pass < m->limit)
		return 0;

	// 超过限时
	// 此句保证了pass < limit这个条件
	m->pass = 0;
	// 通知父模块，超过了限时
	return cph;
}
void QtimeoutNormalClear(QtimeoutDataT *m)
{
	m->pass = 0;
	return;
}
//}}}

