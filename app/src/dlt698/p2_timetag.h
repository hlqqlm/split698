#ifndef __P2_TIMETAG_H_ 
#define __P2_TIMETAG_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"


/*
TimeTag∷=SEQUENCE
{
 发送时标 date_time_s，
 允许传输延时时间 TI
}
*/
#define P2_TIMETAG_DATETIMES_OFFSET		(0)
#define P2_TIMETAG_DATETIMES_SIZE		(7)

#define P2_TIMETAG_TI_OFFSET			(P2_TIMETAG_DATETIMES_OFFSET + P2_TIMETAG_DATETIMES_SIZE)
#define P2_TIMETAG_TI_SIZE				(DLT698_TI_SIZE)


//{{{ datetimes
//}}}


typedef struct
{
	Dlt698DatetimeS datetimes;
	Dlt698Ti ti;
} P2TimetagValue;
#define kP2TimetagValueDef { kDlt698DatetimeSDef, kDlt698TiDef }


//{{{ part
typedef enum
{
	kP2TimetagCutIxDatetimes,
	kP2TimetagCutIxTi,
	kP2TimetagCutNum				
} P2TimetagCutIxT;

typedef struct
{
	Pcut base;		
	PcutItem items[kP2TimetagCutNum];
} P2TimetagPcut;
#define kP2TimetagPcutDef {		\
	kPcutDef, { kPcutItemDef }		\
}

cp_t P2TimetagPcutOpen(P2TimetagPcut *m);
cp_t P2TimetagPcutClose(P2TimetagPcut *m);
cp_t P2TimetagPcutValid(const P2TimetagPcut *m);
//}}}


// test
cp_t P2ClientApduTest(QTEST_ARG);
#endif

