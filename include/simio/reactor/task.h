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
template<typename T>
class IOStreamTask {
  public:
    explicit IOStreamTask(T inner)
        : inner_(inner), token_(global_next_token.fetch_add(1)), interest_(Interest::READABLE()) {}
    IOStreamTask(T inner, Token token, Interest interest) : inner_(inner), token_(token), interest_(interest) {}

    T inner_;
    Token token_;
    Interest interest_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_
