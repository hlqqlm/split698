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

20230404-----------------------------------------------------------------------
huanglin 创建.
DLT698_45协议的链路层，实现报文的接收、发送
*/
#include <string.h>

#include "qos/qstr.h"
#include "qos/qtest.h"
#include "p2_link.h"
#include "p2_link.xcp.h"
//#define this_file_id	0x  // echo -n p2_link.c | rhash --simple -


#define TEST_EN				(0)


//{{{ class
cp_t P2LinkOpen(P2Link *m)
{
	ifer(QtimeoutInit(&m->timeout, P2_RX_INTERVAL_TIMEOUT_SEC));
	ifer(QbufInit(&m->qbuf, m->buf, P2_LINK_RX_BUF_SIZE, &m->timeout));
	ifer(P2PcutOpen(&m->pcut));
	ifer(P2LinkValid(m));
	return 0;
}
cp_t P2LinkClose(P2Link *m)
{
	dve(P2LinkValid(m));
	ifer(P2PcutClose(&m->pcut));
	return 0;
}
void P2LinkConfigPrintRxError(P2Link *m, P2LinkPrintRxErrorT print_rx_error)
{
	dve(P2LinkValid(m));
	m->print_rx_error = print_rx_error;
}
cp_t P2LinkValid(const P2Link *m)
{
	ifbr(NULL != m->print_rx_error);
	ifer(QtimeoutValid(&m->timeout));
	ifer(QbufValid(&m->qbuf));
	ifer(P2PcutValid(&m->pcut));
	return 0;
}
//}}}


//{{{ rx
int P2LinkRoom(const P2Link *m)
{
	return QbufRoom(&m->qbuf);
}
int P2LinkRxSize(const P2Link *m)
{
	return QbufLen(&m->qbuf);
}
char *P2LinkBuf(P2Link *m) 
{ 
	return m->buf; 
}
char *P2LinkRxPos(P2Link *m)
{
	return QbufPos(&m->qbuf);
}
cp_t P2LinkAppend(P2Link *m, int len)
{
	return QbufAppend(&m->qbuf, len);
}
// return: 0，说明未超时；非0，超时，清空缓冲区。
cp_t P2LinkPassTime(P2Link *m, int pass)
{
	return QbufPassTime(&m->qbuf, pass);
}
static cp_t DiscardLeft(P2Link *m, int discard_n)
{
	dve(P2LinkValid(m));
	dvb(0 <= discard_n);

	m->discard_total += discard_n;
	ifer(QbufDiscardLeft(&m->qbuf, discard_n));
	return 0;
}
//}}}


//{{{ pcut
P2Pcut *P2LinkPart(P2Link *m)
{
	return &m->pcut;
}
static int InvalidIx(P2Link *m)
{
	return PcutInvalidIx(&m->pcut.base, m->buf);
}
static int FrameSize(P2Link *m)
{
	return PcutIxLen(&m->pcut.base, kPcutIxAll, m->buf);
}
//}}}


//{{{ valid_frame
// 缓冲区内有一个完整有效帧
// 调用本函数时，保证rx_len >= frame_len
cp_t P2LinkFrameValid(P2Link *m)
{
	// 应该接收到的内容
	ifbr(0 < P2LinkRxSize(m));
	// 帧没有错误
	ifbr(InvalidIx(m) < 0);
	ifbr(FrameSize(m) <= P2LinkRxSize(m));
	return 0;
}
int P2LinkFrameSize(P2Link *m)
{
	dve(P2LinkFrameValid(m));
	return FrameSize(m);
}
// 抛弃掉前面一个有效帧
cp_t P2LinkDiscardValidFrame(P2Link *m)
{
	dve(P2LinkFrameValid(m));
	const int frame_size = P2LinkFrameSize(m);
	dvb(frame_size <= P2LinkRxSize(m));

	// 帧抛弃后，有效帧就不存在在buf中，需要再次调用Update()来进行判断
	m->valid_frame_exist = false;	
	ifer(DiscardLeft(m, frame_size));
	return 0;
}
void P2LinkFramePrint(P2Link *m)
{
	dve(P2LinkFrameValid(m));

	const char * const frame = P2LinkBuf(m);
	PcutAllPrint(&m->pcut.base, 0, frame);
	const uint8_t control = P2ControlInFrame(frame);

	// 下面补充重要信息
#if 0	// 已经由pcut.Explain代替
	qos_printf("\r\nMore explain:\r\n");
	qos_printf("control: ");
	P2ControlPrint(control);
	qos_printf("\r\n");

	const uint8_t apdu_choice = P2LudChoice(frame);
	qos_printf("apdu_choice: %02x\r\n", apdu_choice);
#endif
	return;
}
//}}}


