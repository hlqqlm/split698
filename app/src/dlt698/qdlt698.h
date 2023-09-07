#ifndef _QDLT698_H
#define _QDLT698_H



#include "qos/qcp.h"
#include "qos/qtime.h"


// 类型字节长度
#define kDlt698DatatypeSize		(1)
#define kDlt698DatatypeInvalid	(0xffff)
#define kDlt698DatatypeName		"datatype"
// 表4 接口类及对象实例使用的数据类型定义
typedef enum 
{
	kDlt698DataTypeNull = 0,	// NULL 0 空
	kDlt698DataTypeArray = 1,	// array 1 SEQUENCE OF Data（见 7.3.1 ） 数组的元素在对象属性或方法的描述中定义
	kDlt698DataTypeStructure = 2,	//structure 2 SEQUENCE OF Data（见 7.3.1 ） 结构的元素在对象属性或方法的描述中定义
	kDlt698DataTypeBool = 3,	// bool 3 布尔值 1 或 0
	kDlt698DataTypeBitString = 4,	// bit-string 4 位串
	kDlt698DataTypeDoubleLong = 5,	// double-long 5 32 位整数 -2^31…2^31-1
	kDlt698DataTypeDoubleLongUnsigned = 6,	//double-long-unsigned 6 32 位正整数 0…2^32-1
											// 保留 7-8
	kDlt698DataTypeOctetString = 9,			// octet-string 9 8 位字节串
	kDlt698DataTypeVisibleString = 10,		// visible-string 10 ASCII 字符串
											// 保留 11
											/*
UTF8-string 12 UTF-8 编码的字符串
保留 13-14
*/
	kDlt698DataTypeInteger = 15,		// integer 15 8 位整数 -128…127
	kDlt698DataTypeLong = 16,			// long 16 16 位整数 -32768…32767
	kDlt698DataTypeUnsigned = 17,		// unsigned 17 8 位正整数 0…255
	kDlt698DataTypeLongUnsigned = 18,	// long-unsigned 18 16 位正整数 0…65535
/*
保留 19
*/
	kDlt698DataTypeLong64 = 20,			// long64 20 64 位整数 -2 63…2^63-1
	/*
long64-unsigned 21 64 位正整数 0…2^64-1
*/
	kDlt698DataTypeEnum = 22,			// enum 22 枚举的元素在对象属性或方法的描述中定义 0…255
	/*
float32 23 32 位浮点数
float64 24 64 位浮点数
*/
	kDlt698DataTypeDatetime = 25,	// date_time 25 octet-string（SIZE（10））
/*
date 26 octet-string（SIZE（5））
time 27 octet-string（SIZE（3））
*/
	kDlt698DataTypeDatetimeS = 28,		// date_time_s 28 octet-string（SIZE（7））
	/*
保留 29-79
OI 80 见 0
*/
	kDlt698DataTypeOad = 81,		// OAD 81 见 0
	kDlt698DataTypeRoad = 82,		// ROAD 82 见 0
/*
Q/GDW XXXXX—XXXX
15
类型描述 标记 定义 数值范围
OMD 83 见 0
TI 84 见 0
*/
	kDlt698DataTypeTsa = 85,		// TSA 85 见 0
/*
MAC 86 见 0
*/
	kDlt698DataTypeRn = 87,		// RN 87 见 0
/*
Region 88 见 0
Scaler_Unit 89 见 0
RSD 90 见 0
CSD 91 见 0
MS 92 见 0
SID 93 见 0
SID_MAC 94 见 0
COMDCB 95 见 0
RCSD 96 见 0
VQDS 97 见 0
保留 98-255
*/
} Qdlt698DateTypeT;


typedef uint16_t OiT;	// OI	80	见6.3.3.24　	
typedef uint32_t OiAttrT;	// oi + attrib, 例如：0xF20602 -> oi是F206, attr是02
typedef uint32_t OadT;		// OAD	Object Attribute Descriptor	对象属性描述符 OAD	81	见6.3.3.5　	
				// 代码中大量用到oad00的概念，实际指尾部是00的oad，即oi + attrib + 00
				// 这样，避免引入oi + attrib的概念
//ROAD	82	见6.3.3.6　	
typedef uint32_t OmdT;		// OMD	Object Method Descriptor	对象方法描述符 OMD	83	见6.3.3.12　	
typedef uint8_t  AttribT;	// 属性号, 1)bit0…bit4编码表示对象属性编号，取值0…31，其中0表示整个对象属性，即对象的所有属性；
typedef uint8_t  ItemixT;	// 元素索引，属性内元素索引——00H表示整个属性全部内容。如果属性是一个结构或数组，01H指向对象属性的第一个元素；如果属性是一个记录型的存储区，非零值n表示最近第n次的记录。
typedef uint8_t MethodixT;	// 方法标识——即对象方法编号。

