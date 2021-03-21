//
// Created by Cheng on 2021/3/8.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_

#include <functional>

#include "simio/base/token.h"
#include "simio/base/interest.h"
#include "simio/net/tcp_listener.h"
#include "simio/net/tcp_stream.h"
#include "simio/event.h"

namespace simio {
class Task {
  public:
    explicit Task(const TcpStream &inner)
        : inner_(inner), interest_(Interest::READABLE()), token_(global_next_token.fetch_add(1)) {}
    Task(const TcpStream &inner, Interest interest)
        : inner_(inner), interest_(interest), token_(global_next_token.fetch_add(1)) {}
    Task(const TcpStream &inner, Interest interest, Token token) : inner_(inner), interest_(interest), token_(token) {}

    explicit Task(TcpStream &&inner)
        : inner_(inner), interest_(Interest::READABLE()), token_(global_next_token.fetch_add(1)) {}
    Task(TcpStream &&inner, Interest interest)
        : inner_(inner), interest_(interest), token_(global_next_token.fetch_add(1)) {}
    Task(TcpStream &&inner, Interest interest, Token token) : inner_(inner), interest_(interest), token_(token) {}

    void change_interest(Interest interest) {
        interest_ = interest;
        need_reregister_ = true;
    }

    void close() {
        need_deregister_ = true;
    }

    TcpStream inner_;

    Interest interest_;
    bool need_reregister_{false};
    bool need_deregister_{false};

    Token token_;

    std::function<void(Task *)> callback_{};

    void process() {
        callback_(this);
    }
};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_TASK_H_