//{{{ update
// 计算要抛弃的字符数
// 抛弃缓冲区内容，直到找到0x68
// 考虑到第一个字节可能是0x68，第一个字节一定会抛弃
// return: 抛弃字符数
static int CountDiscardNTillHead(const P2Link *m)
{
	const int rx_size = P2LinkRxSize(m);
	// 必须接收至少1个字节
	dvb(0 < rx_size);
	// 有可能第一个字节就是0x68，因此，从下一个字节开始查找
	const int offset = mem_find_ch(P2_HEAD, m->buf + 1, rx_size - 1);
	// 两种情况：
	// 1.	如果offset < 0，说明没有找到，0x68，则抛弃所有内容
	// 2.	如果offset >= 0，抛弃字符数，要加上第一个字节
	//		特殊情况：缓冲区第二个字节就是0x68，offset=0，则抛弃掉第一个字符即可
	const int discard_n = (offset < 0) ? rx_size : (offset + 1);
	return discard_n;
}
// return：0 表示正常； 非0，表示抛弃了内容，值为抛弃的位置
// return：true，表示抛弃了内容，值为抛弃的位置; false，没有抛弃内容
static bool DiscardWhileUpdateOnce(P2Link *m)
{
	dve(P2LinkValid(m));
	const int rx_size = P2LinkRxSize(m);
	int len_expect = 0;	// 接收到某部分，期望的长度

	if (rx_size <= 0)
		return false;

	// 只有接收到了那一部分，才能分析该部分
	// 判断各部分是否合法
	for (int part_ix = 0; part_ix < kP2CutNum; ++part_ix)
	{
		// 期望的长度
		len_expect += PcutIxLen(&m->pcut.base, part_ix, m->buf);
		// 接收的长度大于当前部分的期望长度，才能进行有效性判断
		if (len_expect <= rx_size)
		{
			const cp_t cp = PcutIxValid(&m->pcut.base, part_ix, m->buf);
			// 有错误出现
			if (0 != cp)
			{
				// first只记录一次
				if (-1 == m->first_rx_error_part_ix)
					m->first_rx_error_part_ix = part_ix;
				m->last_rx_error_part_ix = part_ix;		// 最后一个错误的CutIx，表示对应的部分出错
				m->last_rx_error_cp = cp;				// 错误位置
				const int discard_n = m->last_discard_n = CountDiscardNTillHead(m);	// 抛弃掉的字符数
				dvb(0 < discard_n);							// 至少抛弃一个字符
				// 打印错误。打印错误要在真正的抛弃动作之前，否则m->buf内容就变化了
				m->print_rx_error(m);
				++m->discard_cnt;
				// 执行抛弃动作
				ifed(DiscardLeft(m, discard_n));
				// 当Update()函数退出时，如果没有合法帧存在，则一定有空间容纳新的接收内容
				dvb(P2LinkRoom(m) <= P2_LINK_RX_BUF_SIZE);
				return true;
			}
		}
		else
		{
			// 当前部分还有接收完毕，而且各部分没有错误
			return false;
		}
	}

	// 接收完毕，而且报文没有问题
	dvb(0 < P2LinkRxSize(m));
	dve(P2LinkFrameValid(m));
	m->valid_frame_exist = true;
	return false;
}
bool P2LinkUpdate(P2Link *m)
{
	m->valid_frame_exist = false;
	// 只要抛弃了内容，就需要再重新update一次
	// 没有抛弃内容，两种情况：缓冲区内有合法帧；帧没有收完整，且收下的部分内容合法
	while (DiscardWhileUpdateOnce(m))
	{
		;
	}
	return m->valid_frame_exist;
}
//}}}