typedef uint8_t SwitchStatusT;	// 开关状态，0合1分
typedef uint8_t RelaysSwitchStatusT;	// 所有继电器开关状态, 0合1分, TODO:更名为UnionSwitchStatus,联合开关状态，因为有时候表示的不是继电器的状态，例如led状态

// int
#define kDlt698UnsignedSize				(1)
#define kDlt698LongUnsignedSize			(2)
#define kDlt698DoubleLongUnsignedSize	(4)
uint16_t Dlt698Uint16(const char *mem);
uint32_t Dlt698Uint32(const char *mem);
void Dlt698Uint16Mem(char *mem, uint16_t value);
void Dlt698Uint32Mem(char *mem, uint32_t value);

// oad
#define OAD_MEM_SIZE		(4)
#define kDlt698OadInvalid	(0xffffffff)
OadT Dlt698Mem2Oad(const char mem[]);
void Dlt698Oad2Mem(char mem[OAD_MEM_SIZE], OadT oad);

#define OMD_MEM_SIZE		(4)

// ti
#define DLT698_TI_SIZE		(3)
cp_t Dlt698TiUnitValid(uint8_t unit);
cp_t Dlt698TiValid(const char mem[DLT698_TI_SIZE]);
typedef enum
{
	kDlt698TiUnitSecond = 0,
	kDlt698TiUnitMinute = 1,
	kDlt698TiUnitHour = 2,
	kDlt698TiUnitDay = 3,
	kDlt698TiUnitMonth = 4,
	kDlt698TiUnitYear = 5,
	kDlt698TiUnitNum
} Dlt698TiUnit;
typedef struct 
{
	uint8_t unit;
	uint16_t interval;
} Dlt698Ti;
#define kDlt698TiDef { kDlt698TiUnitSecond, 0 }
void Dlt698TiValue(Dlt698Ti *ti, const char mem[DLT698_TI_SIZE]);
void Dlt698TiMem(char mem[DLT698_TI_SIZE], const Dlt698Ti *ti);

// datetimes
// 示例：1c 07 e6 09 1d 0c 2e 00  类型28，值:2022-09-29 12:46:00
#define kDlt698DatetimeSSize	(7)
cp_t Dlt698DatetimesValid(const char mem[kDlt698DatetimeSSize]);
typedef struct Dlt698DatetimeSS
{
	uint16_t year;
	unsigned char month;	// 1 ~ 12
	unsigned char day;		// 1 ~ 31
	unsigned char hour;		// 0 ~ 23
	unsigned char minute;	// 0 ~ 59
	unsigned char second;	// 0 ~ 59
} Dlt698DatetimeS;
#define kDlt698DatetimeSDef { 0, 0, 0, 0, 0, 0 }
void Dlt698DatetimesValue(Dlt698DatetimeS *dts, const char mem[kDlt698DatetimeSSize]);
void Dlt698DatetimesMem(char mem[kDlt698DatetimeSSize], const Dlt698DatetimeS *dts);

cp_t Dlt698DatetimeSPtrEqual(const Dlt698DatetimeS *p, const Dlt698DatetimeS *q);
cp_t Dlt698DatetimeSEqual(const Dlt698DatetimeS &x, const Dlt698DatetimeS &y);
bool operator==(const Dlt698DatetimeS &x, const Dlt698DatetimeS &y);

cp_t Dlt698DatetimeSPrint(std::ostream &os, const Dlt698DatetimeS *dts);
std::ostream & operator<<(std::ostream &os, const Dlt698DatetimeS &dts);

// date_time_s
// 暂时保留，可以用Dlt698Datetime系列函数代替
// void Qdlt698DatetimeSParse(DatetimeS *dst, const unsigned char *mem);

// datetime
// |-request_timedate_time(u16,u8,u8,week,u8,u8,u8,ms)	07 E7 07 05 03 0F 18 11 00 00	2023-07-05 15:24:17    0ms 周4	
/*
date_time∷=SEQUENCE
{
	year long-unsigned，
	month unsigned，
	day_of_month unsigned，
	day_of_week unsigned，
	hour unsigned，
	minute unsigned，
	second unsigned，
	milliseconds long-unsigned
}
*/
#define kDlt698DatetimeSize		(10)
cp_t Dlt698DatetimeValid(const char mem[kDlt698DatetimeSize]);
typedef struct Dlt698DatetimeStruct
{
	uint16_t year;			// long-unsigned，
	uint8_t month;			// unsigned，
	uint8_t day_of_month;	// unsigned，
	uint8_t day_of_week;	// unsigned，0是周一
	uint8_t hour;			// unsigned，
	uint8_t minute;			// unsigned，
	uint8_t second;			// unsigned，
	uint16_t milliseconds;	// long-unsigned
} Dlt698Datetime;
#define kDlt698DatetimeDef { 0, 0, 0, 0, 0, 0, 0, 0 }
//example:   |-request_timedate_time(u16,u8,u8,week,u8,u8,u8,ms)	07 E7 07 0B 02 10 2B 3B 00 00	2023-07-11 16:43:59    0ms 周3	
void Dlt698DatetimeValue(Dlt698Datetime *dt, const char mem[kDlt698DatetimeSize]);
void Dlt698DatetimeMem(char mem[kDlt698DatetimeSize], const Dlt698Datetime *dt);

