//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_UNIX_IO_SOURCE_STATE_H_
#define SIMIO_INCLUDE_SIMIO_UNIX_IO_SOURCE_STATE_H_

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

class IOSourceState {
  public:
    IOSourceState() = default;

    // template<typename T>
    // using IOCallback = std::function<int(T)>;

    template<typename IOCallback, typename T, typename RET>
    RET do_io(const IOCallback &f, T &io) {
        return f(io->as_raw_fd());
    }
};

}
}

#endif //SIMIO_INCLUDE_SIMIO_UNIX_IO_SOURCE_STATE_H_
