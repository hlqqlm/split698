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

20230303---------------------------------------------------------------------
huanglin create
本文档用来作为dlt698报文解析、组帧的基础部分。
*/
// standard library
#include <string.h>
#include <iomanip>      // std::setw

#include "qos/qtime.h"
//#include "qos/qnum.h"
#include "qos/qbit.h"
#include "a_xdr.h"
#include "qdlt698.h"
#include "qdlt698.xcp.h"
//#define this_file_id	0xdf5373d4 // echo -n qdlt698.c | rhash --simple -

#define TEST_EN				(0)
#define TEST_RUN_TIMES		(10)	// 测试运行次数


//{{{ mem
/*
参考：
#define PutFrmByte(frm, x) do {*(frm->detxbuf + frm->txdepointer++) = x;} while (0)
#define PutFrmU16(frm, x) do {*(frm->detxbuf + frm->txdepointer++) = (x / 0x100); *(frm->detxbuf + frm->txdepointer++) = (x % 0x100); } while (0)
#define PutFrmU32(frm, x) do { PutFrmU16(frm, x / 0x10000); PutFrmU16(frm, x % 0x10000);} while (0)
#define PutFrmU64(frm, x) do { PutFrmU32(frm, x / 0x100000000); PutFrmU32(frm, x % 0x100000000);} while (0)
#define PutFrmMem(frm, x, len) do {memcpy(frm->detxbuf + frm->txdepointer, x, len); frm->txdepointer += len;} while (0)
#define PutFrmReMem(frm, x, len) do {xMemReCopy(frm->detxbuf + frm->txdepointer, x, len); frm->txdepointer += len; } while (0)
#define PutFrmBcdByte(frm, x) do {*(frm->detxbuf + frm->txdepointer++) = BinToBcd(x);} while (0)
#define PutFrmBcdWord(frm, x) do {*(frm->detxbuf + frm->txdepointer++) = BinToBcd(x % 100); *(frm->detxbuf + frm->txdepointer++) = BinToBcd(x / 100); } while (0)

#define PutPKGByte(pkg, x) do {*(pkg->txptr + pkg->txlen++) = x; } while (0)
#define PutPKGWord(pkg, x) do {*(pkg->txptr + pkg->txlen++) = (x % 0x100); *(pkg->txptr + pkg->txlen++) = (x / 0x100);} while (0)
#define PutPKGMem(pkg, x, len) do {if (x != NULL) memcpy(pkg->txptr + pkg->txlen, x, len); pkg->txlen += len;} while (0)
#define PutPKGBcdByte(pkg, x) do {*(pkg->txptr + pkg->txlen++) = BinToBcd(x);} while (0)
#define PutPKGBcdWord(pkg, x) do{*(pkg->txptr + pkg->txlen++) = BinToBcd(x % 100); *(pkg->txptr + pkg->txlen++) = BinToBcd(x / 100);} while (0)

*/


#define	GetBufU16(buf,x) do{x=(*buf++)<<8;x|=*buf++;}while(0)
#define	GetBufU32(buf,x) do{x=(*buf++)<<24;x|=(*buf++)<<16;x|=(*buf++)<<8;x|=*buf++;}while(0)
uint16_t Dlt698Uint16(const char *mem)
{
	const unsigned char *um = (const unsigned char *)mem;
	uint16_t ret = 0;
	GetBufU16(um, ret);
	return ret;
}
uint32_t Dlt698Uint32(const char *mem)
{
	const unsigned char *um = (const unsigned char *)mem;
	uint32_t ret = 0;
	GetBufU32(um, ret);
	return ret;
}
#define PutFrmByte(frm, x) do {*(frm->detxbuf + frm->txdepointer++) = x;} while (0)
#define PutFrmU16(frm, x) do {*(frm->detxbuf + frm->txdepointer++) = (x / 0x100); *(frm->detxbuf + frm->txdepointer++) = (x % 0x100); } while (0)
#define PutFrmU32(frm, x) do { PutFrmU16(frm, x / 0x10000); PutFrmU16(frm, x % 0x10000);} while (0)
#define PutFrmU64(frm, x) do { PutFrmU32(frm, x / 0x100000000); PutFrmU32(frm, x % 0x100000000);} while (0)
void Dlt698Uint16Mem(char *mem, uint16_t value)
{
	mem[0] = (value >> 8);	// value / 0x100
	mem[1] = value & 0xff;	// value % 0x100
}
void Dlt698Uint32Mem(char *mem, uint32_t value)
{
	mem[0] = (value >> 24);	
	mem[1] = (value >> 16);	
	mem[2] = (value >> 8);	
	mem[3] = value & 0xff;
}
//}}}