//{{{ print
void P2LinkPrint(const P2Link *m)
{
	char cp_str[CPSTR_SIZE];
	const int rx_size = P2LinkRxSize(m);
	qos_printf("dlt698_45_link:"
			" last_rx_error_part_ix=%d(%s) last_rx_error_cp=%s "
			" last_discard_n=%d discard_total=%d"
			" rx_size=%d"
			, m->last_rx_error_part_ix, PcutIxStr(&m->pcut.base, m->last_rx_error_part_ix), CpStr(cp_str, m->last_rx_error_cp)
			, m->last_discard_n, m->discard_total
			, rx_size
			);
	printf_section_hex(" rx_buf=[", m->buf, rx_size, " ");
	qos_printf("]");
	qos_printf(" timeout:[");
	QtimeoutPrint(&m->timeout);
	qos_printf("]");
	qos_printf(" qbuf:[");
	QbufPrint(&m->qbuf);
	qos_printf("]");
	return;
}
// 发生错误，打印错误信息
cp_t P2LinkPrintRxErrorNormal(const P2Link *m)
{
	qos_printf("dlt698_45 rx error. P2Link:");
	P2LinkPrint(m);
	qos_printf("\n");

	// cs错误，这个发生比较多，打印信息，方便调试
	if (kP2CutIxHcs == m->last_rx_error_part_ix)
	{
		const uint16_t hcs_in_frame = P2HcsInFrame(m->buf);
		const uint16_t hcs_calc = P2HcsCalc(m->buf);
		dvb(hcs_calc != hcs_in_frame);
		qos_printf("HCS don't match. hcs_calc=%04x hcs_in_frame=%04x\n", hcs_calc, hcs_in_frame);
		return 0;
	}

	if (kP2CutIxFcs == m->last_rx_error_part_ix)
	{
		const uint16_t fcs_in_frame = P2FcsInFrame(m->buf);
		const uint16_t fcs_calc = P2FcsCalc(m->buf);
		dvb(fcs_calc != fcs_in_frame);
		qos_printf("FCS don't match. fcs_calc=%04x fcs_in_frame=%04x\n", fcs_calc, fcs_in_frame);
		return 0;
	}
	return 0;
}
cp_t P2LinkPrintRxErrorDisable(const P2Link *m)
{
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestOneValidFrame(void)
{
	// qos_printf("dlt698_45_link will test one valid frame.\r\n");

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char frame[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	const int frame_size = sizeof(frame) - 1;
	
	P2Link m = kP2LinkDef;
	ifer(P2LinkOpen(&m));
	ifer(P2LinkValid(&m));

	ifbr(P2_LINK_RX_BUF_SIZE == P2LinkRoom(&m));
	ifbr(0 == P2LinkRxSize(&m));
	ifbr(P2LinkBuf(&m) == P2LinkRxPos(&m));
	ifbr(0 != P2LinkFrameValid(&m));

	// 传入5个字符
	memcpy(P2LinkRxPos(&m), frame, 5);
	ifer(P2LinkAppend(&m, 5));
	ifbr((P2_LINK_RX_BUF_SIZE - 5) == P2LinkRoom(&m));
	ifbr(5 == P2LinkRxSize(&m));
	ifbr((P2LinkBuf(&m) + 5) == P2LinkRxPos(&m));
	ifbr(!P2LinkUpdate(&m));
	ifbr(0 != P2LinkFrameValid(&m));

	// 传入剩余字符
	memcpy(P2LinkRxPos(&m), frame + 5, frame_size - 5);
	ifer(P2LinkAppend(&m, frame_size - 5));
	ifbr((P2_LINK_RX_BUF_SIZE - frame_size) == P2LinkRoom(&m));
	ifbr(frame_size == P2LinkRxSize(&m));
	ifbr((P2LinkBuf(&m) + frame_size) == P2LinkRxPos(&m));
	ifbr(P2LinkUpdate(&m));
	ifbr(0 == P2LinkFrameValid(&m));
	ifbr(frame_size == P2LinkFrameSize(&m));

	ifer(P2LinkDiscardValidFrame(&m));
	ifbr(0 == P2LinkRxSize(&m));
	ifbr(0 != P2LinkFrameValid(&m));

	ifer(P2LinkClose(&m));
	// qos_printf("dlt698_45_link one valid frame ok.\r\n");
	return 0;
}
static cp_t TestTwoValidFrame(void)
{
	// qos_printf("dlt698_45_link will test two valid frame.\r\n");

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char two_frame[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16" "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	const int two_frame_size = sizeof(two_frame) - 1;
	const int one_frame_size = two_frame_size / 2;
	
	P2Link m = kP2LinkDef;
	ifer(P2LinkOpen(&m));
	ifer(P2LinkValid(&m));

	// 输入一帧报文多5字节
	memcpy(P2LinkRxPos(&m), two_frame, one_frame_size + 5);
	ifer(P2LinkAppend(&m, one_frame_size + 5));
	// 找到一帧
	ifbr(P2LinkUpdate(&m));
	ifbr(0 == P2LinkFrameValid(&m));
	ifbr(one_frame_size == P2LinkFrameSize(&m));

	ifer(P2LinkDiscardValidFrame(&m));
	ifbr(5 == P2LinkRxSize(&m));
	ifbr(0 != P2LinkFrameValid(&m));

	memcpy(P2LinkRxPos(&m), two_frame + one_frame_size + 5, one_frame_size - 5);
	ifer(P2LinkAppend(&m, one_frame_size - 5));
	// 找到第2帧
	ifbr(P2LinkUpdate(&m));
	ifbr(0 == P2LinkFrameValid(&m));
	ifbr(one_frame_size == P2LinkFrameSize(&m));

	ifer(P2LinkDiscardValidFrame(&m));
	ifbr(0 == P2LinkRxSize(&m));
	ifbr(0 != P2LinkFrameValid(&m));

	ifer(P2LinkClose(&m));
	// qos_printf("dlt698_45_link two valid frame ok.\r\n");
	return 0;
}
static cp_t TestHcsInvalidFrame(void)
{
	// qos_printf("dlt698_45_link will test hcs invalid frame.\r\n");

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char frame[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x8a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	const int frame_size = sizeof(frame) - 1;
	
	P2Link m = kP2LinkDef;
	ifer(P2LinkOpen(&m));

	memcpy(P2LinkRxPos(&m), frame, frame_size);
	ifer(P2LinkAppend(&m, frame_size));
	// P2LinkConfigPrintRxError(&m, P2LinkPrintRxErrorNormal);
	ifbr(!P2LinkUpdate(&m));

	ifbr(!m.valid_frame_exist);
	// 因为测试报文中无0x68，因此，first / last 是一样的.
	ifbr(kP2CutIxHcs == m.first_rx_error_part_ix);
	ifbr(kP2CutIxHcs == m.last_rx_error_part_ix);
	ifbr(frame_size == m.discard_total);
	ifbr(0 == P2LinkRxSize(&m));

	ifer(P2LinkClose(&m));
	// qos_printf("dlt698_45_link hcs invalid frame ok.\r\n");
	return 0;
}
static cp_t TestTimeout(void)
{
	// qos_printf("dlt698_45_link will test timeout.\r\n");

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char frame[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	const int frame_size = sizeof(frame) - 1;
	
	P2Link m = kP2LinkDef;
	ifer(P2LinkOpen(&m));

	// 情况1：立即超时
	memcpy(P2LinkRxPos(&m), frame, 5);
	ifer(P2LinkAppend(&m, 5));
	// P2LinkConfigPrintRxError(&m, P2LinkPrintRxErrorNormal);
	ifbr(!P2LinkUpdate(&m));
	ifbr(!m.valid_frame_exist);
	ifbr(0 != P2LinkPassTime(&m, P2_RX_INTERVAL_TIMEOUT_SEC));
	ifbr(0 == P2LinkRxSize(&m));

	// 情况2：分两次超时
	memcpy(P2LinkRxPos(&m), frame, 5);
	ifer(P2LinkAppend(&m, 5));
	// P2LinkConfigPrintRxError(&m, P2LinkPrintRxErrorNormal);
	ifbr(!P2LinkUpdate(&m));
	ifbr(0 == P2LinkPassTime(&m, P2_RX_INTERVAL_TIMEOUT_SEC - 1));
	ifbr(5 == P2LinkRxSize(&m));

	// 又经过1秒，且没有输入任何内容，超时
	ifbr(0 != P2LinkPassTime(&m, 1));
	ifbr(0 == P2LinkRxSize(&m));

	// 情况3：快超时时，又输入了内容，超时时间清零
	memcpy(P2LinkRxPos(&m), frame, 5);
	ifer(P2LinkAppend(&m, 5));
	ifbr(!P2LinkUpdate(&m));
	ifbr(0 == P2LinkPassTime(&m, P2_RX_INTERVAL_TIMEOUT_SEC - 1));
	ifbr(5 == P2LinkRxSize(&m));
	memcpy(P2LinkRxPos(&m), frame + 5, frame_size - 5);
	ifer(P2LinkAppend(&m, frame_size - 5));
	ifbr(0 == P2LinkPassTime(&m, P2_RX_INTERVAL_TIMEOUT_SEC - 1));
	ifbr(frame_size == P2LinkRxSize(&m));
	//P2LinkConfigPrintRxError(&m, P2LinkPrintRxErrorNormal);
	ifbr(P2LinkUpdate(&m));

	ifer(P2LinkClose(&m));
	// qos_printf("dlt698_45_link timeout ok.\r\n");
	return 0;
}


 
static const QtestItem kTestItem[] = {
	TestOneValidFrame,
	TestTwoValidFrame,
	TestHcsInvalidFrame,
	TestTimeout
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2LinkTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2LinkTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

