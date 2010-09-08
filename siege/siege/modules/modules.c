#define SG_BUILD_LIBRARY
#include <siege/modules/modules.h>
#include <siege/modules/audio.h>
#include <siege/modules/window.h>
#include <siege/modules/graphics.h>
#include <siege/modules/input.h>
#include <siege/modules/physics.h>
#include <siege/modules/fonts.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

char* SG_EXPORT _sgModuleGetFile(char* module)
{
    DIR* dir;
    struct dirent* ent;

    char* buf = malloc(strlen("Modules/libSGModule-") + strlen(module) + strlen(".debug.") + 25);

    dir = opendir("Modules");
    if(dir != NULL)
    {
        while((ent = readdir(dir)))
        {
            strcpy(buf, "libSGModule-");
            strcat(buf, module);
            strcat(buf, ".");
            if(strstr(ent->d_name, buf + 3) == ent->d_name) // prefer without "lib"
            {
                strcpy(buf, "Modules/");
                strcat(buf, ent->d_name);
                return buf;
            }
            if(strstr(ent->d_name, buf) == ent->d_name)
            {
                strcpy(buf, "Modules/");
                strcat(buf, ent->d_name);
                return buf;
            }
        }
        closedir(dir);
    }

    free(buf);
    return NULL;
}

SGbool SG_EXPORT _sgModuleInit(void)
{
    _sg_modFirst = SG_TRUE;
    _sg_modList = sgListCreate();
    if(_sg_modList == NULL)
        return SG_FALSE;
    return SG_TRUE;
}
SGbool SG_EXPORT _sgModuleDeinit(void)
{
    ptrdiff_t i;
    for(i = _sg_modList->numitems - 1; i >= 0; i--)
        sgModuleUnload(_sg_modList->items[i]);
    sgListDestroy(_sg_modList);
    return SG_TRUE;
}

SGModule* SG_EXPORT sgModuleLoad(char* name)
{
    char* fname = _sgModuleGetFile(name);
    if(fname == NULL)
    {
        fprintf(stderr, "Warning: Unable to load module %s: Not found\n", name);
        return NULL;
    }

    SGModule* module = malloc(sizeof(SGModule));
    module->name = malloc(strlen(name) + 1);
    strcpy(module->name, name);
    module->lib = sgLibraryLoad(fname);
    if(module->lib == NULL)
        fprintf(stderr, "Warning: Unable to load module %s: Unknown error\n", name);
    free(fname);

    module->sgmModuleInit = sgGetProcAddress(module->lib, "sgmModuleInit");
    module->sgmModuleExit = sgGetProcAddress(module->lib, "sgmModuleExit");
    //module->sgmModuleFree = sgGetProcAddress(module->lib, "sgmModuleFree");
    module->sgmModuleMatch = sgGetProcAddress(module->lib, "sgmModuleMatch");

    _sgModuleLoadAudio(module->lib);
    _sgModuleLoadWindow(module->lib);
    _sgModuleLoadGraphics(module->lib);
    _sgModuleLoadInput(module->lib);
    _sgModuleLoadPhysics(module->lib);
    _sgModuleLoadFonts(module->lib);

    _sg_modFirst = SG_FALSE;

    if(module->sgmModuleInit != NULL)
        module->sgmModuleInit(&module->minfo);
    sgListAppend(_sg_modList, module);
    return module;
}

void SG_EXPORT sgModuleUnload(SGModule* module)
{
    if(module == NULL)
        return;

    //if(module->sgmModuleFree != NULL)
    //    module->sgmModuleFree(module->minfo);
    if(module->sgmModuleExit != NULL)
        module->sgmModuleExit(module->minfo);

    free(module->name);
    sgLibraryUnload(module->lib);
    sgListRemoveItem(_sg_modList, module);
    free(module);
}
