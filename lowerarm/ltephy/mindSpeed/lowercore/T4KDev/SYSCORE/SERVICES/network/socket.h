
#ifndef __SOCKET_H__
#define __SOCKET_H__


//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned int socklen_t;

#ifndef __timeval__
#define __timeval__
typedef struct _timeval_t
{
     long tv_sec;     /* seconds */
     long tv_usec;    /* and microseconds */
} timeval;
#endif

//--------- Address Families --------

#define  AF_INET                2
#define  AF_INET6               23

//------------ Socket Types ------------

#define  SOCK_STREAM            1
#define  SOCK_DGRAM             2
#define  SOCK_RAW               3
#define  SOCK_RDM               4
#define  SOCK_SEQPACKET         5

//----------- Socket Protocol ----------

#define IPPROTO_IP              0           // dummy for IP
#define IPPROTO_ICMP            1           // control message protocol
#define IPPROTO_IPV4            IPPROTO_IP  // IP inside IP
#define IPPROTO_TCP             6           // tcp
#define IPPROTO_UDP             17          // user datagram protocol
#define IPPROTO_IPV6            41          // IPv6 in IPv6
#define IPPROTO_NONE            59          // No next header
#define IPPROTO_RAW             255         // raw IP packet
#define IPPROTO_MAX             256

//----------- Socket Options -----------

#define  SOL_SOCKET             0xFFFF
#define  SO_DEBUG               0x0001
#define  SO_ACCEPTCONN          0x0002
#define  SO_REUSEADDR           0x0004
#define  SO_KEEPALIVE           0x0008
#define  SO_DONTROUTE           0x0010
#define  SO_BROADCAST           0x0020
#define  SO_USELOOPBACK         0x0040
#define  SO_LINGER              0x0080
#define  SO_OOBINLINE           0x0100
#define  SO_SNDBUF              0x1001
#define  SO_RCVBUF              0x1002
#define  SO_SNDLOWAT            0x1003
#define  SO_RCVLOWAT            0x1004
#define  SO_SNDTIMEO            0x1005
#define  SO_RCVTIMEO            0x1006
#define  SO_ERROR               0x1007
#define  SO_TYPE                0x1008
#define  SO_OPENTYPE            0x7008

#define  SO_SYNCHRONOUS_ALERT   0x10
#define  SO_SYNCHRONOUS_NONALERT \
                                0x20

#define  TCP_NODELAY            0x0001
#define  TCP_BSDURGENT          0x7000

#define  MAX_PACKET_SIZE        1500
#define  MAX_LISTEN_QUQUE       4

#define  IOCTL_SOCKET_EVENTMASK

#define ENOBUFS                 55          // No buffer space available
#define EAGAIN                  11	

#define __FD_SETSIZE            32

#define IP_TOS		4

#define INADDR_BROADCAST	0xFFFFFFFF
#define INADDR_LOOPBACK		0x7F000001
#define INADDR_ANY		0x00000000	

#define IN_CLASSD(a)            ((((long int) (a)) & 0xf0000000) == 0xe0000000)
#define IN_MULTICAST(a)         IN_CLASSD(a)
#define IN_MULTICAST_NET        0xF0000000


typedef struct _in_addr_t
{
    unsigned long s_addr;                   // load with inet_aton()
} in_addr;

typedef struct _sockaddr_t
{
    unsigned short int    sa_family;
    unsigned char     sa_data[14];
} sockaddr;

typedef struct _sockaddr_in_t
{
    short            sin_family;            // e.g. AF_INET
    unsigned short   sin_port;              // e.g. htons(3490)
    in_addr          sin_addr;              // see struct in_addr, below
    char             sin_zero[8];           // zero this if you want to
} sockaddr_in;

typedef unsigned int socklen_t;

// The fd_set member is required to be an array of longs.
typedef long int __fd_mask;

