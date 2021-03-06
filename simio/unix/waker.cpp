//
// Created by Cheng on 2021/2/28.
//

#include <sys/eventfd.h>
#include <cerrno>

#include "simio.h"

namespace simio {

namespace sys {

Waker::Waker(const Selector &selector, Token token) {
    fd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    //    fd = open(fd_raw);
    selector.event_register(fd, token, Interest::READABLE());
}

bool Waker::wake() {

    eventfd_t buf = 1;
    int ret = eventfd_write(fd, buf);
    if (ret < 0) {
        if (errno == EWOULDBLOCK) {
            if (!reset()) {
                return false;
            }
            return wake();
        }
        return false;
    }
    return true;
}

bool Waker::reset() const {
    eventfd_t buf = 1;
    int ret = eventfd_write(fd, buf);
    if (ret < 0) {
        if (errno == EWOULDBLOCK) {
            return true;
        }
        return false;
    }
    return true;
}

}

}