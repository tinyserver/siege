#define SG_BUILD_LIBRARY
#include <siege/util/net/address.h>
#include <siege/util/net/socket.h>
#include <siege/util/string.h>

#include <stdlib.h>
#include <string.h>

#ifdef __WIN32__
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#endif /* WIN32 */

static SGAddress* _sgAddressCreate(SGenum family, const void* ptr, SGushort port)
{
    SGAddress* addr = malloc(sizeof(SGAddress));
    if(!addr) return NULL;

    if(!_sgSocketInit())
    {
        free(addr);
        return NULL;
    }

    addr->family = family;
    if(ptr)
        memcpy(addr->addr.ptr, ptr, (family == SG_AF_IP6) ? sizeof(addr->addr.ip6) : (family == SG_AF_IP4) ? sizeof(addr->addr.ip4) : 0);
    addr->port = port;

    return addr;
}
static size_t _sgAddressPopulateSS(SGAddress* addr, struct sockaddr_storage* ss, SGbool cport)
{
    struct sockaddr_in* ip4;
    struct sockaddr_in6* ip6;
    switch(addr->family)
    {
    case SG_AF_IP4:
        ip4 = (struct sockaddr_in*)ss;
        ip4->sin_family = AF_INET;
        ip4->sin_port = cport ? addr->port : 0;
        memcpy(&ip4->sin_addr, &addr->addr.ip4, sizeof(addr->addr.ip4));
        return sizeof(struct sockaddr_in);
    case SG_AF_IP6:
        ip6 = (struct sockaddr_in6*)ss;
        ip6->sin6_family = AF_INET6;
        ip6->sin6_port = cport ? addr->port : 0;
        ip6->sin6_flowinfo = 0;
        memcpy(&ip6->sin6_addr, addr->addr.ip6, sizeof(addr->addr.ip6));
        ip6->sin6_scope_id = 0;
        return sizeof(struct sockaddr_in6);
    case SG_AF_UNSPEC:
        break;
    }
    ss->ss_family = AF_UNSPEC;
    return sizeof(struct sockaddr);
}

SGAddress* SG_EXPORT sgAddressCreate4v(const void* ptr, SGushort port)
{
    return _sgAddressCreate(SG_AF_IP4, ptr, port);
}
SGAddress* SG_EXPORT sgAddressCreate4(SGuint ip4, SGushort port)
{
    return _sgAddressCreate(SG_AF_IP4, &ip4, port);
}
SGAddress* SG_EXPORT sgAddressCreate6v(const void* ptr, SGushort port)
{
    return _sgAddressCreate(SG_AF_IP6, ptr, port);
}
SGAddress* SG_EXPORT sgAddressCreate6(const SGulong ip6[2], SGushort port)
{
    return _sgAddressCreate(SG_AF_IP6, ip6, port);
}

SGAddress* SG_EXPORT sgAddressCreateHost(const char* host, SGushort port, SGenum flags)
{
    if(!_sgSocketInit()) return NULL;

    SGAddress* addr = NULL;
    
    char sport[6];
    sgSPrintf(sport, 6, "%u", port);

    struct addrinfo hints = { 0 };
    if(flags & SG_AI_PASSIVE)
        hints.ai_flags |= AI_PASSIVE;

    struct addrinfo* ainfo;
    if(getaddrinfo(host, (flags & SG_AI_PASSIVE) ? NULL : sport, &hints, &ainfo))
        return NULL;

    struct sockaddr* res = ainfo->ai_addr;
    struct sockaddr_in* res4;
    struct sockaddr_in6* res6;

    switch(res->sa_family)
    {
    case AF_INET:
        res4 = (struct sockaddr_in*)res;
        addr = sgAddressCreate4v(&res4->sin_addr, res4->sin_port);
        break;
    case AF_INET6:
        res6 = (struct sockaddr_in6*)res;
        addr = sgAddressCreate6v(&res6->sin6_addr, res6->sin6_port);
        break;
    default:
        addr = _sgAddressCreate(SG_AF_UNSPEC, NULL, port);
    }

    freeaddrinfo(ainfo);

    /* to compensate for the init above */
    _sgSocketDeinit();

    return addr;
}

void SG_EXPORT sgAddressDestroy(SGAddress* addr)
{
    if(!addr) return;
    free(addr);
    _sgSocketDeinit();
}

SGbool SG_EXPORT sgAddressToString(SGAddress* addr, char* buf, size_t buflen, SGbool cport)
{
    if(!buf || !buflen)
        return SG_TRUE;
#ifdef __WIN32__
    struct sockaddr_storage ss;
    size_t size = _sgAddressPopulateSS(addr, &ss, cport);

    DWORD dbuflen = buflen;
    return !WSAAddressToString((struct sockaddr*)&ss, size, NULL, buf, &dbuflen);
#else
    int af;
    switch(addr->family)
    {
    case SG_AF_IP4:    af = AF_INET; break;
    case SG_AF_IP6:    af = AF_INET6; break;
    case SG_AF_UNSPEC:
    default: af = AF_UNSPEC; break;
    }

    if(!addr->port)
        cport = SG_FALSE;

    char* ptr = buf;
    size_t sz = buflen;
    size_t slen;
    size_t plen;
    char pbuf[6];

    if(cport && addr->family == SG_AF_IP6)
    {
        ptr++;
        sz--;
    }

    const char* ret = inet_ntop(af, addr->addr.ptr, ptr, sz);
    if(ret == NULL) return SG_FALSE;

    if(cport)
    {
        plen = sgSPrintf(pbuf, sizeof(pbuf), "%hu", addr->port);
        
        slen = strlen(ptr);
        // base  ':'  port   '[' and ']'                      '\0'
        if(slen + 1 + plen + (addr->family == SG_AF_IP6) * 2 + 1 >= buflen)
            return SG_FALSE;
        if(addr->family == SG_AF_IP6)
        {
            buf[0] = '[';
            slen++;
            buf[slen] = ']';
            slen++;
        }
        buf[slen] = ':';
        memcpy(&buf[slen+1], pbuf, plen + 1);
    }
    return SG_TRUE;
#endif /* __WIN32__ */
}
