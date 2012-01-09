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

#define SG_BUILD_LIBRARY
#include <siege/util/string.h>
#include <siege/util/thread.h>
#include <siege/util/mutex.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// MinGW thinks it's smart and uses its own variant of vswprintf...
#define __STRICT_ANSI__
#include <stdio.h>
#include <wchar.h>

int vswprintf(wchar_t *wcs, size_t maxlen, const wchar_t *format, va_list args);

char* _sgStringAppend(char** str, size_t* len, size_t* mem, const char* what)
{
    size_t wlen = strlen(what);
    if(*len + wlen >= *mem)
    {
        if(*mem == 0)
            *mem = 32;
        else
            *mem <<= 1;
        *str = realloc(*str, *mem);
    }
    memcpy(*str + *len, what, wlen);
    *len += wlen;
    (*str)[*len] = 0;
    return *str;
}

static SGMutex* _sg_strMutex;

static void SG_EXPORT _sgStringDeinit(void)
{
    sgMutexDestroy(_sg_strMutex);
}
/*
 * Will have to solve the race condition regarding two sgPrintf's
 * getting called for the first time, at the same time...
 * 
 * Maybe a global mutex for just this purpose?
 */
static void SG_EXPORT _sgStringInit(void)
{
    sgThreadAtExit(_sgStringDeinit);
    _sg_strMutex = sgMutexCreate();
}

size_t SG_EXPORT sgPrintfW(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    size_t ret = sgPrintfvW(format, args);
    va_end(args);
    return ret;
}
size_t SG_EXPORT sgPrintfvW(const wchar_t* format, va_list args)
{
    if(!_sg_strMutex)
        _sgStringInit();
    sgMutexLock(_sg_strMutex);
    size_t ret = vwprintf(format, args);
    sgMutexUnlock(_sg_strMutex);
    return ret;
}
size_t SG_EXPORT SG_HINT_PRINTF(1, 2) sgPrintf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    size_t ret = sgPrintfv(format, args);
    va_end(args);
    return ret;
}
size_t SG_EXPORT SG_HINT_PRINTF(1, 0) sgPrintfv(const char* format, va_list args)
{
    if(!_sg_strMutex)
        _sgStringInit();
    sgMutexLock(_sg_strMutex);
    size_t ret = vprintf(format, args);
    sgMutexUnlock(_sg_strMutex);
    return ret;
}

size_t SG_EXPORT sgSPrintfW(wchar_t* buf, size_t buflen, const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    size_t size = sgSPrintfvW(buf, buflen, format, args);
    va_end(args);
    return size;
}
size_t SG_EXPORT sgSPrintfvW(wchar_t* buf, size_t buflen, const wchar_t* format, va_list args)
{
    int ret = vswprintf(buf, buflen, format, args);
    if(ret < 0)
        return 0;
    return ret;
}

wchar_t* SG_EXPORT sgAPrintfW(const wchar_t* format, ...)
{
    va_list args;
    va_start(args, format);
    wchar_t* str = sgAPrintfvW(format, args);
    va_end(args);
    return str;
}
wchar_t* SG_EXPORT sgAPrintfvW(const wchar_t* format, va_list args)
{
    wchar_t buf;
    
    va_list argcpy;
    va_copy(argcpy, args);
    size_t len = sgSPrintfvW(&buf, 1, format, argcpy);
    va_end(argcpy);
    
    wchar_t* str = malloc((len + 1) * sizeof(wchar_t));
    if(!str)
        return NULL;
    sgSPrintfW(str, len + 1, format, args);
    
    return str;
}

size_t SG_EXPORT SG_HINT_PRINTF(3, 4) sgSPrintf(char* buf, size_t buflen, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	size_t size = sgSPrintfv(buf, buflen, format, args);
	va_end(args);
	return size;
}
size_t SG_EXPORT SG_HINT_PRINTF(3, 0) sgSPrintfv(char* buf, size_t buflen, const char* format, va_list args)
{
    int ret = vsnprintf(buf, buflen, format, args);
    if(ret < 0)
        return 0;
    return ret;
}

char* SG_EXPORT SG_HINT_PRINTF(1, 2) sgAPrintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char* str = sgAPrintfv(format, args);
	va_end(args);
	return str;
}
char* SG_EXPORT SG_HINT_PRINTF(1, 0) sgAPrintfv(const char* format, va_list args)
{
    char buf;
    
    va_list argcpy;
    va_copy(argcpy, args);
    size_t len = sgSPrintfv(&buf, 1, format, argcpy);
    va_end(argcpy);
    
    char* str = malloc(len + 1);
    if(!str)
        return NULL;
    sgSPrintfv(str, len + 1, format, args);
    
    return str;
}

void SG_EXPORT sgAPrintFree(void* str)
{
    free(str);
}

SGbool SG_EXPORT sgStartsWith(const char* text, const char* what)
{
    return !strncmp(text, what, strlen(what));
}
char* SG_EXPORT sgSpaceEnd(const char* text)
{
    if(!text)
        return NULL;
    
    while(isspace(*text))
        text++;
    return (char*)text;
}
char* SG_EXPORT sgLineEnd(const char* text)
{
	if(text == NULL)
		return NULL;
    return (char*)text + strcspn(text, "\r\n");
}
SGuint SG_EXPORT sgLineLength(const char* text)
{
	return sgLineEnd(text) - text;
}
char* SG_EXPORT sgNextLine(const char* text)
{
	if(text == NULL)
		return NULL;

	char* brk = strpbrk(text, "\r\n");
    if(!brk)
		return NULL;
	if((brk[0] == '\r') && (brk[1] == '\n'))
		return brk + 2;
	return brk + 1;
}
SGuint SG_EXPORT sgNumLines(const char* text)
{
	SGuint numlines = 0;

	const char* ptr = text;
	while(ptr != NULL)
	{
		ptr = sgNextLine(ptr);
		numlines++;
	}
	return numlines;
}

/*size_t SG_EXPORT sgStrcspnU32(const SGdchar* text, const SGdchar* sel)
{
    size_t i, j;
    for(i = 0; text[i]; i++)
        for(j = 0; sel[j]; j++)
            if(text[i] == sel[j])
                return i;
    return i;
}*/
SGdchar* SG_EXPORT sgLineEndU32(const SGdchar* text)
{
	if(text == NULL)
		return NULL;

	while(*text != '\r' && *text != '\n' && *text != '\0')
		text++;

	return (SGdchar*)text;
}
SGuint SG_EXPORT sgLineLengthU32(const SGdchar* text)
{
	return sgLineEndU32(text) - text;
}
SGdchar* SG_EXPORT sgNextLineU32(const SGdchar* text)
{
	if(text == NULL)
		return NULL;

	SGdchar* end = sgLineEndU32(text);
	if(end[0] == 0)
		return NULL;

	if(end[0] == '\r' && end[1] == '\n')
		return end + 2;
	return end + 1;
}
SGuint SG_EXPORT sgNumLinesU32(const SGdchar* text)
{
	SGuint numlines = 0;

	const SGdchar* ptr = text;
	while(ptr != NULL)
	{
		ptr = sgNextLineU32(ptr);
		numlines++;
	}
	return numlines;
}