//cp_t Dlt698DatetimeEqual(const Dlt698Datetime *x, const Dlt698Datetime *y);
cp_t Dlt698DatetimePtrEqual(const Dlt698Datetime *p, const Dlt698Datetime *q);
cp_t Dlt698DatetimeEqual(const Dlt698Datetime &x, const Dlt698Datetime &y);
bool operator==(const Dlt698Datetime &x, const Dlt698Datetime &y);

cp_t Dlt698DatetimePrint(std::ostream &os, const Dlt698Datetime *dt);
std::ostream & operator<<(std::ostream &os, const Dlt698Datetime &dt);
std::string Dlt698DatetimeStr(const Dlt698Datetime &dt);

// 约定： 报文变量名
// 如果为mem，表示不包括类型字节
// 如果为memtype，表示包括类型字节


// bit-string
// 示例：04 08 c0 类型4,8bit
// 这里只考虑长度为一字节情况。
// bit数
int Qdlt698BitStringContentBit(const unsigned char *mem);
// 字节数
int Qdlt698BitStringContentByte(const unsigned char *mem);
// 长度域字节数
int Qdlt698BitStringLenByte(const unsigned char *mem);
// 长度域 + 内容域字节数
int Qdlt698BitStringLenContentByte(const unsigned char *mem);
// 将bit位拷贝到dst，返回拷贝的长度。如果dst_size小于mem字节数，只拷贝前面一部分。
int Qdlt698BitStringParse(unsigned char *dst, int dst_size, const unsigned char *mem);



// octet-string
/* octet-string是否带09类型的解释：
出现了两种格式的octet-string，主要区别是否有类型字节09：
1	格式1
09 06 10 00 00 00 22 21 OCTET STRING 6字节
2	格式2
SECURITY-Request中
15 05 02 00 04 20 00 02 00 20 01 02 00 20 04 02 00 00 10 02 00 00
那么，什么是否有类型字节，看表107 SECURITY-Request 数据类型定义
其为“数据类型定义”，类型是明确的，没有类型字节。其它，则有类型字节。
*/
// 示例(有类型字节)： 09 06 10 00 00 00 22 21 OCTET STRING 6字节
// 示例(无类型字节)： 06 10 00 00 00 22 21 OCTET STRING 6字节
// 变长长度字节数
int Qdlt698OctetStringLenSize(const char *mem);
// 内容长度
int Qdlt698OctetStringContentSize(const char *mem);
// len + content长度
int Qdlt698OctetStringLenContentSize(const char *mem);
// type + len + content长度
int Qdlt698OctetStringTypeLenContentSize(const char *memtype);

// visible-string
// 变长长度字节数
int Qdlt698VisibleStringLenSize(const char *mem);
// 内容长度
int Qdlt698VisibleStringContentSize(const char *mem);
// len + content长度
int Qdlt698VisibleStringLenContentSize(const char *mem);
// type + len + content长度
int Qdlt698VisibleStringTypeLenContentSize(const char *memtype);


// rn
// 变长长度字节数
int Qdlt698RnLenSize(const char *mem);
// 内容长度
int Qdlt698RnContentSize(const char *mem);
// len + content长度
int Qdlt698RnLenContentSize(const char *mem);
// type + len + content长度
int Qdlt698RnTypeLenContentSize(const char *memtype);


/*
服务序号及优先标志PIID（Priority and Invoke ID）的数据类型定义见表6 。
PIID 用于客户机 APDU（Client-APDU）的服务数据类型中，其中：
bit7（服务优先级）——0：普通优先级，1：高优先级，在应答 APDU中，其值与请求的 APDU 相同。
bit6（保留）。
bit0…bit5（服务序号）——二进制编码表示 0…63，在应答 APDU 中，其值与请求的 APDU 相同。
*/
#define kP2PiidSize	(1)
typedef struct 
{
	uint8_t service_priority; //bit7（服务优先级）——0：普通优先级，1：高优先级，在应答 APDU中，其值与请求的 APDU 相同。
	uint8_t service_code;		// bit0…bit5（服务序号）——二进制编码表示 0…63，在应答 APDU 中，其值与请求的 APDU 相同。
} P2Piid;
#define kP2PiidDef { 0 , 0 }
void P2PiidPart(P2Piid *dst, uint8_t piid);
uint8_t P2PiidFill(const P2Piid *split);