//{{{ oad
// 698报文转换成oad
OadT Dlt698Mem2Oad(const char mem[])
{
	const unsigned char * const um = (const unsigned char *)mem;
	OadT oad = (OadT)(um[0]) << 24;
	oad += (OadT)(um[1]) << 16;
	oad += (OadT)(um[2]) << 8;
	oad += (OadT)(um[3]);
	return oad;
	//return Dlt698Uint32((const unsigned char *)mem);
}
void Dlt698Oad2Mem(char mem[OAD_MEM_SIZE], OadT oad)
{
	mem[0] = (uint8_t)((oad >> 24) & 0xff);
	mem[1] = (uint8_t)((oad >> 16) & 0xff);
	mem[2] = (uint8_t)((oad >> 8) & 0xff);
	mem[3] = (uint8_t)(oad & 0xff);
}
//}}}


//{{{ ti
/*TI∷=SEQUENCE
{
单位 ENUMERATED
{
秒 （0），
分 （1），
时 （2），
日 （3），
月 （4），
 年 （5）
 }，
 间隔值 long-unsigned
}
 */

cp_t Dlt698TiUnitValid(uint8_t unit)
{
	ifbr(0 <= unit);
	ifbr(unit < kDlt698TiUnitNum);
	return 0;
}
cp_t Dlt698TiValid(const char mem[DLT698_TI_SIZE])
{
	Dlt698Ti value;
	Dlt698TiValue(&value, mem);
	ifer(Dlt698TiUnitValid(value.unit));
	return 0;
}
void Dlt698TiValue(Dlt698Ti *ti, const char mem[DLT698_TI_SIZE])
{
	ti->unit = mem[0];
	ti->interval = Dlt698Uint16(&mem[1]);
}
void Dlt698TiMem(char mem[DLT698_TI_SIZE], const Dlt698Ti *ti)
{
	mem[0] = ti->unit;
	Dlt698Uint16Mem(mem + 1, ti->interval);
}
//}}}


//{{{ datetimes
cp_t Dlt698DatetimesValid(const char mem[kDlt698DatetimeSSize])
{
	qos_printf("Dlt698DatetimesValid todo\r\n");
	return 0;
}
void Dlt698DatetimesValue(Dlt698DatetimeS *dts, const char mem[kDlt698DatetimeSSize])
{
	dts->year = Dlt698Uint16(mem);
	dts->month = mem[2];
	dts->day = mem[3];
	dts->hour = mem[4];
	dts->minute = mem[5];
	dts->second = mem[6];
}
void Dlt698DatetimesMem(char mem[kDlt698DatetimeSSize], const Dlt698DatetimeS *dts)
{
	Dlt698Uint16Mem(mem, dts->year);
	mem[2] = dts->month;		// 1 ~ 12
	mem[3] = dts->day;			// 1 ~ 31
	mem[4] = dts->hour;			// 0 ~ 23
	mem[5] = dts->minute;		// 0 ~ 59
	mem[6] = dts->second;		// 0 ~ 59
}



cp_t Dlt698DatetimeSPtrEqual(const Dlt698DatetimeS *p, const Dlt698DatetimeS *q)
{   
	ifbr(NULL != p);
	ifbr(NULL != q);
	const Dlt698DatetimeS &x = *((const Dlt698DatetimeS *)p);
	const Dlt698DatetimeS &y = *((const Dlt698DatetimeS *)q);
	return Dlt698DatetimeSEqual(x, y);
}
cp_t Dlt698DatetimeSEqual(const Dlt698DatetimeS &x, const Dlt698DatetimeS &y)
{   
	ifbr(x.year == y.year);  
	ifbr(x.month == y.month);
	ifbr(x.day == y.day);   
	ifbr(x.minute == y.minute);
	ifbr(x.second == y.second);
	return 0;
}
bool operator==(const Dlt698DatetimeS &x, const Dlt698DatetimeS &y)
{   
	return (0 == Dlt698DatetimeSEqual(x, y)); 
}   

static std::ostream &DateTimeSPrintYear(std::ostream &os, unsigned int year)
{   
	if (0xffff == year)
		return os << "xxxx";

	return os << year;
}
static std::ostream &DateTimeSPrintUint8(std::ostream &os, uint8_t x)
{       
	if (0xff == x)
		return os << "xx";

	return os << (int)x;
}   

