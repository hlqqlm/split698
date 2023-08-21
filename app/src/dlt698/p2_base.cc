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
DLT698_45报文解析

因为DLT645_07前缀太长，因此，将程序内的协议编号
编号	协议名称		缩写
1	DLT645_07		P1	(protocol 1#)
2	DLT698_45		P2	(protocol 2#)

一段报文成为cut
*/
#include <math.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "fcs.h"

// for apdu_choice_name
#include "p2_link_apdu_choice.h"
#include "p2_client_apdu_choice.h"
#include "p2_server_apdu_choice.h"
#include "p2_security_apdu_choice.h"

#include "p2_base.h"
#include "p2_base.xcp.h"
//#define this_file_id	0x634089b5 // echo -n dlt698_45_base.c | rhash --simple -

#define TEST_EN					(0)
#define PRINT_PART_IN_TEST_EN	(0)		// 在测试中打印cut解帧结果
#define PRINT_FILL_IN_TEST_EN	(1)		// 在测试中打印fill组帧结果


//{{{ frame_size
int P2FrameSize(const char *frame)
{
	return P2_FRAME_SIZE(P2LValue(frame));
}
cp_t P2FrameSizeValid(int frame_size)
{
	ifbr(P2_FRAME_SIZE_MIN <= frame_size);
	ifbr(frame_size <= P2_FRAME_SIZE_MAX);
	return 0;
}
//}}}


//{{{ lead
int P2FrameFillLead(char *mem)
{
	memset(mem, P2_LEAD_CH, P2_LEAD_NUM);
	return P2_LEAD_NUM;
}
cp_t P2FillItemProcessLead(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	dvb(0 == offset);
	memset(mem + offset, P2_LEAD_CH, P2_LEAD_NUM);
	*fill_size = P2_LEAD_NUM;
	return 0;
}
//}}}


//{{{ head
static int LenHead(Pcut *cut, int ix, const char *frame) { return P2_HEAD_SIZE; }
static int OffsetHead(Pcut *cut, int ix, const char *frame) { return P2_HEAD_OFFSET; }
static cp_t ValidHead(Pcut *cut, int ix, const char *frame) 
{ 
	const char ch = frame[P2_HEAD_OFFSET];
	ifbr(P2_HEAD == ch);
	return 0; 
}
cp_t P2FillItemProcessHead(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	dvb(P2_HEAD_OFFSET == offset);
	mem[offset] = P2_HEAD;
	*fill_size = P2_HEAD_SIZE;
	return 0;
}
//}}}


//{{{ L
int P2LValueCut(const char l_cut[P2_L_SIZE])
{
	const uint8_t low = (uint8_t)l_cut[0];
	const uint8_t high = (uint8_t)l_cut[1];
	// dLen = *(p+1) + (*(p+2)&0x3f)*256;
	const uint8_t len_unit_k = high & 0x40;
	const uint32_t value = low + ((uint32_t)(high & 0x3f)) * 256;
	if (len_unit_k)
		return value * 1000;
	return value;
}
cp_t P2LValuePack(char l_cut[P2_L_SIZE], int l_value)
{
	ifbr(l_value >= P2_L_VALUE_MIN);

	// 大于这个长度，必须用k为单位
	if (l_value > 0x3fff)
	{
		const uint16_t l_value_k = (uint16_t)(l_value / 1000);
		const uint8_t high = ((uint8_t)(l_value_k >> 8)) & 0x3f + 0x40;	// 0x40是k单位
		const uint8_t low = (uint8_t)(l_value_k & 0x00ff);
		l_cut[0] = low;
		l_cut[1] = high;
		return 0;
	}

	const uint8_t high = ((uint8_t)(l_value >> 8)) & 0x3f;	
	const uint8_t low = (uint8_t)(l_value & 0x00ff);
	l_cut[0] = low;
	l_cut[1] = high;
	return 0;
}
int P2LValue(const char *frame)
{
	const char *l_cut = frame + P2_L_OFFSET;
	return P2LValueCut(l_cut);
}
static int LenL(Pcut *cut, int ix, const char *frame) { return P2_L_SIZE; }
static int OffsetL(Pcut *cut, int ix, const char *frame) { return P2_L_OFFSET; }
static cp_t ValidL(Pcut *cut, int ix, const char *frame) 
{
	const int l_value = P2LValue(frame);
	ifbr(P2_L_VALUE_MIN <= l_value);
	ifbr(l_value <= P2_L_VALUE_MAX);
	// const int frame_size = P2_L_VALUE_2_FRAME_SIZE(l_value);
	// ifer(P2FrameSizeValid(frame_size));
	return 0;
}
static cp_t ExplainL(Pcut *cut, int ix, const char *frame) 
{
	const int l_value = P2LValue(frame);
	qos_printf("%dD", l_value);
	return 0;
}
cp_t P2FillItemProcessL(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	dvb(P2_L_OFFSET == offset);

	const int addr_size = PfillIxLen(fill, kP2FillIxA);
	dvb(0 < addr_size);
	const int lud_size = PfillIxLen(fill, kP2FillIxLud);

	const int l_value = P2_LUD_SIZE_2_L_VALUE(lud_size, addr_size);

	ifer(P2LValuePack(mem + offset, l_value));
	*fill_size = P2_L_SIZE;
	return 0;
}
//}}}


//{{{ C
uint8_t P2ControlInFrame(const char *frame)
{
	return frame[P2_C_OFFSET];
}
void P2ControlSplit(P2Control *dst, uint8_t control)
{
	// 传输方向位及启动标志位定义：
	//dst->dir_from_server = control & kP2ControlDirFromServerMask;
	//dst->prm_form_client = control & kP2ControlPrmFromClientMask;
	dst->dir_prm = (control & kP2ControlDirPrmMask) >> kP2ControlDirPrmShift;
	dst->split = (control & kP2ControlSplitMask) >> kP2ControlSplitShift;		// 分帧标志位
	dst->scamble = (control & kP2ControlScambleMask) >> kP2ControlScambleShift;	// 扰码标志位
	dst->func_code = control & kP2ControlFuncCodeMask;							// 功能码
}
uint8_t P2ControlPack(const P2Control *control)
{
	const uint8_t dir_prm = control->dir_prm << kP2ControlDirPrmShift;
	const uint8_t split = control->split ? 0x20 : 0x00;
	const uint8_t scamble = control->scamble ? 0x08 : 0x00;
	const uint8_t func_code = control->func_code & kP2ControlFuncCodeMask;
	const uint8_t c_value = dir_prm + split + scamble + func_code;
	return c_value;
}
uint8_t P2ControlDirPrm(uint8_t control)
{
	return (control & kP2ControlDirPrmMask) >> kP2ControlDirPrmShift;
}
void P2ControlPrint(uint8_t control)
{
	P2Control p2c;
	P2ControlSplit(&p2c, control);
	qos_printf("dirprm=%u split=%u scamble=%u func=%u", p2c.dir_prm, p2c.split, p2c.scamble, p2c.func_code);
}
static int LenC(Pcut *cut, int ix, const char *frame) { return P2_C_SIZE; }
static int OffsetC(Pcut *cut, int ix, const char *frame) { return P2_C_OFFSET; }
static cp_t ValidC(Pcut *cut, int ix, const char *frame) 
{ 
	// 可以拆解C后检查，这里不做检查
	/*
	const char C = frame[P1_CONTROL_OFFSET];
	P1ControlT split = P1_CONTROL_DEF; 
	P1ControlSplit(&split, C);
	ifer(P1FunCodeValid(split.fun_code));
	*/
	return 0; 
}
static cp_t ExplainC(Pcut *cut, int ix, const char *frame) 
{
	const uint8_t control = P2ControlInFrame(frame);
	qos_printf("\t");
	P2ControlPrint(control);
	return 0;
}
cp_t P2FillItemProcessC(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2FillItemC *derive = (P2FillItemC*)fi;

	mem[offset] = P2ControlPack(&derive->control);
	*fill_size = P2_C_SIZE;
	return 0;
}
//}}}

#if 0
//{{{ A
/*
6.2.4 地址域 A
6.2.4.1 地址域组成
地址域A由可变字节数的服务器地址SA和1字节的客户机地址CA组成，定义见图9 。

服务器地址 SA
客户机地址 CA
图9 地址域定义
6.2.4.2 服务器地址 SA
6.2.4.2.1 服务器地址定义
服务器地址SA由1字节地址特征和N个字节地址组成。定义见图10 。
地址特征定义：
a) bit0…bit3：为地址的字节数，取值范围：0…15，对应表示 1…16 个字节长度；
b) bit4…bit5：逻辑地址；
bit5=0 表示无扩展逻辑地址，bit4 取值 0 和 1 分别表示逻辑地址 0 和 1；
bit5=1 表示有扩展逻辑地址，bit4 备用；地址长度 N 包含 1 个字节的扩展逻辑地址，取值范
围 2…255，表示逻辑地址 2…255；
c) bit6…bit7：为服务器地址的地址类型，0 表示单地址，1 表示通配地址，2 表示组地址，3
表示广播地址。
扩展逻辑地址和地址要求如下：
a) 扩展逻辑地址取值范围 2…255；
b) 编码方式为压缩 BCD 码，0 保留；
c) 当服务器地址的十进制位数为奇数时，最后字节的 bit3…bit0 用 FH 表示。
*/
const char *P2AddrSaCut(const char *addr_cut)
{
	return addr_cut;
}
uint8_t P2AddrSaCutFeature(const char *addr_sa_cut)
{
	const uint8_t sa_feature = (uint8_t)addr_sa_cut[0];
	return sa_feature;
}
int P2AddrSaCutSize(const char *addr_sa_cut)
{
	const uint8_t sa_feature = P2AddrSaCutFeature(addr_sa_cut);
	const uint8_t len_value = sa_feature & kP2SaLenMask;
	// qos_printf("len_value = %d.\r\n", (int)len_value);
	const int size = (int)len_value + 1 + 1;	// len_value 0对应1字节，加1字节特征字节
												// len_value取值范围：0…15，对应表示 1…16 个字节长度；
	return size;
}
/*
b) bit4…bit5：逻辑地址；
bit5=0 表示无扩展逻辑地址，bit4 取值 0 和 1 分别表示逻辑地址 0 和 1；
bit5=1 表示有扩展逻辑地址，bit4 备用；地址长度 N 包含 1 个字节的扩展逻辑地址，取值范
围 2…255，表示逻辑地址 2…255；
*/
bool P2AddrSaLogicExist(const char *addr_sa_cut)
{
	const uint8_t sa_feature = P2AddrSaCutFeature(addr_sa_cut);
	const uint8_t logic_exist = sa_feature & kP2SaLogicExistMask;
	return !!(logic_exist);
}
uint8_t P2AddrSaLogicAddr(const char *addr_sa_cut)
{
	const bool logic_exist = P2AddrSaLogicExist(addr_sa_cut);
	if (logic_exist)
		return addr_sa_cut[1];

	const uint8_t sa_feature = P2AddrSaCutFeature(addr_sa_cut);
	const uint8_t logic_addr = (sa_feature & kP2SaLogicAddrMask) >> 4;
	return logic_addr;
}
int P2AddrSaNakedOffset(const char *addr_sa_cut)
{
	const uint8_t logic_exist = P2AddrSaLogicExist(addr_sa_cut);
	// 当逻辑地址存在，多一个字节扩展逻辑地址
	const int offset = 1 + (logic_exist ? 1 : 0);
	return offset;
}
int P2AddrSaNakedSize(const char *addr_sa_cut)
{
	const int sa_size = P2AddrSaCutSize(addr_sa_cut);
	dvb(sa_size >= 1);
	const bool logic_exist = P2AddrSaLogicExist(addr_sa_cut);
	if (logic_exist)
	{
		dvb(sa_size >= 2);
		return sa_size - 2;		// 1字节地址特征，1字节扩展逻辑地址/分路地址
	}
	return sa_size - 1;			// 1字节地址特征
}
const char *P2AddrSaNaked(const char *addr_sa_cut)
{
	return addr_sa_cut + P2AddrSaNakedOffset(addr_sa_cut);
}
const char *P2AddrCaCut(const char *addr_cut)
{
	const char *sa = P2AddrSaCut(addr_cut);
	const int sa_size = P2AddrSaCutSize(sa);
	return sa + sa_size;
}
uint8_t P2AddrCa(const char *addr_ca_cut)
{
	return (uint8_t)(*addr_ca_cut);
}
int P2AddrSizeCut(const char *addr_cut)
{
	const char *sa = P2AddrSaCut(addr_cut);
	const int sa_size = P2AddrSaCutSize(sa);
	// qos_printf("sa_size = %d.\r\n", sa_size);
	const int addr_size = sa_size + P2_ADDR_CA_SIZE;
	return addr_size;
}
int P2AddrSize(const char *frame)
{
	const char * const addr_cut = frame + P2_ADDR_OFFSET;
	return P2AddrCutSize(addr_cut);
}
//}}}
#endif
//{{{ A
static int AddrSize(const char *frame)
{
	const char * const addr_cut = frame + P2_ADDR_OFFSET;
	return P2AddrCutSize(addr_cut);
}
static int LenAddr(Pcut *cut, int ix, const char *frame) { return P2_ADDR_SIZE(AddrSize(frame)); }
static int OffsetAddr(Pcut *cut, int ix, const char *frame) { return P2_ADDR_OFFSET; }
// 通过sub来检查addr的有效性
#define ValidAddr		PcutItemValidBySub
/*
static cp_t ValidAddr(Pcut *cut, int ix, const char *frame) 
{ 
	return 0; 
}
*/
cp_t P2FillItemProcessAddr(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	dvb(P2_ADDR_OFFSET == offset);

	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const P2FillItemAddr *derive = (P2FillItemAddr*)fi;
	char *dst = mem + offset;
	ifer(P2AddrFill(dst, &derive->addr));
	*fill_size = P2AddrCutSize(dst);
	return 0;
}
//}}}


//{{{ HCS
uint16_t P2HcsInFrame(const char *frame)
{
	const int addr_size = AddrSize(frame);
	const int offset = P2_HCS_OFFSET(addr_size);
	
	const uint8_t low = (uint8_t)frame[offset];
	const uint8_t high = (uint8_t)frame[offset + 1];
	//const uint16_t v = (uint16_t)high * 0x100 + low;
	// 注：const uint16_t v = (uint16_t)high << 8 + low; 写法错误
	// 当addr_size=8 offset=12 low=7aH high=4eH v=0000，+优先级高于<<
	const uint16_t v = ((uint16_t)high << 8) + low;
	// qos_printf("addr_size=%d offset=%d low=%02xH high=%02xH v=%04x\r\n", addr_size, offset, low, high, v);
	return v;
}
uint16_t P2HcsCalc(const char *frame)
{
	const int offset = P2_HCS_CALC_OFFSET;
	const int addr_size = AddrSize(frame);
	const int len = P2_HCS_CALC_LEN(addr_size);
	return GetFcs(frame + offset, len);
}
void P2HcsPack(char *frame)
{
	const uint16_t hcs = P2HcsCalc(frame);
	const uint8_t low = hcs & 0x00ff;
	const uint8_t high = (uint8_t)(hcs >> 8);

	const int addr_size = AddrSize(frame);
	const int offset = P2_HCS_OFFSET(addr_size);
	frame[offset] = low;
	frame[offset + 1] = high;
	return;
}
static cp_t HcsValid(const char *frame)
{
	const uint16_t in_frame = P2HcsInFrame(frame);
	const uint16_t calc = P2HcsCalc(frame);
	// qos_printf("in_frame=%04x calc=%04x\r\n", in_frame, calc);
	ifbr(in_frame == calc);
	return 0;
}
static int LenHcs(Pcut *cut, int ix, const char *frame) { return P2_HCS_SIZE; }
static int OffsetHcs(Pcut *cut, int ix, const char *frame) { return P2_HCS_OFFSET(AddrSize(frame)); }
static cp_t ValidHcs(Pcut *cut, int ix, const char *frame) 
{ 
	return HcsValid(frame);
}
int P2FillItemOffsetHcs(struct PfillS *fill, int ix, const char *mem, int mem_size)
{
	dve(PfillValid(fill));
	dve(PfillIndexValid(fill, ix));

	const int addr_size = PfillIxLen(fill, kP2FillIxA);
	dvb(0 < addr_size);
	const int offset = P2_HCS_OFFSET(addr_size);
	return offset;
}
cp_t P2FillItemProcessHcs(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const uint16_t hcs = P2HcsCalc(mem);
	mem[offset] = hcs & 0x00ff;
	mem[offset + 1] = (uint8_t)(hcs >> 8);
	*fill_size = P2_HCS_SIZE;
	return 0;
}
//}}}


//{{{ lud / apdu
int P2LudSize(const char *frame)
{
	return P2_LUD_SIZE(P2LValue(frame), AddrSize(frame)); 
}
int P2LudOffset(const char *frame)
{
	return P2_LUD_OFFSET(AddrSize(frame));
}
const char *P2Lud(const char *frame)
{
	return frame + P2LudOffset(frame);
}
static int LenLud(Pcut *cut, int ix, const char *frame) { return P2LudSize(frame); }
static int OffsetLud(Pcut *cut, int ix, const char *frame) { return P2LudOffset(frame); }
static cp_t ValidLud(Pcut *cut, int ix, const char *frame) 
{
	return 0;
}
static cp_t ExplainLud(Pcut *cut, int ix, const char *frame) 
{
	const uint8_t apdu_choice = P2LudChoice(frame);
	qos_printf("apdu_choice=%uD(%s)", (unsigned int)apdu_choice, P2LudChoiceStr(apdu_choice));
	return 0;
}
uint8_t P2LudChoice(const char *frame)
{
	const int offset = P2LudOffset(frame);
	return frame[offset];
}
const char *P2LudChoiceStr(uint8_t choice)
{
	if (0 == P2LinkApduChoiceValid(choice))
		return P2LinkApduChoiceStr(choice);

	if (0 == P2ClientApduChoiceValid(choice))
		return P2ClientApduChoiceStr(choice);

	if (0 == P2ServerApduChoiceValid(choice))
		return P2ServerApduChoiceStr(choice);

	if (0 == P2SecurityApduChoiceValid(choice))
		return P2SecurityApduChoiceStr(choice);
	return "unknow";
}
int P2FillItemOffsetLud(struct PfillS *fill, int ix, const char *mem, int mem_size)
{
	dve(PfillValid(fill));
	dve(PfillIndexValid(fill, ix));

	// 得到addr的长度
	const int addr_size = PfillIxLen(fill, kP2FillIxA);
	// addr域应该已经填写了
	dvb(0 < addr_size);
	const int offset = P2_LUD_OFFSET(addr_size);
	return offset;
}
//}}}


//{{{ FCS
int P2FcsOffset(const char *frame)
{
	const int l_value = P2LValue(frame);
	const int offset = P2_FCS_OFFSET(l_value);
	return offset;
}
uint16_t P2FcsInFrame(const char *frame)
{
	const int offset = P2FcsOffset(frame);
	const uint8_t low = (uint8_t)frame[offset];
	const uint8_t high = (uint8_t)frame[offset + 1];
	//const uint16_t v = (uint16_t)high * 0x100 + low;
	const uint16_t v = ((uint16_t)high << 8) + low;
	return v;
}
uint16_t P2FcsCalc(const char *frame)
{
	const int offset = P2_FCS_CALC_OFFSET;
	const int l_value = P2LValue(frame);
	const int len = P2_FCS_CALC_LEN(l_value);
	return GetFcs(frame + offset, len);
}
void P2FcsPack(char *frame)
{
	const uint16_t fcs = P2FcsCalc(frame);
	const uint8_t low = fcs & 0x00ff;
	const uint8_t high = (uint8_t)(fcs >> 8);

	const int offset = P2FcsOffset(frame);
	frame[offset] = low;
	frame[offset + 1] = high;
	return;
}
static cp_t FcsValid(const char *frame)
{
	const uint16_t in_frame = P2FcsInFrame(frame);
	const uint16_t calc = P2FcsCalc(frame);
	ifbr(in_frame == calc);
	return 0;
}
static int LenFcs(Pcut *cut, int ix, const char *frame) { return P2_FCS_SIZE; }
static int OffsetFcs(Pcut *cut, int ix, const char *frame) { return P2_FCS_OFFSET(P2LValue(frame)); }
static cp_t ValidFcs(Pcut *cut, int ix, const char *frame) 
{ 
	return FcsValid(frame);
}
int P2FillItemOffsetFcs(struct PfillS *fill, int ix, const char *mem, int mem_size)
{
	dve(PfillValid(fill));
	dve(PfillIndexValid(fill, ix));

	const int addr_size = PfillIxLen(fill, kP2FillIxA);
	dvb(0 < addr_size);
	const int lud_size = PfillIxLen(fill, kP2FillIxLud);
	const int l_value = P2_LUD_SIZE_2_L_VALUE(lud_size, addr_size);
	const int offset = P2_FCS_OFFSET(l_value);
	return offset;
}
cp_t P2FillItemProcessFcs(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const uint16_t fcs = P2FcsCalc(mem);
	mem[offset] = fcs & 0x00ff;
	mem[offset + 1] = (uint8_t)(fcs >> 8);
	*fill_size = P2_FCS_SIZE;
	return 0;
}
//}}}


//{{{ end
uint8_t P2End(const char *frame)
{
	const int l_value = P2LValue(frame);
	const int offset = P2_END_OFFSET(l_value);
	return frame[offset];
}
static int LenEnd(Pcut *cut, int ix, const char *frame) { return P2_END_SIZE; }
static int OffsetEnd(Pcut *cut, int ix, const char *frame) { return P2_END_OFFSET(P2LValue(frame)); }
static cp_t ValidEnd(Pcut *cut, int ix, const char *frame) 
{
	const uint8_t end = P2End(frame);
	ifbr(P2_END == end);
	return 0; 
}
int P2FillItemOffsetEnd(struct PfillS *fill, int ix, const char *mem, int mem_size)
{
	dve(PfillValid(fill));
	dve(PfillIndexValid(fill, ix));

	const int addr_size = PfillIxLen(fill, kP2FillIxA);
	dvb(0 < addr_size);
	const int lud_size = PfillIxLen(fill, kP2FillIxLud);
	const int l_value = P2_LUD_SIZE_2_L_VALUE(lud_size, addr_size);
	const int offset = P2_END_OFFSET(l_value);
	return offset;
}
cp_t P2FillItemProcessEnd(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	mem[offset] = P2_END;
	*fill_size = P2_END_SIZE;
	return 0;
}
//}}}


//{{{ pcut
// 为了节约内存，const部分集中在一起
static const PcutItemFix kCutFix[kP2CutNum] = {
	// name len offset valid explain
	{ kP2NameHead, LenHead, OffsetHead, ValidHead, NULL },
	{ kP2NameL, LenL, OffsetL, ValidL, ExplainL }, 
	{ kP2NameC, LenC, OffsetC, ValidC, ExplainC },
	{ kP2NameAddr, LenAddr, OffsetAddr, ValidAddr, NULL },
	{ kP2NameHcs, LenHcs, OffsetHcs, ValidHcs, NULL },
	{ kP2NameLud, LenLud, OffsetLud, ValidLud, ExplainLud },
	{ kP2NameFcs, LenFcs, OffsetFcs, ValidFcs, NULL },
	{ kP2NameEnd, LenEnd, OffsetEnd, ValidEnd, NULL },
	//{ "all", LenAll, OffsetAll, ValidAll }
};
	

static const PcutItem m_items_pattern[kP2CutNum] = {
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxHead]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxL]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxC]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxA]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxHcs]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxLud]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxFcs]),
	PCUT_ITEM_NO_SUB(&kCutFix[kP2CutIxEnd])
};



