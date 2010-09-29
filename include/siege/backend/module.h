#ifndef __SIEGE_BACKEND_MODULE_H__
#define __SIEGE_BACKEND_MODULE_H__

#include "../common.h"

#ifdef __cplusplus
extern "C"
{
#endif
    SGuint SG_EXPORT sgmModuleInit(SGModuleInfo** minfo);
    SGuint SG_EXPORT sgmModuleExit(SGModuleInfo* minfo);
    //SGuint SG_EXPORT sgmModuleFree(void* data);

    SGuint SG_EXPORT sgmModuleMatch(SGModuleInfo** minfos, SGuint numinfos, SGbool* ok);
#ifdef __cplusplus
}
#endif

#endif // __SIEGE_BACKEND_MODULE_H__
