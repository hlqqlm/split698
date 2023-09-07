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

20230816-----------------------------------------------------------------------
It adds processor Server-APDU.

20230511-----------------------------------------------------------------------
huanglin 创建.
执行dlt698协议
*/
#include <string.h>
#include <algorithm>

#include "qos/qtest.h"
#include "qos/qstr.h"
#include "qos/qmem.h"

// dlt698_45
#include "protocol/pfill_repository.h"

#include "p2_config.h"

#include "p2_double_long_unsigned.h"
#include "p2_double_long.h"
#include "p2_long_unsigned.h"
#include "p2_octet_string.h"
#include "p2_sequence_of_var_len.h"
#include "p2_array.h"


#include "p2_link_apdu_choice.h"
#include "p2_process_link_apdu.h"

#include "p2_client_apdu_choice.h"
#include "p2_process_client_apdu.h"

#include "p2_server_apdu_choice.h"
#include "p2_process_server_apdu.h"

#include "p2_get_response.h"
#include "p2_get_response_normal.h"
#include "p2_get_response_normal_list.h"
#include "p2_server_apdu_choice.h"
#include "p2_server_apdu.h"

#if 0
#include "p2_security_apdu_choice.h"
#include "p2_security_request.h"
#include "p2_security_response.h"
#include "p2_security_response_data_choice.h"
#endif
#include "p2_security_apdu_choice.h"
#include "p2_process_security_apdu.h"

#include "p2_process.h"
#include "p2_process.xcp.h"
//#define this_file_id	0x  // echo -n p2_process.c | rhash --simple -


#define TEST_EN				(0)
#define PRINT_FILL_EN		(0)			// 是否打印填充帧过程
#define CHECK_MEM_LEAK_EN	(0)			// 是否检查内存泄漏


//{{{ tx_info
typedef struct
{
	char *tx_buf; 
	int tx_buf_size; 
	int tx_size;
} TxInfo;
#define kTxInfo(_tx_buf, _tx_buf_size, _tx_size) {_tx_buf, _tx_buf_size, _tx_size}

// 为了简化tx信息传递，这里用了全局变量
static TxInfo m_tx_info = kTxInfo(NULL, 0, 0);
//}}}


//{{{ class
cp_t P2ProcessOpen(P2Process *m)
{
	ifer(P2LinkOpen(&m->link));
	return 0;
}
cp_t P2ProcessClose(P2Process *m)
{
	dve(P2ProcessValid(m));
	ifer(P2LinkClose(&m->link));
	return 0;
}
cp_t P2ProcessValid(P2Process *m)
{
	ifbr(NULL != m);
	ifer(P2LinkValid(&m->link));
	return 0;
}
//}}}


