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
void SG_EXPORT sgSocketDestroy(SGSocket* socket);

void SG_EXPORT sgSocketSetBlocking(SGSocket* socket, SGbool blocking);
SGbool SG_EXPORT sgSocketGetBlocking(SGSocket* socket);

SGbool SG_EXPORT sgSocketIsAlive(SGSocket* socket);

SGvoid SG_EXPORT sgSocketBind(SGSocket* socket, SGAddress* address);
SGvoid SG_EXPORT sgSocketConnect(SGSocket* socket, SGAddress* address);
SGvoid SG_EXPORT sgSocketListen(SGSocket* socket, SGint backlog);
SGSocket* SG_EXPORT sgSocketAccept(SGSocket* socket);
SGvoid SG_EXPORT sgSocketShutdown(SGSocket* socket, SGenum how);
SGvoid SG_EXPORT sgSocketClose(SGSocket* socket);

SGint SG_EXPORT sgSocketSend(SGSocket* socket, SGubyte* buf, SGint buflen);
SGint SG_EXPORT sgSocketRecv(SGSocket* socket, SGubyte* buf, SGint maxbuf, SGint* buflen)

SGvoid SG_EXPORT sgSocketKeepAlive(SGSocket* socket, SGint time, SGint interval);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _SIEGE_NET_SOCKET_H_
