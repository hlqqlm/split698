#ifndef __P2_FOLLOW_REPORT_H_ 
#define __P2_FOLLOW_REPORT_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"


/*
{
 若干个对象属性及数据 [1] SEQUENCE OF A-ResultNormal，
若干个记录型对象属性及数据 [2] SEQUENCE OF A-ResultRecord
}
*/
// todo
#define kP2FollowReportXXXoffset		(0)
#define kP2FollowReportXXXSize			(1)



//{{{ datetimes
//}}}


//{{{ cut
typedef enum
{
	kP2FollowReportPartIxDatetimes,
	kP2FollowReportPartNum				
} P2FollowReportPartIxT;

typedef struct
{
	Pcut base;		
	PcutItem items[kP2FollowReportPartNum];
} P2FollowReportPcut;
#define kP2FollowReportPcutDef {		\
	kPcutDef, { kPcutItemDef }		\
}

cp_t P2FollowReportPcutOpen(P2FollowReportPcut *m);
cp_t P2FollowReportPcutClose(P2FollowReportPcut *m);
cp_t P2FollowReportPcutValid(const P2FollowReportPcut *m);
//}}}


// test
cp_t P2FollowReportTest(QTEST_ARG);
#endif

