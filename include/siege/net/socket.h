#ifndef _SIEGE_NET_SOCKET_H_
#define _SIEGE_NET_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "../common.h"
#include "address.h"

typedef struct SGSocket
{
} SGSocket;

typedef struct SGSocketSet
{
} SGSocketSet;

/**
 * Create a socket set while specifying the capacity of the underlying fd_set.
 * The meaning of capacity differs on different platforms:
 *   * On POSIX platforms, capacity specifies the highest file descriptor
 *     that can be stored. fd_set is a bit array of file descriptors.
 *   * On Windows, capacity refers to the maximum number of sockets that
 *     can be stored. fd_set is an array of socket handles.
 */
SGSocketSet* SG_EXPORT sgSocketSetCreate(SGuint capacity);
/// Release the resources associated with a given socket set.
void SG_EXPORT sgSocketSetDestroy(SGSocketSet* socketSet);
/// Remove all sockets from the socket set.
void SG_EXPORT sgSocketSetReset(SGSocketSet* socketSet);
/// Add a given socket to the socket set.
void SG_EXPORT sgSocketSetAdd(SGSocketSet* socketSet, SGSocket* socket);
/// Remove a given socket from the socket set.
void SG_EXPORT sgSocketSetRemove(SGSocketSet* socketSet, SGSocket* socket);
/// Returns SG_TRUE if the given socket is in the socket set.
SGbool SG_EXPORT sgSocketSetInSet(SGSocketSet* socketSet, SGSocket* socket);
/// Returns the capacity (see sgSocketSetCreate) for the socket set.
SGuint SG_EXPORT sgSocketSetGetCapacity(SGSocketSet* socketSet);

enum
{
    SG_SOCKET_STREAM,
    SG_SOCKET_DGRAM
};

/// How a socket is shutdown; what is/are disallowed.
enum
{
    SG_SHUTDOWN_RECV,
    SG_SHUTDOWN_SEND,
    SG_SHUTDOWN_BOTH
};

/// Create a socket. Defaults to blocking.
SGSocket* SG_EXPORT sgSocketCreate(SGenum addressType, SGenum socketType);
/// Release the memory for a socket. Calls sgSocketClose if needed.
void SG_EXPORT sgSocketDestroy(SGSocket* socket);

/// Set the blocking status of the given socket.
void SG_EXPORT sgSocketSetBlocking(SGSocket* socket, SGbool blocking);
/// Retrieve the blocking status of the given socket.
SGbool SG_EXPORT sgSocketGetBlocking(SGSocket* socket);
/// Returns: true if this is a valid, living socket.
SGbool SG_EXPORT sgSocketIsAlive(SGSocket* socket);
/// Associate an address with this socket.
SGvoid SG_EXPORT sgSocketBind(SGSocket* socket, SGAddress* address);
/// Connect to an address. If socket is blocking, this waits until the connection is established.
SGvoid SG_EXPORT sgSocketConnect(SGSocket* socket, SGAddress* address);
/**
 * Listen for an incoming connection.
 * An address must have been associated with the socket with sgSocketBind.
 * Params:
 *   socket = the socket to listen on.
 *   backlog = how many connections to queue until they're accepted.
 */
SGvoid SG_EXPORT sgSocketListen(SGSocket* socket, SGint backlog);
/**
 * Accept an incoming connection.
 *
 * Returns: the accepted socket, or NULL if unable to accept.
 */
SGSocket* SG_EXPORT sgSocketAccept(SGSocket* socket);
/// Disable sends and recvs from this socket.
SGvoid SG_EXPORT sgSocketShutdown(SGSocket* socket, SGenum how);
/// Drop connections from this socket. Calling sgSocketShutdown is recommended.
SGvoid SG_EXPORT sgSocketClose(SGSocket* socket);

/**
 * Sends data out on the given socket.
 *
 * This call blocks if the socket is set to blocking mode.
 *
 * Params:
 *   socket = the socket to send data from.
 *   buf = the data to send.
 *   buflen = the length of the data, in bytes, in buf.
 * Returns: the actual amount of data sent.
 *          Less than zero on error.
 */
SGint SG_EXPORT sgSocketSend(SGSocket* socket, SGubyte* buf, SGint buflen);

/**
 * Receive data from the given socket.
 *
 * This call blocks if the socket is set to blocking mode.
 *
 * Params:
 *   socket = the socket to get data from.
 *   buf = a buffer to store received data.
 *   maxbuf = the total length of the buf buffer.
 *
 * Returns: the number of bytes received. 
 *          Zero if the remote closed the connection. 
 *          Less than zero on an error.
 */
SGint SG_EXPORT sgSocketRecv(SGSocket* socket, SGubyte* buf, SGint maxbuf)

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _SIEGE_NET_SOCKET_H_
