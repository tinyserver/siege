#define SG_BUILD_LIBRARY
#include <siege/util/net/socket.h>

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
static size_t _initCount = 0;
static WSADATA _initData;
#else
#endif /* WIN32 */

SGbool _sgSocketInit(void)
{
#ifdef __WIN32__
    _initCount++;
    return !WSAStartup(MAKEWORD(2, 2), &_initData);
#else
    return SG_TRUE;
#endif /* __WIN32__ */
}
SGbool _sgSocketDeinit(void)
{
    #ifdef __WIN32__
    _initCount--;
    if(!_initCount)
        return !WSACleanup();
    #endif /* __WIN32__ */
    return SG_TRUE;
}