cp_t Dlt698DatetimeSPrint(std::ostream &os, const Dlt698DatetimeS *dts)
{
	ifbr(NULL != dts);
	const Dlt698DatetimeS &r = *dts;
	os << std::dec << std::right << std::setfill(' ') << std::setw(4);
	DateTimeSPrintYear(os, r.year) << "-" << std::right << std::setfill('0') << std::setw(2);
	DateTimeSPrintUint8(os, r.month) << "-" << std::right << std::setfill('0') << std::setw(2);
	DateTimeSPrintUint8(os, r.day) << " " << std::right << std::setfill('0') << std::setw(2);
	DateTimeSPrintUint8(os, r.hour) << ":" << std::right << std::setfill('0') << std::setw(2);
	DateTimeSPrintUint8(os, r.minute) << ":" << std::right << std::setfill('0') << std::setw(2);
	DateTimeSPrintUint8(os, r.second);
	return 0;
}
std::ostream & operator<<(std::ostream &os, const Dlt698DatetimeS &dts)
{
	if (0 != Dlt698DatetimeSPrint(os, &dts))
		os << "Error in print Dlt698DatetimeS.";
	return os;
}



//}}}


//{{{ datetime
cp_t Dlt698DatetimeValid(const char mem[kDlt698DatetimeSize])
{
	// todo
	return 0;
}
void Dlt698DatetimeValue(Dlt698Datetime *dt, const char mem[kDlt698DatetimeSize])
{
	dt->year = Dlt698Uint16(mem);
	dt->month = mem[2];
	dt->day_of_month = mem[3];
	dt->day_of_week = mem[4];
	dt->hour = mem[5];
	dt->minute = mem[6];
	dt->second = mem[7];
	dt->milliseconds = Dlt698Uint16(mem + 8);
}
void Dlt698DatetimeMem(char mem[kDlt698DatetimeSize], const Dlt698Datetime *dt)
{
	Dlt698Uint16Mem(mem, dt->year);
	mem[2] = dt->month;		
	mem[3] = dt->day_of_month;
	mem[4] = dt->day_of_week;
	mem[5] = dt->hour;		
	mem[6] = dt->minute;	
	mem[7] = dt->second;	
	Dlt698Uint16Mem(mem + 8, dt->milliseconds);
}


cp_t Dlt698DatetimePtrEqual(const Dlt698Datetime *p, const Dlt698Datetime *q)
{   
	ifbr(NULL != p);
	ifbr(NULL != q);
	const Dlt698Datetime &x = *((const Dlt698Datetime *)p);
	const Dlt698Datetime &y = *((const Dlt698Datetime *)q);
	return Dlt698DatetimeEqual(x, y);
}
cp_t Dlt698DatetimeEqual(const Dlt698Datetime &x, const Dlt698Datetime &y)
{   
	ifbr(x.year == y.year);			// long-unsigned，
	ifbr(x.month == y.month);			// unsigned，
	ifbr(x.day_of_month == y.day_of_month);	// unsigned，
	ifbr(x.day_of_week == y.day_of_week);	// unsigned，
	ifbr(x.hour == y.hour);			// unsigned，
	ifbr(x.minute == y.minute);			// unsigned，
	ifbr(x.second == y.second);			// unsigned，
	ifbr(x.milliseconds == y.milliseconds);	// long-unsigned
	return 0;
}
bool operator==(const Dlt698Datetime &x, const Dlt698Datetime &y)
{   
	return (0 == Dlt698DatetimeEqual(x, y)); 
}   



static std::ostream &DatetimePrintYear(std::ostream &os, unsigned int year)
{   
	if (0xffff == year)
		return os << "xxxx";

	return os << year;
}
static std::ostream &DatetimePrintMs(std::ostream &os, unsigned int ms)
{   
	if (0xffff == ms)
		return os << "xxxx";

	return os << ms;
}
static std::ostream &DatetimePrintUint8(std::ostream &os, uint8_t x)
{       
	if (0xff == x)
		return os << "xx";

	return os << (int)x;
}   
cp_t Dlt698DatetimePrint(std::ostream &os, const Dlt698Datetime *dt)
{
	ifbr(NULL != dt);
	const Dlt698Datetime &r = *dt;
	os << std::dec << std::right << std::setfill(' ') << std::setw(4);
	DatetimePrintYear(os, r.year) << "-" << std::right << std::setfill('0') << std::setw(2);
	DatetimePrintUint8(os, r.month) << "-" << std::right << std::setfill('0') << std::setw(2);
	DatetimePrintUint8(os, r.day_of_month) << " " << std::right << std::setfill('0') << std::setw(2);
	DatetimePrintUint8(os, r.hour) << ":" << std::right << std::setfill('0') << std::setw(2);
	DatetimePrintUint8(os, r.minute) << ":" << std::right << std::setfill('0') << std::setw(2);
	DatetimePrintUint8(os, r.second) << ":" << std::right << std::setfill('0') << std::setw(4);
	DatetimePrintMs(os, r.milliseconds) << " " << "week" << std::setw(1) << ((unsigned int)(r.day_of_week) + 1);	// unsigned，0是周一
	return 0;
}
std::ostream & operator<<(std::ostream &os, const Dlt698Datetime &dt)
{
	if (0 != Dlt698DatetimePrint(os, &dt))
		os << "Error in print Dlt698Datetime.";
	return os;
}