static void P2PcutItemsInit(PcutItem items[kP2CutNum])
{
	memcpy(items, m_items_pattern, sizeof(m_items_pattern));
}


cp_t P2PcutOpen(P2Pcut *m)
{
	P2PcutItemsInit(m->items);
	ifer(P2AddrPcutOpen(&m->addr_cut));
	ifer(PcutOpen(&m->base, m->items, kP2CutNum));
	PcutSubSet(&m->base, kP2CutIxA, &m->addr_cut.base, "addr");
	return 0;
}
cp_t P2PcutClose(P2Pcut *m)
{
	dve(P2PcutValid(m));

	PcutSubSet(&m->base, kP2CutIxA, NULL, NULL);
	ifer(PcutClose(&m->base));
	ifer(P2AddrPcutClose(&m->addr_cut));
	return 0;
}
cp_t P2PcutValid(const P2Pcut *m)
{
	ifer(PcutValid(&m->base));
	ifer(P2AddrPcutValid(&m->addr_cut));
	return 0;
}
//}}}

#if 0
//{{{ qpack
// 用sub来填写data
#define FillLud			QpackItemFillBySub
#define FillA			QpackItemFillBySub
/*
static cp_t FillLud(Qpack *pack, char *mem, int mem_size, int offset) 
{
	// 由sub pack填写
	return 0; 
}
static cp_t FillA(Qpack *pack, char *mem, int mem_size, int offset) 
{ 
	// 由sub pack填写
	return 0; 
}
*/
static cp_t FillL(Qpack *pack, int ix, char *mem, int mem_size, int offset)
{
	Pcut * const cut = QpackCut(pack);
	const int addr_size = PcutIxLen(cut, kP2CutIxA, mem);
	const int lud_size = PcutIxLen(cut, kP2CutIxLud, mem);
	const int l_value = P2_LUD_SIZE_2_L_VALUE(lud_size, addr_size);

	dvb(P2_L_OFFSET == offset);
	ifer(P2LValuePack(mem + offset, l_value));
	return 0;
}
static cp_t FillC(Qpack *pack, int ix, char *mem, int mem_size, int offset)
{
	P2Qpack * const pq = (P2Qpack*)pack;
	mem[offset] = P2ControlPack(&pq->control);
	return 0;
}
static cp_t FillHcs(Qpack *pack, int ix, char *mem, int mem_size, int offset)
{
	const uint16_t hcs = P2HcsCalc(mem);
	mem[offset] = hcs & 0x00ff;
	mem[offset + 1] = (uint8_t)(hcs >> 8);
	return 0;
}
static cp_t FillFcs(Qpack *pack, int ix, char *mem, int mem_size, int offset)
{
	const uint16_t fcs = P2FcsCalc(mem);
	mem[offset] = fcs & 0x00ff;
	mem[offset + 1] = (uint8_t)(fcs >> 8);
	return 0;
}
static cp_t FillHead(Qpack *pack, int ix, char *mem, int mem_size, int offset)
{
	mem[offset] = P2_HEAD;
	return 0;
}
static cp_t FillEnd(Qpack *pack, int ix, char *mem, int mem_size, int offset)
{
	mem[offset] = P2_END;
	return 0;
}

