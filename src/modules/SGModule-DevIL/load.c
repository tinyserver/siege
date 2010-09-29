#include "main.h"
#include "load.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SGuint SG_EXPORT sgmGraphicsLoadFile(char* fname, SGuint* width, SGuint* height, SGuint* bpp, void** data)
{
    if(strcmp(fname, "") == 0)
    {
        *width = 2;
        *height = 2;
        *bpp = 4;
        *data = NULL;

        return SG_OK;
    }

    ILuint img;

    ilGenImages(1, &img);
    ilBindImage(img);

    ilLoadImage(fname);

    *width = ilGetInteger(IL_IMAGE_WIDTH);
    *height = ilGetInteger(IL_IMAGE_HEIGHT);

    *bpp = 32;//ilGetInteger(IL_IMAGE_BPP);
    //format = GL_RGBA;//ilGetInteger(IL_IMAGE_FORMAT);
    //type = GL_UNSIGNED_BYTE;

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    *data = malloc(*width * *height * *bpp / 8);
    *data = memcpy(*data, ilGetData(), *width * *height * *bpp / 8);

    ilDeleteImages(1, &img);

    return SG_OK;
}

/*SGuint SG_EXPORT sgmGraphicsLoadStream(char* fname, SGuint* width, SGuint* height, SGuint* bpp, SGPointer* data)
{
    return 1;
}*/

SGuint sgmGraphicsLoadFreeData(void* data)
{
    free(data);
    return SG_OK;
}