//}}}


//{{{ bit-string
int Qdlt698BitStringContentBit(const unsigned char *mem)
{
	return mem[0];
}
int Qdlt698BitStringContentByte(const unsigned char *mem)
{
	const int bit = Qdlt698BitStringContentBit(mem);
	const int tail = (0 == (bit % 8)) ? 0 : 1;
	const int byte = bit / 8 + tail;
	return byte;
}
// 长度域字节数
int Qdlt698BitStringLenByte(const unsigned char *mem)
{
	return 1;
}
int Qdlt698BitStringLenContentByte(const unsigned char *mem)
{
	return Qdlt698BitStringLenByte(mem) + Qdlt698BitStringContentByte(mem);
}
int Qdlt698BitStringParse(unsigned char *dst, int dst_size, const unsigned char *mem)
{
	const int byte = Qdlt698BitStringContentByte(mem);
	// const int copy = min(byte, dst_size);
	const int copy = byte > dst_size ? dst_size : byte;

	const int len_byte = Qdlt698BitStringLenByte(mem);
	const unsigned char *bit_start = mem + len_byte;
	memcpy(dst, bit_start, copy);
	return copy;
}
//}}}

#if 0
//{{{ date_time_s
void Qdlt698DatetimeSParse(DatetimeS *dst, const unsigned char *mem)
{
	dst->year = Dlt698Uint16((const char *)mem);
	mem += 2;
	dst->month = *mem++;    // 1 ~ 12
	dst->day = *mem++;              // 1 ~ 31
	dst->hour = *mem++;             // 0 ~ 23
	dst->minute = *mem++;   // 0 ~ 59
	dst->second = *mem++;   // 0 ~ 59
	return;
}
//}}}
#endif

//{{{ octet-string
int Qdlt698OctetStringLenSize(const char *mem)
{
	return VariableLenIntByteNum(*mem);
	//return (VariableLenIntContentByteNum(*mem) + 1);
}
int Qdlt698OctetStringContentSize(const char *mem)
{
	return VariableLenIntValue(mem);
}
int Qdlt698OctetStringLenContentSize(const char *mem)
{
	return Qdlt698OctetStringLenSize(mem) + Qdlt698OctetStringContentSize(mem);
}
int Qdlt698OctetStringTypeLenContentSize(const char *memtype)
{
	const char * const mem = memtype + kDlt698DatatypeSize;	// 第一个字节是类型
	return kDlt698DatatypeSize + Qdlt698OctetStringLenContentSize(mem);
}
//}}}


//{{{ piid
void P2PiidPart(P2Piid *dst, uint8_t piid)
{
	dst->service_priority = piid & 0x80;	// bit7（服务优先级）——0：普通优先级，1：高优先级，在应答 APDU中，其值与请求的APDU相同。
	dst->service_code = piid & 0x3f;		// bit0…bit5（服务序号）——二进制编码表示 0…63，在应答 APDU 中，其值与请求的 APDU 相同。
}
uint8_t P2PiidFill(const P2Piid *split)
{
	uint8_t value = split->service_code;
	value = BIT_SET_VALUE(value, 7, split->service_priority);
	return value;
}
//}}}


//{{{ piid-acd
void P2PiidAcdPart(P2PiidAcd *dst, uint8_t piid_acd)
{
	dst->service_priority = piid_acd & 0x80;	// bit7（服务优先级）——0：普通优先级，1：高优先级，在应答 APDU中，其值与请求的APDU相同。
	dst->request_acd = piid_acd & 0x40;		// bit6（请求访问 ACD）——0：不请求，1：请求。
	dst->service_code = piid_acd & 0x3f;		// bit0…bit5（服务序号）——二进制编码表示 0…63，在应答 APDU 中，其值与请求的 APDU 相同。
}
uint8_t P2PiidAcdFill(const P2PiidAcd *split)
{
	uint8_t value = split->service_code;
	value = BIT_SET_VALUE(value, 7, split->service_priority);
	value = BIT_SET_VALUE(value, 6, split->request_acd);
	return value;
}
//}}}


//{{{ dar
cp_t P2DarValid(uint8_t dar)
{
	if (kP2DarSuccess <= dar && dar <= kP2DarCompareFailure)
		return 0;
	if (kP2DarOtherReason == dar)
		return 0;
	return cph;
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
void Qdlt698Test(void)
{
	QtestTest("qdlt698", TEST_RUN_TIMES, kTestItem, kTestItemNum);
}

#else
void Qdlt698Test(void)
{
	return;
}
#endif
//}}}