// pack_ix
typedef enum
{
	kPackIxLud,
	kPackIxA,
	kPackIxL,
	kPackIxC,
	kPackIxHcs,
	kPackIxFcs,
	kPackIxHead,
	kPackIxEnd,
	kPackNum				
} PackIxT;


// 为了节约内存，const部分集中在一起
// 固定部分
// pack顺序，要保证前一部分填写完毕后，要能推算出后一部分的偏移量。即后填的可以依赖先填的，但不能先填的依赖后填的
static const QpackItemFix kPackFix[kP2PackNum] = {
	// cut_ix	fill
	{ kP2CutIxLud, FillLud },
	{ kP2CutIxA, FillA },
	{ kP2CutIxL, FillL },
	{ kP2CutIxC, FillC },
	{ kP2CutIxHcs, FillHcs },
	{ kP2CutIxFcs, FillFcs },
	{ kP2CutIxHead, FillHead },
	{ kP2CutIxEnd, FillEnd }
};
static const QpackItem kPackItemsPattern[kP2PackNum] = {
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxLud]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxA]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxL]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxC]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxHcs]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxFcs]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxHead]),
	QPACK_ITEM_NO_SUB(&kPackFix[kPackIxEnd])
};
static void P2QpackItemsInit(QpackItem items[kP2PackNum])
{
	memcpy(items, kPackItemsPattern, sizeof(kPackItemsPattern));
}


