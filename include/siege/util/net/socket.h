#ifndef __SIEGE_UTIL_NET_SOCKET_H__
#define __SIEGE_UTIL_NET_SOCKET_H__

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "../../common.h"
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
/// Creates a pair of interconnected sockets.
void SG_EXPORT sgSocketPair(SGSocket** a, SGSocket** b);

/// Set the blocking status of the given socket.
void SG_EXPORT sgSocketSetBlocking(SGSocket* socket, SGbool blocking);
/// Retrieve the blocking status of the given socket.
SGbool SG_EXPORT sgSocketGetBlocking(SGSocket* socket);
/// Returns: true if this is a valid, living socket.
SGbool SG_EXPORT sgSocketIsAlive(SGSocket* socket);
/// Associate an address with this socket.
void SG_EXPORT sgSocketBind(SGSocket* socket, SGAddress* address);
/// Connect to an address. If socket is blocking, this waits until the connection is established.
void SG_EXPORT sgSocketConnect(SGSocket* socket, SGAddress* address);
/**
 * Listen for an incoming connection.
 * An address must have been associated with the socket with sgSocketBind.
 * Params:
 *   socket = the socket to listen on.
 *   backlog = how many connections to queue until they're accepted.
 */
void SG_EXPORT sgSocketListen(SGSocket* socket, SGuint backlog);

/**
 * Wait for a socket to change status.
 * *Select    = timeout in seconds.
 * *NSelect   = timeout in nanoseconds.
 * *USelect   = timeout in microseconds.
 * *MSelect   = timeout in milliseconds.
 * *MaxSelect = use the maximum timeout.
 *
 * Returns the number of sockets with status changes. 0 if timeout, -1 on an interruption.
 * Otherwise, the SGSocketSets are updated to contain only the sockets having status changes.
 * For a connecting socket, a write status change means the connection is established and it's able to send. 
 * For a listening socket, a read status change means there is an incoming connection request and it's able to accept.
 */

SGint SG_EXPORT sgSocketSelect(SGSocketSet* checkRead, SGSocketSet* checkWrite, SGSocketSet* checkError, SGulong timeout);
SGint SG_EXPORT sgSocketNSelect(SGSocketSet* checkRead, SGSocketSet* checkWrite, SGSocketSet* checkError, SGulong timeout);
SGint SG_EXPORT sgSocketUSelect(SGSocketSet* checkRead, SGSocketSet* checkWrite, SGSocketSet* checkError, SGulong timeout);
SGint SG_EXPORT sgSocketMSelect(SGSocketSet* checkRead, SGSocketSet* checkWrite, SGSocketSet* checkError, SGulong timeout);
SGint SG_EXPORT sgSocketMaxSelect(SGSocketSet* checkRead, SGSocketSet* checkWrite, SGSocketSet* checkError);

/**
 * Accept an incoming connection.
 *
 * Returns: the accepted socket, or NULL if unable to accept.
 */
SGSocket* SG_EXPORT sgSocketAccept(SGSocket* socket);
/// Disable sends and recvs from this socket.
void SG_EXPORT sgSocketShutdown(SGSocket* socket, SGenum how);
/// Drop connections from this socket. Calling sgSocketShutdown is recommended.
void SG_EXPORT sgSocketClose(SGSocket* socket);

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
SGint SG_EXPORT sgSocketSend(SGSocket* socket, SGubyte* buf, size_t buflen);

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
SGint SG_EXPORT sgSocketRecv(SGSocket* socket, SGubyte* buf, size_t maxbuf);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __SIEGE_UTIL_NET_SOCKET_H__
