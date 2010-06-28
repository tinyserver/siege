module siege.modules.modules;

private
{
    import derelict.util.loader;

    import siege.modules.common;
    import siege.modules.audio;
    import siege.modules.fonts;
    import siege.modules.graphics;
    import siege.modules.input;
    import siege.modules.physics;
    import siege.modules.window;

    import std.file;
    import std.gc;
}

enum: uint
{
    SG_MODULE_WINDOW       = 0x01,
    SG_MODULE_INPUT        = 0x02,
    SG_MODULE_CORE         = (SG_MODULE_WINDOW | SG_MODULE_INPUT),
    SG_MODULE_GRAPHICS     = 0x04,
    SG_MODULE_GRAPHICSLOAD = 0x08,
    SG_MODULE_AUDIO        = 0x10,
    SG_MODULE_AUDIOLOAD    = 0x20,
    SG_MODULE_FONTLOAD     = 0x40,
    SG_MODULE_PHYSICS      = 0x80,
}

void loadModule(SharedLib lib)
{
    loadModuleAudio(lib);
    loadModuleFonts(lib);
    loadModuleGraphics(lib);
    loadModuleInput(lib);
    loadModulePhysics(lib);
    loadModuleWindowing(lib);
}

extern(C)
{
    struct SGModuleInfo
    {
        ushort imajor;
        ushort iminor;
        ushort ipatch;

        ushort vmajor;
        ushort vminor;
        ushort vpatch;

        uint type;
        char* name;
        //char* longname;
        //char* description;

        void* data;
    }
}

class SiegeModule
{
    extern(C)
    {
        uint function(void* gc, SGModuleInfo** minfo)                 sgmModuleInit;
        uint function(SGModuleInfo* minfo)                            sgmModuleExit;
        //uint function(void* data)                                     sgmModuleFree;
        uint function(SGModuleInfo** minfos, uint numinfos, bool* ok) sgmModuleMatch;
    }

    void loadModuleGeneral(SharedLib lib)
    {
        checkBindFunc(sgmModuleInit)("sgmModuleInit", lib);
        checkBindFunc(sgmModuleExit)("sgmModuleExit", lib);
        //checkBindFunc(sgmModuleFree)("sgmModuleFree", lib);
        checkBindFunc(sgmModuleMatch)("sgmModuleMatch", lib);
    }

    SharedLib lib;
    SGModuleInfo* moduleInfo;
    char[] fname;
    this(char[] name)
    {
        char[][] dbglist;
        debug dbglist = listdir("Modules", "*[\\/]SGModule-"~name~".debug.*")
                      ~ listdir("Modules", "*[\\/]libSGModule-"~name~".debug.*");
        char[][] rellist = listdir("Modules", "*[\\/]SGModule-"~name~".*")
                         ~ listdir("Modules", "*[\\/]libSGModule-"~name~".*");

        // in debug builds, prefer *.debug.* versions; nevertheless, fall back to the "normal" variants
        debug
            char[][] files = dbglist ~ rellist; // prefer debug modules in debug mode, but fall back to release
        else
            char[][] files = rellist ~ dbglist; // prefer release modules in release mode, but fall back to debug
        if(files.length == 0)
            throw new Exception("Cannot load module " ~ name);
        fname = files[0];

        lib = Derelict_LoadSharedLib(fname);
        loadModuleGeneral(lib);
        loadModule(lib);

        if(sgmModuleInit !is null)
            sgmModuleInit(getGCHandle(), &moduleInfo);
    }

    ~this()
    {
        //if(sgmModuleFree !is null)
        //    sgmModuleFree(moduleInfo);
        if(sgmModuleExit !is null)
            sgmModuleExit(moduleInfo);
    }
}

