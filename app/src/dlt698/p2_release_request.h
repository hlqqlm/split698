#ifndef __P2_RELEASE_REQUEST_H_ 
#define __P2_RELEASE_REQUEST_H_ 

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "p2_choice.h"
#include "qdlt698.h"


//{{{ base
#define kP2ReleaseRequestName			"connect_request"

/*
RELEASE-Request∷=SEQUENCE
{
服务序号-优先级 PIID
}
*/
#define kP2ReleaseRequestPiidOffset					(0)
#define kP2ReleaseRequestPiidSize					(kP2PiidSize)
//}}}


//{{{ cut
typedef enum
{
	kP2ReleaseRequestCutIxPiid,
	kP2ReleaseRequestCutNum				
} P2ReleaseRequestCutIxT;

typedef struct 
{
	Pcut base;
	PcutItem items[kP2ReleaseRequestCutNum];
} P2ReleaseRequestPcut;
#define kP2ReleaseRequestPcutDef { kPcutDef, { kPcutItemDef } }

cp_t P2ReleaseRequestPcutOpen(P2ReleaseRequestPcut *m);
cp_t P2ReleaseRequestPcutClose(P2ReleaseRequestPcut *m);
cp_t P2ReleaseRequestPcutValid(const P2ReleaseRequestPcut *m);

cp_t P2ReleaseRequestPcutOpenBase(Pcut *base);
cp_t P2ReleaseRequestPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2ReleaseRequestTest(QTEST_ARG);
#endif

