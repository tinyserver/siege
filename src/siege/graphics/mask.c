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
#include <siege/graphics/mask.h>
#include <siege/modules/graphics.h>
// for DrawDBG && _sg_drawCurColor
#include <siege/graphics/draw.h>

#include <stdlib.h>
#include <math.h>
// for fprintf
#include <stdio.h>

SGMask* SG_EXPORT sgMaskCreateSprite(SGSprite* sprite)
{
	return sgMaskCreateTexture2i(sprite->subimages[0], sprite->xoffset, sprite->yoffset);
}
SGMask* SG_EXPORT sgMaskCreateTexture2i(SGTexture* texture, SGint xoffset, SGint yoffset)
{
	SGMask* mask = malloc(sizeof(SGMask));
	if(mask == NULL)
		return NULL;

	SGuint i, j;

	mask->width = sgTextureGetWidth(texture);
	mask->height = sgTextureGetHeight(texture);

	mask->xoffset = xoffset;
	mask->yoffset = yoffset;

	mask->precise = SG_TRUE;

	mask->field = malloc(mask->width * sizeof(SGbool*));
	for(i = 0; i < mask->width; i++)
		mask->field[i] = calloc(mask->height, sizeof(SGbool));

	SGuint awidth = 0;
	SGuint aheight = 0;
	SGuint bpp = 0;
	char* data = NULL;
	if(_sg_modGraphics.sgmGraphicsTextureGetData != NULL)
		_sg_modGraphics.sgmGraphicsTextureGetData(texture->handle, &awidth, &aheight, &bpp, (void**)&data);

	SGubyte bypp = 0;
	SGubyte r, g, b, a;
	SGubyte rbits = 0;
	SGubyte gbits = 0;
	SGubyte bbits = 0;
	SGubyte abits = 0;

	switch(bpp)
	{
		case 32:
			rbits = 8;
			gbits = 8;
			bbits = 8;
			abits = 8;
			bypp = 4;
			break;
		case 24:
			rbits = 8;
			gbits = 8;
			bbits = 8;
			bypp = 3;
			break;
		case 16:
			rbits = 5;
			gbits = 6;
			bbits = 5;
			bypp = 2;
			break;
		case 15:
			rbits = 5;
			gbits = 5;
			bbits = 5;
			abits = 1; // ?
			bypp = 2;
			break;
		case 8:
			abits = 8;
			bypp = 1;
			break;

		default:
			fprintf(stderr, "Unsupported BPP '%d'\n", bpp);
	}

	r = (SGubyte)(_sg_drawCurColor[0] * ((1 << rbits) - 1));
	g = (SGubyte)(_sg_drawCurColor[1] * ((1 << gbits) - 1));
	b = (SGubyte)(_sg_drawCurColor[2] * ((1 << bbits) - 1));
	a = (SGubyte)(_sg_drawCurColor[3] * ((1 << abits) - 1));

	SGuint ui = a
			|	(b << abits)
			|	(g << (abits + bbits))
			|	(r << (abits + bbits + gbits));

	for(i = 0; i < mask->width; i++)
	{
		for(j = 0; j < mask->height; j++)
		{
			switch(bpp)
			{
				case 32:
					mask->field[i][j] = data[(j * awidth + i) * 4 + 3] ? SG_TRUE : SG_FALSE;
					break;
				case 24:
					mask->field[i][j] = (data[(j * awidth + i) * 3 + 0] == r)
									&&	(data[(j * awidth + i) * 3 + 1] == g)
									&&	(data[(j * awidth + i) * 3 + 2] == b);
					break;
				case 16:
				case 15:
					mask->field[i][j] = *(SGshort*)&data[(j * awidth + i) * 2] == ui;
					break;
				case 8:
					mask->field[i][j] = data[(j * awidth + i)] == a;
					break;
			}
		}
	}

	_sg_modGraphics.sgmGraphicsTextureFreeData(data);

	return mask;
}
SGMask* SG_EXPORT sgMaskCreateTexture(SGTexture* texture)
{
	SGMask* mask = sgMaskCreateTexture2i(texture, 0, 0);
	if(mask == NULL)
		return NULL;
	mask->xoffset = mask->width / 2;
	mask->yoffset = mask->height / 2;
	return mask;
}
SGMask* SG_EXPORT sgMaskCreateFile2i(const char* fname, SGint xoffset, SGint yoffset)
{
	SGTexture* texture = sgTextureCreateFile(fname);
	if(texture == NULL)
		return NULL;

	SGMask* mask = sgMaskCreateTexture2i(texture, xoffset, yoffset);
	sgTextureDestroy(texture);
	
	return mask;
}
SGMask* SG_EXPORT sgMaskCreateFile(const char* fname)
{
	SGMask* mask = sgMaskCreateFile2i(fname, 0, 0);
	if(mask == NULL)
		return NULL;
	mask->xoffset = mask->width / 2;
	mask->yoffset = mask->height / 2;
	return mask;
}
void SG_EXPORT sgMaskDestroy(SGMask* mask)
{
	if(mask == NULL)
		return;

	SGuint i;
	for(i = 0; i < mask->width; i++)
		free(mask->field[i]);
	free(mask->field);
	free(mask);
}

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(x) (((x) < 0) ? -(x) : (x))
SGbool SG_EXPORT sgMaskCheckCollision(SGMask* m1, SGint x1, SGint y1, SGMask* m2, SGint x2, SGint y2)
{
	x1 -= m1->xoffset;
	y1 -= m1->yoffset;
	x2 -= m2->xoffset;
	y2 -= m2->yoffset;

	if(x2 > x1)
	{
		x2 -= x1;
		x1 = 0;
	}
	else
	{
		x1 -= x2;
		x2 = 0;
	}
	if(y2 > y1)
	{
		y2 -= y1;
		y1 = 0;
	}
	else
	{
		y1 -= y2;
		y2 = 0;
	}

	ptrdiff_t i, j;	
	for(i = 0; i < MAX(m1->width, m2->width); i++)
		if((0 <= i - x1) && (i - x1 < m1->width)
		&& (0 <= i - x2) && (i - x2 < m2->width))
			for(j = 0; j < MAX(m1->height, m2->height); j++)
				if((0 <= j - y1) && (j - y1 < m1->height)
				&& (0 <= j - y2) && (j - y2 < m2->height))
					if((!m1->precise || m1->field[i - x1][j - y1])
					&& (!m2->precise || m2->field[i - x2][j - y2]))
						return SG_TRUE;
	return SG_FALSE;
}

void SG_EXPORT sgMaskGetSize(SGMask* mask, SGuint* width, SGuint* height)
{
	if(mask == NULL)
		return;

	if(width != NULL)
		*width = mask->width;
	if(height != NULL)
		*height = mask->height;
}
SGuint SG_EXPORT sgMaskGetWidth(SGMask* mask)
{
	if(mask == NULL)
		return 0;
	return mask->width;
}
SGuint SG_EXPORT sgMaskGetHeight(SGMask* mask)
{
	if(mask == NULL)
		return 0;
	return mask->height;
}

void SG_EXPORT sgMaskDrawDBG(SGMask* mask, SGint x, SGint y, SGbool transparent)
{
	if(mask == NULL)
		return;
	SGuint i, j;

	sgDrawBegin(SG_GRAPHICS_PRIMITIVE_POINTS);
		for(i = 0; i < mask->width; i++)
		{
			for(j = 0; j < mask->height; j++)
			{
				if(!transparent)
					sgDrawColor1f(mask->field[i][j] ? 1.0 : 0.0);
				if(mask->field[i][j] || !transparent)
					sgDrawVertex2f(x - mask->xoffset + i, y - mask->yoffset + j);
			}
		}
	sgDrawEnd();
}
