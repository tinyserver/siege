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
/**
 * \file siege/graphics/font.h
 * \brief Font handling.
 * \ingroup Graphics
 */

#ifndef __SIEGE_GRAPHICS_FONT_H__
#define __SIEGE_GRAPHICS_FONT_H__

#include "../common.h"
#include "../util/stream.h"
#include "../util/conv.h"
#include "../util/map.h"
#include "texture.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define SG_ALIGN_CENTER   0x00

#define SG_ALIGN_LEFT     0x01
#define SG_ALIGN_RIGHT    0x02

#define SG_ALIGN_TOP      0x10
#define SG_ALIGN_BASELINE 0x20
#define SG_ALIGN_BOTTOM   0x40

#define SG_ALIGN_HMASK    0x0F
#define SG_ALIGN_VMASK    0xF0

/**
 * \brief Character info.
 * \private
 *
 * This is the character info which holds data such as datafield
 * width, height, character width, height, etc...
 */
typedef struct SGCharInfo
{
	/**
	 * \brief Character's texture.
	 * 
	 * \todo
	 *	Should multiple characters be put into a single texture?
	 */
	SGTexture* texture;

	/**
	 * \name Actual size
	 */
	/// @{
	float width;
	float height;
	/// @}

	/**
	 * \name Bitmap size
	 */
	/// @{
	size_t dwidth;
	size_t dheight;
	/// @}
	
	/**
	 * \name Local offset
	 *
	 * This is not cumulative with other characters - thus,
	 * it does not affect the position of other characters.
	 */
	/// @{
	float xpre;
	float ypre;
	/// @}
	
	/**
	 * \name Global offset
	 *
	 * This \em is (unlike the local offset) cumulative with
	 * other characters and does affect their position.
	 */
	/// @{
	float xpost;
	float ypost;
	/// @}
} SGCharInfo;

/**
 * \brief Font info
 *
 * This buffer holds the info of a font, along
 * with the characters (both preloaded and not).
 */
typedef struct SGFont
{
	/// @{
	/**
	 * \brief Internal handle
	 * \private
	 */
	void* handle;
	float height;	/// < Height of the font
    SGuint dpi;
	/// @}

    float ascent;
    float descent;
    float linegap;

	// char, wchar_t, utf8, utf16
	SGConv* conv[4];

	/**
	 * \name Preloaded characters
	 */
	/// @{
    size_t npreload;
	SGuint numchars;	/// < The number of characters
	SGCharInfo* chars;	/// < The characters themselves
	/// @}

	/**
	 * \name On-demand loaded characters
	 */
	/// @{
	SGuint numcache;		/// < Length of the arrays
	SGdchar* cachechars;	/// < The character UTF-32 values (keys)
	SGCharInfo* cache;		/// < Character infos (values)
    SGMap* cmap;          ///< SGdchar->SGCharInfo mapping
	/// @}

    SGStream* stream;
    SGbool del;
} SGFont;

SGCharInfo* SG_EXPORT _sgFontFindCache(SGFont* font, SGdchar c);
SGbool SG_EXPORT _sgFontGetChars(SGFont* font, SGdchar* str, SGuint strlen, SGCharInfo* info);
void SG_EXPORT _sgFontToLoad(SGFont* font, SGdchar* input, SGuint inlen, SGdchar* output, SGuint* outlen);
SGbool SG_EXPORT _sgFontLoad(SGFont* font, SGdchar* chars, SGuint numchars, SGbool force);
SGubyte* SG_EXPORT _sgFontToRGBA(SGFont* font, SGubyte* data, SGuint datalen);

void SG_EXPORT _sgFontCenterOffsetU32(SGFont* font, float* x, float* y, const SGdchar* text);

SGdchar* SG_EXPORT _sgFontU16ToU32(SGFont* font, const SGwchar* text);
SGdchar* SG_EXPORT _sgFontU8ToU32(SGFont* font, const SGchar* text);
SGdchar* SG_EXPORT _sgFontWToU32(SGFont* font, const wchar_t* text);
SGdchar* SG_EXPORT _sgFontToU32(SGFont* font, const char* text);

