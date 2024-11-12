#include <unistd.h>
#include <poll.h>
#include "e.h"
#include "writeall.h"

int writeall(int fd, const void *xv, long long xlen) {

    const unsigned char *x = xv;
    long long w;
    while (xlen > 0) {
        w = xlen;
        if (w > 1048576) w = 1048576;
        w = write(fd, x, (unsigned long long) w);
        if (w < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
                struct pollfd p;
                p.fd = fd;
                p.events = POLLOUT | POLLERR;
                poll(&p, 1, -1);
                continue;
            }
            return -1;
        }
        x += w;
        xlen -= w;
    }
    return 0;
}