//{{{ process
static cp_t ProcessApduCp(PfillRepository *fill_repository_life, uint8_t apdu_choice, const char *apdu, int apdu_size)
{
	if (0 == P2SecurityApduChoiceValid(apdu_choice))
	{
		// 按security_apdu解析
		ifer(P2ProcessSecurityApdu(fill_repository_life, apdu, apdu_size));
		// p2_security_apdu_choice.h
	}
	else if (0 == P2ClientApduChoiceValid(apdu_choice))
	{
		// 按client_apdu解析
		ifer(P2ProcessClientApdu(fill_repository_life, apdu, apdu_size));
	}
	else if (0 == P2ServerApduChoiceValid(apdu_choice))
	{
		ifer(P2ProcessServerApdu(fill_repository_life, apdu, apdu_size));
	}
	else if (0 == P2LinkApduChoiceValid(apdu_choice))
	{
		// 按link_apdu解析
		ifer(P2ProcessLinkApdu(fill_repository_life, apdu, apdu_size));
	}
	else
	{
		qos_printf("This apdu_choice can NOT be processed. apdu_choice=%02x\n", apdu_choice);
		return cph;
	}

	return 0;
}
static cp_t ProcessApdu(PfillRepository *fill_repository_life, uint8_t apdu_choice, const char *apdu, int apdu_size)
{
	ifer(ProcessApduCp(fill_repository_life, apdu_choice, apdu, apdu_size));
	const int num = PfillRepositoryNum(fill_repository_life);
	if (0 == num)
	{
		qos_printf("Note: No answer for this rxing frame.\n");
		return 0;
	}

	// 找apdu
	ifbr(0 < num);
	// 认为最外层（最后一个）fill是apdu。apdu可能有多个名字：client_server_apdu，security_response等。
	// 可以默认最后一个是apdu。因为组帧，apdu一定是最后一个
	// 这样找有个好处，不用层层传递apdu指针。减轻传递变量的繁琐，也减少栈的占用，时间换空间
	const int last_ix = num - 1;		// 最后一个索引号
	ifer(PfillRepositoryIxValid(fill_repository_life, last_ix));
	const PfillRepositoryItem *fr_item_last = PfillRepositoryIxItemConst(fill_repository_life, last_ix);
	// 确认fill_apdu在fill_repository_life中，且是最后一个
	ifbr(NULL != fr_item_last);
	// ifbr(0 == strcmp(fr_item_last->name, kP2NameApdu));
	Pfill *fill_apdu = fr_item_last->fill;

	// 构建最外层
	P2Pfill fill_frame = kP2PfillDef(fill_apdu);
	const P2Control control = { kP2DirPrmServerResponse, 0, 0, kP2FunCodeUserData };
	const int addr_size = 6;
	const char kAddr[] = kAddrParameterMrCommAddress;
	const P2AddrValue addr = { kP2AddrSaTypeSingle
		, {
			kAddr[0],
			kAddr[1],  
			kAddr[2],  
			kAddr[3],  
			kAddr[4],  
			kAddr[5]
		}
		, addr_size, 0, 0 };
	ifer(P2PfillOpen(&fill_frame, &control, &addr));
	ifer(P2PfillValid(&fill_frame));

	if (PRINT_FILL_EN)
		PfillSetPrintFillWithSubs(&fill_frame.base, PfillPrintFillNormal);

	dvb(NULL != m_tx_info.tx_buf);
	dvb(0 < m_tx_info.tx_buf_size);
	ifer(PfillDo(&fill_frame.base, 0, m_tx_info.tx_buf, m_tx_info.tx_buf_size));
	const int fill_size = PfillIxLen(&fill_frame.base, kPfillIxAll);
	ifbr(fill_size <= m_tx_info.tx_buf_size);
	m_tx_info.tx_size = fill_size; 

	if (PRINT_FILL_EN)
		printf_hex_ex("tx_buf: ", "\n", m_tx_info.tx_buf, fill_size, "");

	ifer(P2PfillClose(&fill_frame));
	return 0;
}
static cp_t Process(P2Process *m)
{
	dve(P2ProcessValid(m));
	P2Link * const link = &m->link;

	dve(P2LinkFrameValid(link));
	P2LinkFramePrint(link);
	// int P2LinkFrameSize(P2Link *m);
	
	P2Pcut * const part = P2LinkPart(link);
	const char * const frame = P2LinkBuf(link);

	const uint8_t apdu_choice = P2LudChoice(frame);
	const char * const apdu = P2Lud(frame);
	const int apdu_size = P2LudSize(frame);

	// fill仓库，用来管理Pfill指针
	// 用来保存所有fill指针，负责fill的生命周期，即调用fill的close+qfree
	// 为什么所有fill的生命周期，要用一个fill_repository来管理，因为其默认记录了fill的生成顺序，也即释放顺序。
	// 如果分成多个fill_repository，则这个顺序就不好维护了。
	const int malloc_cnt_old = MallocCnt();
	PfillRepository fill_repository_life = kPfillRepositoryDef;	// 用来调用fill的close和qfree
#if CHECK_MEM_LEAK_EN > 0
		qos_printf("Before process_apdu()\n");
		QmemtablePrintAllGlobal();
#endif
	ifer(PfillRepositoryOpen(&fill_repository_life));
	const cp_t cp = ProcessApdu(&fill_repository_life, apdu_choice, apdu, apdu_size);

	if (QmemtableCheckBarrier(QmemtableGlobal()) > 0)
		qos_printf("ERROR: Memory barrier WRONG.\n");

	ifer(PfillRepositoryClose(&fill_repository_life));
	const int malloc_cnt_curr = MallocCnt();
	if (malloc_cnt_old != malloc_cnt_curr)
	{
		qos_printf("WARNING: Memory maybe leaked. malloc_cnt old=%d curr=%d\n", malloc_cnt_old, malloc_cnt_curr);
		qos_printf("After process_apdu()\n");
		QmemtablePrintAllGlobal();
	}

	//const char * const lud = PcutIxPtrConst(&part->base, kP2PartIxLud, P2LinkBuf(link));		// const版本
	//const uint8_t choice = lud[0];
	//if (choice)
//调试这里

	return cp;
}