SGFont* SG_EXPORT sgFontCreateStream(SGStream* stream, SGbool delstream, float height, SGuint dpi, SGuint preload);
/// @{
/**
 * \brief Load a font
 * 
 * \param fname Filename of the font to load
 * \param height Height of the font (in pt)
 * \param preload Number of characters to preload; good values are 127 or 255.
 * 
 * \return The newly created font info if successful, NULL otherwise.
 */
SGFont* SG_EXPORT sgFontCreate(const char* fname, float height, SGuint preload);
/**
 * \brief Destroy a font info
 * 
 * \param font The font info to destroy.
 * It should not be used anymore after this call.
 */
void SG_EXPORT sgFontDestroy(SGFont* font);
/// @}

void SG_EXPORT sgFontClearCache(SGFont* font);
void SG_EXPORT sgFontSetHeight(SGFont* font, float height, SGuint dpi);

/**
 * \name Printing
 *
 * \param font The font face to use for printing.
 * \param x X position (left)
 * \param y Y position (baseline)
 *
 * These functions print text in a position <em>x</em>,<em>y</em>,
 * where \a x is the start, leftmost, position of the printed text
 * and \a y is the baseline.
 */
/// @{
void SG_EXPORT sgFontPrintfW(SGFont* font, float x, float y, const wchar_t* format, ...);
void SG_EXPORT sgFontPrintfvW(SGFont* font, float x, float y, const wchar_t* format, va_list args);
/**
 * \brief printf-style print
 */
void SG_EXPORT SG_HINT_PRINTF(4, 5) sgFontPrintf(SGFont* font, float x, float y, const char* format, ...);
/**
 * \brief vprintf-style print
 */
void SG_EXPORT SG_HINT_PRINTF(4, 0) sgFontPrintfv(SGFont* font, float x, float y, const char* format, va_list args);
/**
 * \brief Text-based print
 */
void SG_EXPORT sgFontPrintU32(SGFont* font, float x, float y, const SGdchar* text);
void SG_EXPORT sgFontPrintU16(SGFont* font, float x, float y, const SGwchar* text);
void SG_EXPORT sgFontPrintU8(SGFont* font, float x, float y, const SGchar* text);
void SG_EXPORT sgFontPrintW(SGFont* font, float x, float y, const wchar_t* text);
void SG_EXPORT sgFontPrint(SGFont* font, float x, float y, const char* text);
/// @}

void SG_EXPORT sgFontPrintAlignedfW(SGFont* font, float x, float y, SGenum align, const wchar_t* format, ...);
void SG_EXPORT sgFontPrintAlignedfvW(SGFont* font, float x, float y, SGenum align, const wchar_t* format, va_list args);

void SG_EXPORT SG_HINT_PRINTF(5, 6) sgFontPrintAlignedf(SGFont* font, float x, float y, SGenum align, const char* format, ...);
void SG_EXPORT SG_HINT_PRINTF(5, 0) sgFontPrintAlignedfv(SGFont* font, float x, float y, SGenum align, const char* format, va_list args);

void SG_EXPORT sgFontPrintAlignedU32(SGFont* font, float x, float y, SGenum align, const SGdchar* text);
void SG_EXPORT sgFontPrintAlignedU16(SGFont* font, float x, float y, SGenum align, const SGwchar* text);
void SG_EXPORT sgFontPrintAlignedU8(SGFont* font, float x, float y, SGenum align, const SGchar* text);
void SG_EXPORT sgFontPrintAlignedW(SGFont* font, float x, float y, SGenum align, const wchar_t* text);
void SG_EXPORT sgFontPrintAligned(SGFont* font, float x, float y, SGenum align, const char* text);

