//
// Created by Cheng on 2021/3/8.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_

#include "simio/base/token.h"
#include "simio/base/interest.h"
#include "simio/net/tcp_listener.h"
#include "simio/net/tcp_stream.h"
#include "simio/event.h"

namespace simio {
class Task {
  public:
    virtual void on_readable() = 0;
    virtual void on_writable() = 0;
};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_