// It's easier to assume 8-bit bytes than to get CHAR_BIT.
#define __NFDBITS               (8 * sizeof (__fd_mask))
#define __FDELT(d)              ((d) / __NFDBITS)
#define __FDMASK(d)             ((__fd_mask) 1 << ((d) % __NFDBITS))

// fd_set for select and pselect.
typedef struct
{
    __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set)        ((set)->fds_bits)
} fd_set;

// We don't use `memset' because this would require a prototype and
//   the array isn't too big.
# define __FD_ZERO(set)                               \
  do {                                                \
    unsigned int __i;                                 \
    fd_set *__arr = (set);                            \
    for (__i = 0; __i < sizeof (fd_set) / sizeof (__fd_mask); ++__i) \
      __FDS_BITS (__arr)[__i] = 0;                    \
  } while (0)
# define __FD_SET(d, set)       (__FDS_BITS (set)[__FDELT (d)] |= __FDMASK (d))
# define __FD_CLR(d, set)       (__FDS_BITS (set)[__FDELT (d)] &= ~__FDMASK (d))
# define __FD_ISSET(d, set)     (__FDS_BITS (set)[__FDELT (d)] & __FDMASK (d))

// Access macros for 'fd_set'.
#define FD_SET(fd, fdsetp)      __FD_SET (fd, fdsetp)
#define FD_CLR(fd, fdsetp)      __FD_CLR (fd, fdsetp)
#define FD_ISSET(fd, fdsetp)    __FD_ISSET (fd, fdsetp)
#define FD_ZERO(fdsetp)         __FD_ZERO (fdsetp)

#ifndef INET6_ADDRSTRLEN
    #define INET6_ADDRSTRLEN    40
#endif

#ifndef INET_ADDRSTRLEN
    #define INET_ADDRSTRLEN     15
#endif

#ifdef HAVE_IPV6
    #define REPORT_ADDRLEN      (INET6_ADDRSTRLEN + 1)
    typedef struct sockaddr_storage iperf_sockaddr;
#else
    #define REPORT_ADDRLEN      (INET_ADDRSTRLEN + 1)
    typedef sockaddr_in         iperf_sockaddr;
#endif

#define htonl(A)                ((((unsigned int)(A) & 0xff000000) >> 24) | \
                                (((unsigned int)(A) & 0x00ff0000) >> 8) | \
                                (((unsigned int)(A) & 0x0000ff00) << 8) | \
                                (((unsigned int)(A) & 0x000000ff) << 24))

#define ntohl                   htonl

#define htons(A)                ((((unsigned int)(A) & 0xff00) >> 8) | \
                                (((unsigned int)(A) & 0x00ff) << 8))

#define ntohs                   htons


#define closesocket(a) close(a)

extern int socket_errno;    
    
extern int getpeername(int fd, sockaddr *addr, socklen_t *len);
extern int getsockname(int fd, sockaddr *address, socklen_t *address_len);
extern int socket(int domain, int type, int protocol);
extern int close(int sd);
extern int bind(int sd, const sockaddr *addr, int addrlen);
extern int connect(int sd, const sockaddr *addr, int addrlen);
extern int setsockopt(int sd, int level, int optname, const void *optval,
                      socklen_t optlen);
extern int getsockopt(int sd, int level, int optname, void *optval,
                      socklen_t *optlen);
extern int read(int sd, void *buf, int len);
extern int recv(int sd, void *buf, int len, int flags);
extern int recvfrom(int sd, void *buf, int len, int flags, sockaddr *from, 
                    socklen_t *fromlen);
extern int write(int sd, const void *buf, int len);
extern int send(int sd, const void *buf, int len, int flags);
extern int sendto(int sd, const void *buf, int len, int flags, 
                  const sockaddr *to, socklen_t tolen);

int accept (int sd, sockaddr *addr, socklen_t *addrlen);
int listen (int sd, int backlog);
int select (int nfds, fd_set *readsds, fd_set *writesds, fd_set *exceptsds,
            timeval *timeout);


#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // __SOCKET_H__
