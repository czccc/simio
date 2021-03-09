//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_EVENT_LOOP_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_EVENT_LOOP_H_

#include <mutex>
#include <memory>
#include "simio/base/noncopyable.h"
#include "simio/base/poll.h"
#include "simio/base/token.h"
#include "simio/base/interest.h"
#include "simio/event.h"

namespace simio {

class EventLoop : noncopyable {
  public:
    EventLoop() : poller_(nullptr), state_(State::init) {}
    ~EventLoop() { state_ = State::stopped; }

    void source_register(EventSource *source, Token token, Interest interest) {
        poller_->get_registry()->event_reregister(source, token, interest);
    }
    void source_reregister();
    void source_deregister();
  private:
    enum class State : int {
        init = 1,
        running = 2,
        blocking = 3,
        stopped = 4,
    };

    std::mutex mutex_;
    std::unique_ptr<Poll> poller_;
    State state_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_EVENT_LOOP_H_
