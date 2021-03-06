//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_SYS_UNIX_SOURCE_FD_H_
#define SIMIO_SYS_UNIX_SOURCE_FD_H_

#include <functional>

#include "simio.h"

namespace simio {
namespace sys {

class SourceFd : EventSource {
  public:
    int fd;

    explicit SourceFd(int fd) : fd(fd) {}

    void event_register(Registry *registry, Token token, Interest interest) override {
        registry->get_selector().event_register(fd, token, interest);
    }
    void event_reregister(Registry *registry, Token token, Interest interest) override {
        registry->get_selector().event_reregister(fd, token, interest);
    }
    void event_deregister(Registry *registry) override {
        registry->get_selector().event_deregister(fd);
    }
};

}
}

#endif //SIMIO_SYS_UNIX_SOURCE_FD_H_
