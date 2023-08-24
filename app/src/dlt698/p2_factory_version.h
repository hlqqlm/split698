#ifndef __P2_FACTORY_VERSION_H_
#define __P2_FACTORY_VERSION_H_


#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "qdlt698.h"
#include "p2_visible_string.h"




/*
FactoryVersion∷=SEQUENCE
{
厂商代码 visible-string(SIZE(4))，
软件版本号 visible-string(SIZE(4))，
软件版本日期 visible-string(SIZE(6))，
硬件版本号 visible-string(SIZE(4))，
硬件版本日期 visible-string(SIZE(6))，
厂商扩展信息 visible-string(SIZE(8))
}
*/
#define kP2FactoryVersionName				"factory_version"


// basic information
#define kP2FactoryVersionFactoryIdOffset		(0)
#define kP2FactoryVersionFactoryIdSize			(1 + 4)

#define kP2FactoryVersionSoftwareVersionOffset	(kP2FactoryVersionFactoryIdOffset + kP2FactoryVersionFactoryIdSize)
#define kP2FactoryVersionSoftwareVersionSize	(1 + 4)

#define kP2FactoryVersionSoftwareDateOffset		(kP2FactoryVersionSoftwareVersionOffset + kP2FactoryVersionSoftwareVersionSize)
#define kP2FactoryVersionSoftwareDateSize		(1 + 6)

#define kP2FactoryVersionHardwareVersionOffset	(kP2FactoryVersionSoftwareDateOffset + kP2FactoryVersionSoftwareDateSize)
#define kP2FactoryVersionHardwareVersionSize	(1 + 4)

#define kP2FactoryVersionHardwareDateOffset		(kP2FactoryVersionHardwareVersionOffset + kP2FactoryVersionHardwareVersionSize)
#define kP2FactoryVersionHardwareDateSize		(1 + 6)

#define kP2FactoryVersionFactoryExtendOffset	(kP2FactoryVersionHardwareDateOffset + kP2FactoryVersionHardwareDateSize)
#define kP2FactoryVersionFactoryExtendSize		(1 + 8)


#define kP2FactoryVersionSize					(kP2FactoryVersionFactoryExtendOffset + kP2FactoryVersionFactoryExtendSize)




//{{{ cut
typedef enum
{
	kP2FactoryVersionCutIxFactoryId,
	kP2FactoryVersionCutIxSoftwareVersion,
	kP2FactoryVersionCutIxSoftwareDate,
	kP2FactoryVersionCutIxHardwareVersion,
	kP2FactoryVersionCutIxHardwareDate,
	kP2FactoryVersionCutIxFactoryExtend,
	kP2FactoryVersionCutNum				
} P2FactoryVersionCutIxT;


typedef struct 
{
	Pcut base;
	PcutItem items[kP2FactoryVersionCutNum];

	P2VisibleStringPcut factory_id;
	P2VisibleStringPcut software_version;
	P2VisibleStringPcut software_date;
	P2VisibleStringPcut hardware_version;
	P2VisibleStringPcut hardware_date;
	P2VisibleStringPcut factory_extend;
} P2FactoryVersionPcut;
#define kP2FactoryVersionPcutDef {				\
	kPcutDef, { kPcutItemDef },		\
	kP2VisibleStringPcutDef, \
	kP2VisibleStringPcutDef, \
	kP2VisibleStringPcutDef, \
	kP2VisibleStringPcutDef, \
	kP2VisibleStringPcutDef, \
	kP2VisibleStringPcutDef, \
}

cp_t P2FactoryVersionPcutOpen(P2FactoryVersionPcut *m);
cp_t P2FactoryVersionPcutClose(P2FactoryVersionPcut *m);
cp_t P2FactoryVersionPcutValid(const P2FactoryVersionPcut *m);

cp_t P2FactoryVersionPcutOpenBase(Pcut *base);
cp_t P2FactoryVersionPcutCloseBase(Pcut *base);
//}}}


// test
cp_t P2FactoryVersionTest(QTEST_ARG);
#endif