cp_t P2QpackOpen(P2Qpack *m, Qpack *lud_pack)
{
	dvb(kP2PackNum == kPackNum);
	dve(QpackValid(lud_pack));

	ifer(P2AddrQpackOpen(&m->addr_pack));

	ifer(P2PcutOpen(&m->cut));
	P2QpackItemsInit(m->items);

	ifer(QpackOpen(&m->base, m->items, kP2PackNum, PcutBasePtr(&m->cut.base)));

	QpackSubSet(&m->base, kPackIxA, P2AddrQpackBase(&m->addr_pack));
	QpackSubSet(&m->base, kPackIxLud, lud_pack);
	return 0;
}
cp_t P2QpackClose(P2Qpack *m)
{
	dve(P2QpackValid(m));

	ifer(QpackClose(&m->base));
	ifer(P2PcutClose(&m->cut));
	ifer(P2AddrQpackClose(&m->addr_pack));
	return 0;
}
cp_t P2QpackValid(const P2Qpack *m)
{
	ifer(P2AddrQpackValid(&m->addr_pack));
	ifer(PcutBaseParentValid(&m->cut.base));
	ifer(QpackValid(&m->base));
	return 0;
}
Qpack *P2QpackBase(P2Qpack *m)
{
	dve(P2QpackValid(m));
	return &m->base;
}

