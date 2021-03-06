/*
 * Copyright (c) 2007 SIEGE Development Team
 * All rights reserved.
 *
 * This file is part of libSIEGE.
 *
 * This software is copyrighted work licensed under the terms of the
 * 2-clause BSD license. Please consult the file "license.txt" for
 * details.
 *
 * If you did not recieve the file with this program, please email
 * Tim Chas <darkuranium@gmail.com>.
 */

#ifndef __SIEGE_MODULES_H__
#define __SIEGE_MODULES_H__
#include "../common.h"
#include "../util/list.h"
#include "../util/link.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define _SG_BIND(name) \
	fptr = sgGetProcAddress(lib, #name);   \
	if(fptr != NULL)    \
		(p##name) = fptr; // not completely valid C99, but blame POSIX for that
        
typedef SGuint SG_EXPORT SGMModuleInitFunction(SGModuleInfo** minfo);
typedef SGuint SG_EXPORT SGMModuleExitFunction(SGModuleInfo* minfo);
typedef SGuint SG_EXPORT SGMModuleTickFunction(SGulong tick);
typedef SGuint SG_EXPORT SGMModuleMatchFunction(SGModuleInfo** minfos, SGuint numinfos, SGbool* ok);

typedef struct SGModule
{
	char* name;
	SGLibrary* lib;
	SGModuleInfo* minfo;
	SGListNode* node;

    SGMModuleInitFunction* sgmModuleInit;
    SGMModuleExitFunction* sgmModuleExit;
    SGMModuleTickFunction* sgmModuleTick;
    SGMModuleMatchFunction* sgmModuleMatch;
} SGModule;

char* SG_EXPORT _sgModuleGetFile(const char* module);

SGModule* SG_EXPORT sgModuleLoad(const char* name);
void SG_EXPORT sgModuleUnload(SGModule* module);

void SG_EXPORT sgModuleSetLoadDirsv(size_t ndirs, va_list args);
void SG_EXPORT sgModuleSetLoadDirs(size_t ndirs, ...);
void SG_EXPORT sgModuleSetLoadDir(const char* dir);
char** SG_EXPORT sgModuleGetLoadDirs(size_t* ndirs);

void SG_EXPORT sgModuleSetLoadPrefixesv(size_t nprefs, va_list args);
void SG_EXPORT sgModuleSetLoadPrefixes(size_t nprefs, ...);
void SG_EXPORT sgModuleSetLoadPrefix(const char* prefix);
char** SG_EXPORT sgModuleGetLoadPrefixes(size_t* nprefs);

SGList* SG_EXPORT sgModuleGetList(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_MODULES_H__