/**
 * \name Get size of printed text
 *
 * \param font The font face to use for printing.
 * \param[out] x Width
 * \param[out] y Height
 *
 * These functions return the size of the text, had it been printed,
 * in <em>x</em>,<em>y</em>, with the former being the width and the
 * latter height.
 */
/// @{
void SG_EXPORT sgFontStrSizefW(SGFont* font, float* x, float* y, const wchar_t* format, ...);
void SG_EXPORT sgFontStrSizefvW(SGFont* font, float* x, float* y, const wchar_t* format, va_list args);
/**
 * \brief printf-style text
 */
void SG_EXPORT SG_HINT_PRINTF(4, 5) sgFontStrSizef(SGFont* font, float* x, float* y, const char* format, ...);
/**
 * \brief vprintf-style text
 */
void SG_EXPORT SG_HINT_PRINTF(4, 0) sgFontStrSizefv(SGFont* font, float* x, float* y, const char* format, va_list args);
/**
 * \brief plain text
 */
void SG_EXPORT sgFontStrSizeU32(SGFont* font, float* x, float* y, const SGdchar* text);
void SG_EXPORT sgFontStrSizeU16(SGFont* font, float* x, float* y, const SGwchar* text);
void SG_EXPORT sgFontStrSizeU8(SGFont* font, float* x, float* y, const SGchar* text);
void SG_EXPORT sgFontStrSizeW(SGFont* font, float* x, float* y, const wchar_t* text);
void SG_EXPORT sgFontStrSize(SGFont* font, float* x, float* y, const char* text);
/// @}

// need a better name for FindIndex and GetPos...
size_t SG_EXPORT sgFontFindIndexfW(SGFont* font, float x, float y, const wchar_t* format, ...);
size_t SG_EXPORT sgFontFindIndexfvW(SGFont* font, float x, float y, const wchar_t* format, va_list args);

size_t SG_EXPORT SG_HINT_PRINTF(4, 5) sgFontFindIndexf(SGFont* font, float x, float y, const char* format, ...);
size_t SG_EXPORT SG_HINT_PRINTF(4, 0) sgFontFindIndexfv(SGFont* font, float x, float y, const char* format, va_list args);

size_t SG_EXPORT sgFontFindIndexU32(SGFont* font, float x, float y, const SGdchar* text);
size_t SG_EXPORT sgFontFindIndexU16(SGFont* font, float x, float y, const SGwchar* text);
size_t SG_EXPORT sgFontFindIndexU8(SGFont* font, float x, float y, const SGchar* text);
size_t SG_EXPORT sgFontFindIndexW(SGFont* font, float x, float y, const wchar_t* text);
size_t SG_EXPORT sgFontFindIndex(SGFont* font, float x, float y, const char* text);

void SG_EXPORT sgFontGetPosfW(SGFont* font, float* x, float* y, size_t index, const wchar_t* format, ...);
void SG_EXPORT sgFontGetPosfvW(SGFont* font, float* x, float* y, size_t index, const wchar_t* format, va_list args);

void SG_EXPORT SG_HINT_PRINTF(5, 6) sgFontGetPosf(SGFont* font, float* x, float* y, size_t index, const char* format, ...);
void SG_EXPORT SG_HINT_PRINTF(5, 0) sgFontGetPosfv(SGFont* font, float* x, float* y, size_t index, const char* format, va_list args);

void SG_EXPORT sgFontGetPosU32(SGFont* font, float* x, float* y, size_t index, const SGdchar* text);
void SG_EXPORT sgFontGetPosU16(SGFont* font, float* x, float* y, size_t index, const SGwchar* text);
void SG_EXPORT sgFontGetPosU8(SGFont* font, float* x, float* y, size_t index, const SGchar* text);
void SG_EXPORT sgFontGetPosW(SGFont* font, float* x, float* y, size_t index, const wchar_t* text);
void SG_EXPORT sgFontGetPos(SGFont* font, float* x, float* y, size_t index, const char* text);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __SIEGE_GRAPHICS_FONT_H__
