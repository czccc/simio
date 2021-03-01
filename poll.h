//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO__POLL_H_
#define SIMIO__POLL_H_

#include <utility>

#include "sys/unix/selector/epoll.h"
#include "event/source.h"

namespace simio {

class Registry {
  public:
    sys::Selector selector;

    explicit Registry(const sys::Selector &s) : selector(s) {}
    void event_register(EventSource s, Token token, Interest interest) {
        s.event_register(this, token, interest);
    }
    void event_reregister(EventSource s, Token token, Interest interest) {
        s.event_reregister(this, token, interest);
    }
    void event_deregister(EventSource s) {
        s.event_deregister(this);
    }
    void register_waker() {
        if (selector.register_waker()) {
            // TODO: exit
        }
    }
};

class Poll {
  public:
    Poll() : registry_(Registry(sys::Selector())) {}
    Registry *registry() {
        return &registry_;
    }
    void poll(sys::EventList event, int timeout) const {
        registry_.selector.select(std::move(event), timeout);
    }
  private:
    Registry registry_;
};

sys::Selector &selector(Registry &registry) {
    return registry.selector;
}

}

#endif //SIMIO__POLL_H_
