//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_UNIX_EPOLL_H_
#define SIMIO_INCLUDE_SIMIO_UNIX_EPOLL_H_

#include <sys/epoll.h>
#include <vector>
#include <atomic>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#include "simio/base/token.h"
#include "simio/base/interest.h"
#include "simio/base/socket_addr.h"

namespace simio {
namespace sys {

class Selector {
  public:

    Selector();
    ~Selector();

    // Selector(Selector &rhs);
    // Selector &operator=(Selector const &rhs);

    // Selector(Selector &&rhs) noexcept;
    // Selector &operator=(Selector &&rhs) noexcept;

    int select(EventList &events, int timeout) const;

    void event_register(int fd, Token token, const Interest &interest) const;

    void event_reregister(int fd, Token token, const Interest &interest) const;

    void event_deregister(int fd) const;

    bool register_waker();

    int get_id() const {
        return id;
    }

  private:
    static std::atomic<int> next_id;
    int id;
    int ep;
    std::atomic<bool> has_waker;

};

}
}

#endif //SIMIO_INCLUDE_SIMIO_UNIX_EPOLL_H_