Qpack *P2QpackAddr(P2Qpack *m)
{
	dve(P2QpackValid(m));
	return P2AddrQpackBase(&m->addr_pack);
}
void P2QpackSetC(P2Qpack *m, const P2Control *control)
{
	m->control = *control;
}
//}}}
#endif

//{{{ fill
cp_t P2PfillOpen(P2Pfill *m, const P2Control *control, const P2AddrValue *addr)
{
	ifer(PfillOpen(&m->base));

	// 下面的add顺序，要和P2FillIxT定义一致
	// addr长度确定后，才能计算出lud的偏移量
	ifer(PfillAdd(&m->base, &m->addr.base));
	ifer(PfillAdd(&m->base, &m->lud.base));
	// L要在lud、addr填完后，才知道二者长度
	ifer(PfillAdd(&m->base, &m->l.base));
	ifer(PfillAdd(&m->base, &m->c.base));
	ifer(PfillAdd(&m->base, &m->hcs.base));
	ifer(PfillAdd(&m->base, &m->fcs.base));
	ifer(PfillAdd(&m->base, &m->head.base));
	ifer(PfillAdd(&m->base, &m->end.base));
	//ifer(PfillAdd(&m->base, &m->lead.base));
	dvb(kP2FillNum == PfillItemNum(&m->base));

	// 填C的值
	if (NULL != control)
		m->c.control = *control;
	// 填Addr的值
	if (NULL != addr)
		m->addr.addr = *addr;
	return 0;
}
cp_t P2PfillClose(P2Pfill *m)
{
	// 断开lud sub联系
	ifer(P2PfillSetLudSub(m, NULL));
	ifer(PfillClose(&m->base));
	return 0;
}
cp_t P2PfillValid(const P2Pfill *m)
{
	ifer(PfillValid(&m->base));
	ifbr(kP2FillNum == PfillItemNum(&m->base));
	return 0;
}
cp_t P2PfillSetLudSub(P2Pfill *m, Pfill *sub_lud)
{
	dve(P2PfillValid(m));
	if (NULL != sub_lud)
		dve(PfillValid(sub_lud));

	dvb(kP2FillIxLud == PfillFindName(&m->base, kP2NameLud));
	PfillSubSet(&m->base, kP2FillIxLud, sub_lud);
	return 0;
}
//}}}


