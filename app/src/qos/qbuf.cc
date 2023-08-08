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

20220302-----------------------------------------------------------------------
huanglin 创建.
通用报文缓存
1	有效报文始终在buf的左侧。
*/
#include <string.h>

#include "qbuf.h"
#include "qbuf.xcp.h"
//#define this_file_id 0x5D498518


//{{{ class
const QbufT kQbufZero = kQbufDef;
cp_t QbufInit(QbufT *m, char *buf, int capacity, QtimeoutT *timeout)
{
	dvb(NULL != buf);

	//*m = kQbufZero;
	m->buf = buf;
	m->capacity = capacity;

	m->timeout = timeout;
	return QbufValid(m);
}
cp_t QbufValid(const QbufT *m)
{
	ifbr(NULL != m);
	ifbr(NULL != m->buf);
	ifbr(0 <= m->capacity);
	ifbr(0 <= m->len);
	ifbr(m->len <= m->capacity);

	ifbr(NULL != m->timeout);
	ifer(QtimeoutValid(m->timeout));
	return 0;
}
cp_t QbufEqual(const QbufT *x, const QbufT *y)
{
	ifbr(x->capacity == y->capacity);
	ifbr(x->len == y->len);
	ifbr(x->buf == y->buf);

	ifer(QtimeoutEqual(x->timeout, y->timeout));
	return 0;
}
void QbufPrint(const QbufT *m)
{
	qos_printf("capacity=%d len=%d", m->capacity, m->len);
}
//}}}


//{{{ info
// 空、满
bool QbufEmpty(const QbufT *m)
{
	return 0 == m->len;
}
bool QbufFull(const QbufT *m)
{
	return m->capacity <= m->len;
}
// 剩余空间
int QbufRoom(const QbufT *m)
{
	return (m->capacity - m->len);
}
int QbufLen(const QbufT *m)
{
	return m->len;
}
char *QbufPos(QbufT *m)
{
	return m->buf + m->len;
}
cp_t QbufCmp(const QbufT *m, const char *buf, int len)
{
	ifbr(len == m->len);
	ifbr(0 == memcmp(m->buf, buf, len));
	return 0;
}
//}}}


//{{{ write
// 如果发生错误，则什么都不改变。
cp_t QbufAppend(QbufT *m, int len)
{
	dve(QbufValid(m));

	const int new_len = m->len + len;
	ifbr(new_len <= m->capacity);
	m->len = new_len;

	// 更新了内容，计时器清零
	QtimeoutClear(m->timeout);
	return 0;
}
cp_t QbufPassTime(QbufT *m, int pass)
{
	dve(QbufValid(m));

	const cp_t timeout = QtimeoutUpdate(m->timeout, pass);
	// 未超时
	if (0 == timeout)
		return 0;

	// 发生超时，将缓冲区里的内容清零
	QbufClear(m);
	return cph;
}
/*
int QbufTimeoutRemain(const QbufT *m)
{
	return QtimeoutRemain(m->timeout);
}
*/
void QbufClear(QbufT *m)
{
	dve(QbufValid(m));
	m->len = 0;
	// 当判断出超时，QtimeoutUpdate()已经将timeout中计时器清零了。
	QtimeoutClear(m->timeout);
	return;
}

cp_t QbufDiscardLeft(QbufT *m, int discard_n)
{
	dve(QbufValid(m));

	// 敏感操作，这里严格判断输入，如果不满足要求，则什么都不做
	ifbr(discard_n >= 0);
	ifbr(discard_n <= m->len);

	const int new_len = m->len - discard_n;
	memmove(m->buf, m->buf + discard_n, new_len);
	m->len = new_len;

	// 更新了内容，计时器清零
	QtimeoutClear(m->timeout);
	return 0;
}
//}}}

