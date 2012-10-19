#ifndef __SIEGE_UTIL_NET_ADDRESS_H__
#define __SIEGE_UTIL_NET_ADDRESS_H__

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "../../common.h"

#define SG_AF_UNSPEC 0
#define SG_AF_IP4    4
#define SG_AF_IP6    6
// aliases
#define SG_AF_INET   SG_AF_IP4
#define SG_AF_INET4  SG_AF_IP4
#define SG_AF_INET6  SG_AF_IP6

#define SG_AI_PASSIVE   0x01

/**
 * Used in place of a port to indicate any port may be used.
 */
#define SG_PORT_ANY 0

/**
 * Represents an internet address.
 */
typedef struct SGAddress
{
    /**
     * The IP protocol version of this address. One of:
     *   SG_AF_IP4
     *   SG_AF_IP6
     */
    SGenum family;
    
    /**
     * The address representation.
     * If SG_IP4_ADDRESS, only the first four bytes are relevant.
     */
    union
    {
        SGubyte ptr[16];
        SGuint  ip4;
        SGulong ip6[2];
    } addr;
    
    SGushort port;
} SGAddress;

SGAddress* SG_EXPORT sgAddressCreate4v(const void* ptr, SGushort port);
SGAddress* SG_EXPORT sgAddressCreate4(SGuint ip4, SGushort port);
SGAddress* SG_EXPORT sgAddressCreate6v(const void* ptr, SGushort port);
SGAddress* SG_EXPORT sgAddressCreate6(const SGulong ip6[2], SGushort port);
/**
 * Create a new SGAddress.
 * Params:
 *   str = either 'a.b.c.d' or 'a:b:c:d:e:f:g:h' or a hostname to
 *          be resolved.
 *   port = the port number to connect through. May be SG_PORT_ANY.
 */
SGAddress* SG_EXPORT sgAddressCreateHost(const char* host, SGushort port, SGenum flags);

void SG_EXPORT sgAddressDestroy(SGAddress* addr);

SGbool SG_EXPORT sgAddressToString(SGAddress* addr, char* buf, size_t buflen, SGbool cport);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __SIEGE_UTIL_NET_ADDRESS_H__
