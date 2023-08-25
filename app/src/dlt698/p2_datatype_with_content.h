#ifndef __P2_DATATYPE_WITH_CONTENT_H_
#define __P2_DATATYPE_WITH_CONTENT_H_

#include "qos/qcp.h"
#include "qos/qtest.h"
#include "protocol/pcut.h"
#include "protocol/pfill.h"
#include "qdlt698.h"


// name
#define kP2DatatypeWithContentName				"datatype_with_content"
#define kP2DatatypeWithContentNameDatatype		"datatype"
#define kP2DatatypeWithContentNameContent		"content"

// offset-size
#define kP2DatatypeWithContentDatatypeOffset		(0)
#define kP2DatatypeWithContentDatatypeSize			(kDlt698DatatypeSize)

#define kP2DatatypeWithContentContentOffset			(kP2DatatypeWithContentDatatypeOffset + kP2DatatypeWithContentDatatypeSize)
#define kP2DatatypeWithContentContentSize(_content_size)	(_content_size)

#define kP2DatatypeWithContentWholeSize(_content_size)	(kP2DatatypeWithContentContentOffset + kP2DatatypeWithContentContentSize(_content_size))

// datatype
uint8_t P2DatatypeWithContentDatatype(const char *whole);

// content
const char *P2DatatypeWithContentContentMem(const char *whole);


//{{{ cut
// cut_ix
// 各部分索引号
typedef enum
{
	kP2DatatypeWithContentCutIxDatatype,
	kP2DatatypeWithContentCutIxContent,
	kP2DatatypeWithContentCutNum				
} P2DatatypeWithContentCutIxT;


// pcut
typedef struct
{
	Pcut base;
	PcutItem items[kP2DatatypeWithContentCutNum];
	uint16_t datatype;
} P2DatatypeWithContentPcut;
#define kP2DatatypeWithContentPcutDef { kPcutDef, { kPcutItemDef }, kDlt698DatatypeInvalid }

cp_t P2DatatypeWithContentPcutOpen(P2DatatypeWithContentPcut *m);
//cp_t P2DatatypeWithContentPcutOpen(P2DatatypeWithContentPcut *m, uint8_t datatype, Pcut *content, const char *content_name);
cp_t P2DatatypeWithContentPcutClose(P2DatatypeWithContentPcut *m);
cp_t P2DatatypeWithContentPcutValid(const P2DatatypeWithContentPcut *m);

// 配置content部分
cp_t P2DatatypeWithContentPcutSetContent(P2DatatypeWithContentPcut *m, uint16_t datatype, Pcut *content, const char *content_name);

cp_t P2DatatypeWithContentPcutOpenBase(Pcut *base);
cp_t P2DatatypeWithContentPcutCloseBase(Pcut *base);

//}}}


// pfill
//cp_t P2DatatypeWithContentFillInit(Pfill *m, bool datatype_exist, uint16_t datatype, int content_size, const void *content_def);
cp_t P2DatatypeWithContentFillInit(Pfill *m, bool datatype_exist, uint16_t datatype);

// test
cp_t P2DatatypeWithContentTest(QTEST_ARG);
#endif

