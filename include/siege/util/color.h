#ifndef __SIEGE_UTIL_COLOR_H__
#define __SIEGE_UTIL_COLOR_H__

#include "../common.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct SGColor
{
 float r, g, b, a;
} SGColor;

#ifdef SG_BUILD_LIBRARY
size_t _sg_colNum;
SGubyte* _sg_colFlags;
char** _sg_colNames;
SGColor* _sg_colValues;
#endif // SG_BUILD_LIBRARY

SGbool SG_EXPORT _sgColorInit(void);
SGbool SG_EXPORT _sgColorDeinit(void);

void SG_EXPORT _sgColorPreprocess(char* buf, size_t buflen, char* name);
SGColor SG_EXPORT _sgColorValue(char* name, SGubyte flag);

SGColor SG_EXPORT sgColor4f(float r, float g, float b, float a);
SGColor SG_EXPORT sgColor3f(float r, float g, float b);
SGColor SG_EXPORT sgColor2f(float g, float a);
SGColor SG_EXPORT sgColor1f(float g);
SGColor SG_EXPORT sgColor4fv(float* f);
SGColor SG_EXPORT sgColor3fv(float* f);
SGColor SG_EXPORT sgColor2fv(float* f);
SGColor SG_EXPORT sgColor1fv(float* f);
SGColor SG_EXPORT sgColor4ub(SGubyte r, SGubyte g, SGubyte b, SGubyte a);
SGColor SG_EXPORT sgColor3ub(SGubyte r, SGubyte g, SGubyte b);
SGColor SG_EXPORT sgColor2ub(SGubyte g, SGubyte a);
SGColor SG_EXPORT sgColor1ub(SGubyte g);
SGColor SG_EXPORT sgColor4ubv(SGubyte* ub);
SGColor SG_EXPORT sgColor3ubv(SGubyte* ub);
SGColor SG_EXPORT sgColor2ubv(SGubyte* ub);
SGColor SG_EXPORT sgColor1ubv(SGubyte* ub);
SGColor SG_EXPORT sgColorRGBA(SGuint color);
SGColor SG_EXPORT sgColorBGRA(SGuint color);
SGColor SG_EXPORT sgColorARGB(SGuint color);
SGColor SG_EXPORT sgColorABGR(SGuint color);
SGColor SG_EXPORT sgColorRGB(SGuint color);
SGColor SG_EXPORT sgColorBGR(SGuint color);
SGColor SG_EXPORT sgColorX11(char* name);
SGColor SG_EXPORT sgColorWeb(char* name);
SGColor SG_EXPORT sgColorNan(void);

SGbool SG_EXPORT sgColorIsNan(SGColor c);

SGColor SG_EXPORT sgColorMix(SGColor a, SGColor b, float t);
SGColor SG_EXPORT sgColorAdd1c(SGColor a, SGColor b);
SGColor SG_EXPORT sgColorSub1c(SGColor a, SGColor b);
SGColor SG_EXPORT sgColorMul1c(SGColor a, SGColor b);
SGColor SG_EXPORT sgColorDiv1c(SGColor a, SGColor b);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_UTIL_COLOR_H__