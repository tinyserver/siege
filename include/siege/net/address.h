#ifndef _SIEGE_NET_ADDRESS_H_
#define _SIEGE_NET_ADDRESS_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "../common.h"

enum
{
    SG_IP4_ADDRESS,
    SG_IP6_ADDRESS,
}

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
     *   SG_IP4_ADDRESS
     *   SG_IP6_ADDRESS
     */
    SGenum ipv;
    
    /**
     * The address representation.
     * One of these will be invalid, depending on the value of
     * the ipv field.
     */
    SGuint ip4Addr;
    SGubyte ip6Addr[16];
    
    SGushort port;
} SGAddress;

/**
 * Create a new SGAddress.
 * Params:
 *   addr = either 'a.b.c.d' or 'a::b::c::d::e' or a hostname to
 *          be resolved.
 *   port = the port number to connect through. Can be SG_PORT_ANY.
 */
SGAddress* SG_EXPORT sgAddressCreate(const char* addr, SGushort port);

void SG_EXPORT sgAddressDestroy(SGAddress*);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _SIEGE_NET_ADDRESS_H_