//{{{ test
#if TEST_EN > 0
//#define TEST_FRAME "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16"
static cp_t TestPcut(void)
{
	P2Pcut p2_pcut = kP2PcutDef; 
	ifer(P2PcutOpen(&p2_pcut));
	Pcut *m = &p2_pcut.base;

	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	const int whole_size = sizeof(whole) - 1;

	//PcutAllPrint(m, 0, whole);

	ifbr(1 ==  PcutIxLen(m, kP2CutIxHead, whole));
	ifbr(2 ==  PcutIxLen(m, kP2CutIxL, whole));
	ifbr(1 ==  PcutIxLen(m, kP2CutIxC, whole));
	// qos_printf("addr_len: %d\r\n", PcutIxLen(kP2CutIxA, whole));
	ifbr(8 ==  PcutIxLen(m, kP2CutIxA, whole));
	ifbr(2 ==  PcutIxLen(m, kP2CutIxHcs, whole));
	ifbr(8 ==  PcutIxLen(m, kP2CutIxLud, whole));
	ifbr(2 ==  PcutIxLen(m, kP2CutIxFcs, whole));
	ifbr(1 ==  PcutIxLen(m, kP2CutIxEnd, whole));
	ifbr(whole_size ==  PcutIxLen(m, kPcutIxAll, whole));

	ifbr(0 == PcutRangeLen(m, kP2CutIxHead, kP2CutIxHead, whole));
	ifbr(1 == PcutRangeLen(m, kP2CutIxHead, kP2CutIxL, whole));
	ifbr(3 == PcutRangeLen(m, kP2CutIxHead, kP2CutIxC, whole));
	ifbr(whole_size == PcutRangeLen(m, 0, kP2CutNum, whole));

	ifbr(0 == PcutIxOffset(m, kP2CutIxHead, whole));
	ifbr(1 ==  PcutIxOffset(m, kP2CutIxL, whole));
	ifbr(3 ==  PcutIxOffset(m, kP2CutIxC, whole));
	ifbr(4 ==  PcutIxOffset(m, kP2CutIxA, whole));
	ifbr(12 ==  PcutIxOffset(m, kP2CutIxHcs, whole));
	ifbr(14 ==  PcutIxOffset(m, kP2CutIxLud, whole));
	ifbr(22 ==  PcutIxOffset(m, kP2CutIxFcs, whole));
	ifbr(24 ==  PcutIxOffset(m, kP2CutIxEnd, whole));
	ifbr(whole_size ==  PcutIxOffset(m, kPcutIxAll, whole));

	ifbr(whole + 0 == PcutIxPtrConst(m, kP2CutIxHead, whole));
	ifbr(whole + 1 ==  PcutIxPtrConst(m, kP2CutIxL, whole));
	ifbr(whole + 3 ==  PcutIxPtrConst(m, kP2CutIxC, whole));
	ifbr(whole + 4 ==  PcutIxPtrConst(m, kP2CutIxA, whole));
	ifbr(whole + 12 ==  PcutIxPtrConst(m, kP2CutIxHcs, whole));
	ifbr(whole + 14 ==  PcutIxPtrConst(m, kP2CutIxLud, whole));
	ifbr(whole + 22 ==  PcutIxPtrConst(m, kP2CutIxFcs, whole));
	ifbr(whole + 24 ==  PcutIxPtrConst(m, kP2CutIxEnd, whole));
	ifbr(whole + whole_size == PcutIxPtrConst(m, kPcutIxAll, whole));

	ifer(PcutIxValid(m, kP2CutIxHead, whole));
	ifer(PcutIxValid(m, kP2CutIxL, whole));
	ifer(PcutIxValid(m, kP2CutIxC, whole));
	ifer(PcutIxValid(m, kP2CutIxA, whole));
	ifer(PcutIxValid(m, kP2CutIxHcs, whole));
	ifer(PcutIxValid(m, kP2CutIxLud, whole));
	ifer(PcutIxValid(m, kP2CutIxFcs, whole));
	ifer(PcutIxValid(m, kP2CutIxEnd, whole));
	ifer(PcutIxValid(m, kPcutIxAll, whole));

	// 取sa_naked，即纯地址部分
	Pcut *cut_addr = PcutSub(m, kP2CutIxA);
	ifbr(NULL != cut_addr);
	const char kSaNaked[] = "\x01\x23\x45\x67\x89\x0a";
	const int kSaNakedSize = sizeof(kSaNaked) - 1;

	const char *addr_cut_mem = PcutIxPtrConst(m, kP2CutIxA, whole);
	const int sa_naked_len = PcutIxLen(cut_addr, kP2AddrCutIxSaNaked, addr_cut_mem);
	const char *sa_naked_mem = PcutIxPtrConst(cut_addr, kP2AddrCutIxSaNaked, addr_cut_mem);
	ifbr(kSaNakedSize == sa_naked_len);
	ifbr(0 == memcmp(sa_naked_mem, kSaNaked, kSaNakedSize));

	if (PRINT_PART_IN_TEST_EN)
		PcutAllPrint(m, 0, whole);
	
	ifer(P2PcutClose(&p2_pcut));
	return 0;
}
static cp_t TestValid(void)
{
	P2Pcut p2_pcut; 
	ifer(P2PcutOpen(&p2_pcut));
	//Pcut *m = PcutBasePtr(&p2_pcut.base);
	Pcut *m = &p2_pcut.base;

	{
	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x69\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	//const int frame_size = sizeof(whole) - 1;
	ifbr(0 != PcutIxValid(m, kPcutIxAll, whole));
	ifbr(kP2CutIxHead == PcutInvalidIx(m, whole));
	}

	{
	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x8a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x16";
	//const int frame_size = sizeof(whole) - 1;
	ifbr(0 != PcutIxValid(m, kPcutIxAll, whole));
	ifbr(kP2CutIxHcs == PcutInvalidIx(m, whole));
	}

	{
	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x04\x16";
	//const int frame_size = sizeof(whole) - 1;
	ifbr(0 != PcutIxValid(m, kPcutIxAll, whole));
	ifbr(kP2CutIxFcs == PcutInvalidIx(m, whole));
	}

	{
	// 68 17 00 43 45 01 23 45 67 89 0a 00 7a 4e 05 01 00 40 01 02 00 00 ed 03 16
	const char whole[] = "\x68\x17\x00\x43\x45\x01\x23\x45\x67\x89\x0a\x00\x7a\x4e\x05\x01\x00\x40\x01\x02\x00\x00\xed\x03\x17";
	//const int frame_size = sizeof(whole) - 1;
	ifbr(0 != PcutIxValid(m, kPcutIxAll, whole));
	ifbr(kP2CutIxEnd == PcutInvalidIx(m, whole));
	}
	ifer(P2PcutClose(&p2_pcut));
	return 0;
}