// PIID-ACD：带请求访问标识的序号及优先标志（Priority and Invoke ID with ACD）
/*
PIID-ACD∷=unsigned 
PIID-ACD 用于服务器 APDU（Server-APDU）的服务数据类型中，其中：
bit7（服务优先级）——见表 6 。
bit6（请求访问 ACD）——0：不请求，1：请求。
bit0…bit5（服务序号）——见表 6 。
*/
#define kP2PiidAcdSize	(1)
typedef struct 
{
	uint8_t service_priority;	// bit7（服务优先级）——0：普通优先级，1：高优先级，在应答 APDU中，其值与请求的APDU相同。
	uint8_t request_acd;		// bit6（请求访问 ACD）——0：不请求，1：请求。
	uint8_t service_code;		// bit0…bit5（服务序号）——二进制编码表示 0…63，在应答 APDU 中，其值与请求的 APDU 相同。
} P2PiidAcd;
#define kP2PiidAcdDef { 0, 0, 0 }
void P2PiidAcdPart(P2PiidAcd *dst, uint8_t piid_acd);
uint8_t P2PiidAcdFill(const P2PiidAcd *split);


// DAR
#define kP2DarSize	(1)
/** @brief 数据访问结果枚举	*/
// DAR: Data_Access_Result
typedef enum 
{
	kP2DarSuccess						= 0,			/**< 成功					*/
	kP2DarHardwareFault					= 1,			/**< 硬件失效				*/
	kP2DarTemporaryFailure				= 2,			/**< 暂时失效				*/
	kP2DarReadWriteDenied				= 3,			/**< 决绝读写				*/
	kP2DarObjectUndefined				= 4,			/**< 对象未定义				*/
	kP2DarObjectClassInconsistent		= 5,			/**< 对象接口类不符合		*/
	kP2DarObjectUnavailable				= 6,			/**< 对象不存在				*/
	kP2DarTypeUnmatched					= 7,			/**< 类型不匹配				*/
	kP2DarScopeOfAccessViolated			= 8,			/**< 越界					*/
	kP2DarDataBlockUnavailable			= 9,			/**< 数据块不可用			*/
	kP2DarLongGetAborted				= 10,			/**< 分帧传输已取消			*/
	kP2DarNoLongGetInProgress			= 11,			/**< 不处于分帧传输状态		*/
	kP2DarLongSetAborted				= 12,			/**< 块写取消				*/
	kP2DarNoLongSetInProgress			= 13,			/**< 不处于块写状态			*/
	kP2DarInvalidBlockno				= 14,			/**< 数据块序号无效			*/
	kP2DarPasswordFailure				= 15,			/**< 密码错/未授权			*/
	kP2DarSpeedNoaccess					= 16,			/**< 通信速率不能更改		*/
	kP2DarTimezoneOver					= 17,			/**< 年时区数超				*/
	kP2DarTimeperiodOver				= 18,			/**< 日时段数超				*/
	kP2DarFeerateOver					= 19,			/**< 费率数超				*/
	kP2DarSecurityUnmatched				= 20,			/**< 安全认证不匹配			*/
	kP2DarRechargeRepeat				= 21,			/**< 重复充值				*/
	kP2DarEsamFailure					= 22,			/**< ESAM验证失败			*/
	kP2DarSecurityFailure				= 23,			/**< 安全认证失败			*/
	kP2DarCustomenoInconsistent			= 24,			/**< 客户编号不匹配			*/
	kP2DarRechargeNumError				= 25,			/**< 充值次数错误			*/
	kP2DarRechargeValueOver				= 26,			/**< 购电超囤积				*/
	kP2DarAddressException				= 27,			/**< 地址异常				*/
	kP2DarSymmetricDecryptionFailure	= 28,			/**< 对称解密错误			*/
	kP2DarAsymmetricDecryptionFailure	= 29,			/**< 非对称解密错误			*/
	kP2DarSignatureFailure				= 30,			/**< 签名错误				*/
	kP2DarMeterSuspend					= 31,			/**< 电能表挂起				*/
	kP2DarTimeflagInvalid				= 32,			/**< 时间标签无效			*/
	kP2DarRequestTimeout				= 33,			/**< 请求超时				*/
	kP2DarEsamP1p2Failure				= 34,			/**< ESAM的P1P2不正确		*/
	kP2DarEsamLcFailure					= 35,			/**< ESAM的LC错误			*/
	kP2DarCompareFailure				= 36,			/**< 比对失败 （36）        */
	kP2DarOtherReason					= 255			/**< 其他					*/
} P2DarT;
cp_t P2DarValid(uint8_t dar);


// test
void Qdlt698Test(void);
#endif
