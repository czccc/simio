//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_WAKER_H
#define SIMIO_WAKER_H

#include "selector/epoll.h"

namespace simio {

namespace sys {

class Waker {
  public:
    Waker(EpollSelector s, Token token);
    ~Waker() = default;

    bool wake();
    bool reset() const;

  private:
    int fd;
};

}

}

#endif //SIMIO_WAKER_H