#define LUD_CONTENT_SIZE		(2)
typedef struct 
{
	PfillItem base;
	char content[2];
} FillItemLudTest;
static cp_t FillProcessLud(struct PfillS *fill, int level, int ix, char *mem, int mem_size, int offset, int *fill_size)
{
	const PfillItem *fi = PfillIxItemConst(fill, ix);
	const FillItemLudTest *derive = (FillItemLudTest*)fi;
	memcpy(mem + offset, derive->content, LUD_CONTENT_SIZE);
	*fill_size = LUD_CONTENT_SIZE;
	return 0;
}
#define kFillItemLudTestDef {			\
	PFILL_ITEM(kP2NameLud, PfillItemOffsetFollow, FillProcessLud, -1, NULL),	\
	{'1', '2'},		\
}


#define TEST_FILL_L_MAX				(P2_LUD_SIZE_2_L_VALUE(LUD_CONTENT_SIZE, kP2AddrSizeMax))
#define TEST_FILL_FRAME_SIZE_MAX	(P2_L_VALUE_2_FRAME_SIZE(TEST_FILL_L_MAX))
static char mem[TEST_FILL_FRAME_SIZE_MAX];
static cp_t TestFill(void)
{
	// lud-sub
	Pfill lud = kPfillDef;
	ifer(PfillOpen(&lud));
	FillItemLudTest lud_item = kFillItemLudTestDef;
	ifer(PfillAdd(&lud, &lud_item.base));

	// p2-fill
	const P2Control control = { kP2DirPrmClientRequest, 0, 0, kP2FunCodeUserData };
	const P2AddrValue addr = { kP2AddrSaTypeGroup, {1,2,3,4,5,6}, 6, 1, 0x78 };
	P2Pfill fill = kP2PfillDef(&lud);
	ifer(P2PfillOpen(&fill, &control, &addr));
	ifer(P2PfillValid(&fill));
	
	// 设置lud，在kP2PfillDef()中填写了sub_lud，可以不调用本函数
	P2PfillSetLudSub(&fill, &lud);
	//ifer(PfillSubSet(&fill.base, kP2FillIxLud, &lud));
	
	if (PRINT_FILL_IN_TEST_EN)
		PfillSetPrintFillWithSubs(&fill.base, PfillPrintFillNormal);

	// 填C的值
	//const P2Control control = { kP2DirPrmClientRequest, 0, 0, kP2FunCodeUserData };
	fill.c.control = control;
	// 填Addr的值
	fill.addr.addr = addr;
	// 填Lud的值
	memcpy(lud_item.content, "12", LUD_CONTENT_SIZE);

	const int mem_size = sizeof(mem);
	memset(mem, 0, mem_size);
	ifer(PfillDo(&fill.base, 0, mem, mem_size));

	const int frame_size = P2FrameSize(mem);
	//qos_printf("frame_size=%d frame=", frame_size);
	//printf_hex(mem, frame_size, "");
	//qos_printf("\r\n");
	// frame_size=19 frame=681100009501020304050678fa463132354116
	if (PRINT_FILL_IN_TEST_EN)
		printf_hex_ex("frame: ", "\r\n", mem, frame_size, "");
	
	// 不是一个合法帧，在解帧软件上，解不出来
	ifbr(19 == frame_size);
	// 6811004395010203040506789e763132354116
	ifbr(0 == memcmp(mem, "\x68\x11\x00\x43\x95\x01\x02\x03\x04\x05\x06\x78\x9e\x76\x31\x32\x35\x41\x16", frame_size));

	ifer(P2PfillClose(&fill));
	ifer(PfillClose(&lud));
	return 0;
}


static const QtestItem kTestItem[] = {
	TestPcut,
	TestValid,
	TestFill
};
#define kTestItemNum	(sizeof(kTestItem)/sizeof(kTestItem[0]))
cp_t P2BaseTest(QTEST_ARG)
{
	return QtestTest(__FILE__, run_times, kTestItem, kTestItemNum, verbose);
}
#else
cp_t P2BaseTest(QTEST_ARG)
{
	return 0;
}
#endif
//}}}

