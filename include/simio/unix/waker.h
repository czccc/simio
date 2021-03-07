//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_UNIX_WAKER_H_
#define SIMIO_INCLUDE_SIMIO_UNIX_WAKER_H_

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

class Selector;

class Waker {
  public:
    Waker(const Selector &s, Token token);
    ~Waker() = default;

    bool wake();
    bool reset() const;

  private:
    int fd;
};

}
}

#endif //SIMIO_INCLUDE_SIMIO_UNIX_WAKER_H_
