//
// Created by Cheng on 2021/3/8.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_

#include "simio/reactor/event_loop.h"
#include "simio/reactor/task.h"

namespace simio {

class Worker {
  public:
    Worker() : loop_();

    void add_task(Task task);
  private:
    std::map<Token, Task<int>> token_map_;
    EventLoop loop_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_
