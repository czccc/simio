//
// Created by Cheng on 2021/3/5.
//

#include <unistd.h>
#include <sys/uio.h>
#include "simio.h"
//
// int simio::sys::peek(int fd, void *buf, size_t count) {
//     return ::recv(fd, buf, count, MSG_PEEK);
// }
// int simio::sys::recv(int fd, void *buf, size_t count) {
//     return ::recv(fd, buf, count, 0);
// }
//
// int simio::sys::peek_from(int fd, void *buf, size_t count, SocketAddr &addr) {
//     auto *a = const_cast<sockaddr *>(addr.get());
//     return ::recvfrom(fd, buf, count, MSG_PEEK, a, reinterpret_cast<socklen_t *>(sizeof(*a)));
// }
// int simio::sys::recv_from(int fd, void *buf, size_t count, SocketAddr &addr) {
//     auto *a = const_cast<sockaddr *>(addr.get());
//     return ::recvfrom(fd, buf, count, 0, a, reinterpret_cast<socklen_t *>(sizeof(*a)));
// }

int simio::sys::read(int fd, void *buf, size_t count) {
    return ::read(fd, buf, count);
}

int simio::sys::read_vectored(int fd, struct iovec *iov, int iovcnt) {
    return ::readv(fd, iov, iovcnt);
}

int simio::sys::write(int fd, const void *buf, size_t count) {
    return ::write(fd, buf, count);
}

int simio::sys::write_vectored(int fd, const struct iovec *iov, int iovcnt) {
    return ::writev(fd, iov, iovcnt);
}
// int simio::sys::flush(int fd, const void *buf, size_t count) {
//     return ::fflush()
// }