// 如何处理698中随机数：
// 方法1：回一个明文。（要确定）
// 方法2：回一个带mac的报文，mac随意。同时：主站设置 ESAM接口->安全模式选择->0，该值控制是否检查mac。
// 方法3：增加一个电表esam卡，需要更改硬件。
static void PlcTaskTx(const char *tx_buf, int tx_size)
{
	printf_hex_ex("tx: ", "\n", tx_buf, tx_size, "");
	return;
}
cp_t P2ProcessInput(P2Process *m, const char *rx_buf, int rx_len, char *tx_buf, int tx_buf_size)
{
	dve(P2ProcessValid(m));
	P2Link * const link = &m->link;

	dvb(P2_LEAD_NUM < tx_buf_size);
	/*
	*tx_size = 0;
	
	// 留出前导符的位置
	m_tx_info.tx_buf = tx_buf + P2_LEAD_NUM;
	m_tx_info.tx_buf_size = tx_buf_size - P2_LEAD_NUM;
	m_tx_info.tx_size = 0;
	*/

	// 调试接收过程
	P2LinkConfigPrintRxError(link, P2LinkPrintRxErrorNormal);
	while (rx_len > 0)
	{
		const int room = P2LinkRoom(link);
		const int copy_len = std::min(rx_len, room);
		if (copy_len <= 0)
		{
			// 有异常，可以考虑清空缓冲区
			dvb(false);
			//return 0;
		}

		memcpy(P2LinkRxPos(link), rx_buf, copy_len);
		ifer(P2LinkAppend(link, copy_len));
		// 是否有合法帧在缓冲区？
		if (P2LinkUpdate(link))
		{
			// 填写发送信息
			// 留出前导符的位置
			m_tx_info.tx_buf = tx_buf + P2_LEAD_NUM;
			m_tx_info.tx_buf_size = tx_buf_size - P2_LEAD_NUM;
			m_tx_info.tx_size = 0;

			const cp_t cp = Process(m);
			if (0 != cp)
			{
				char cp_buf[CPSTR_SIZE];
				qos_printf("Hplc process frame FAILED. at %s\n", CpStr(cp_buf, cp));
				printf_hex_ex("the failed rx: ", "\n", rx_buf, rx_len, "");
				// 这里失败，会导致main()返回负值，从而知道解析错误
				return cp;
			}

			// 填充前导符，发送
			ifbr(m_tx_info.tx_size <= m_tx_info.tx_buf_size);
			if (0 == cp)
			{
				// 填前导符0xfe
				P2FrameFillLead(tx_buf);
				const int tx_size = m_tx_info.tx_size + P2_LEAD_NUM;
				dvb(tx_size <= tx_buf_size);
				// 因为有可能一次收到多帧，所以，只能在这里发送。无法返回，然后让
				// 发送
				PlcTaskTx(tx_buf, tx_size);
			}

			ifer(P2LinkDiscardValidFrame(link));
		}
		rx_buf += copy_len;
		rx_len -= copy_len;
		dvb(rx_len >= 0);
	}

	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
static cp_t TestNormal(void)
{
	return 0;
}


 
static const QtestItem kTestItem[] = {
	TestNormal,
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2ProcessTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2ProcessTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

