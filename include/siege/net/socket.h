#ifndef _SIEGE_NET_SOCKET_H_
#define _SIEGE_NET_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "../common.h"
#include "address.h"

enum
{
    SG_SOCKET_STREAM,
    SG_SOCKET_DGRAM
};

/// How a socket is shutdown; what is/are disallowed.
enum
{
    SG_SOCKET_SHUTDOWN_RECEIVE,
    SG_SOCKET_SHUTDOWN_SEND,
    SG_SOCKET_SHUTDOWN_BOTH
};

typedef struct SGSocket
{
} SGSocket;

SGSocket* SG_EXPORT sgSocketCreate(SGenum addressType, SGenum socketType);
void SG_EXPORT sgSocketDestroy(SGSocket* handle);

void SG_EXPORT sgSocketSetBlocking(SGSocket* handle, SGbool blocking);
SGbool SG_EXPORT sgSocketGetBlocking(SGSocket* handle);

SGbool SG_EXPORT sgSocketIsAlive(SGSocket* handle);

SGvoid SG_EXPORT sgSocketBind(SGAddress* address);
SGvoid SG_EXPORT sgSocketConnect(SGAddress* address);
SGvoid SG_EXPORT sgSocketListen(SGint backlog);
SGSocket* SG_EXPORT sgSocketAccept(void);
SGvoid SG_EXPORT sgSocketShutdown(SGenum how);
SGvoid SG_EXPORT sgSocketClose();

SGint SG_EXPORT sgSocketSend(SGubyte* buf, SGint buflen);
SGint SG_EXPORT sgSocketRecv(SGubyte* buf, SGint maxbuf, SGint* buflen)

SGvoid SG_EXPORT sgSocketKeepAlive(SGint time, SGint interval);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _SIEGE_NET_SOCKET_H_
