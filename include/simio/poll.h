//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_POLL_H_
#define SIMIO_INCLUDE_SIMIO_POLL_H_

#include "unix.h"
#include "event.h"

namespace simio {

class EventSource;

class Waker {
  public:
    Waker(const sys::Selector &s, Token token) : inner(sys::Waker(s, token)) {}
    ~Waker() = default;
    bool wake() { return inner.wake(); }

  private:
    sys::Waker inner;
};

class Registry {
  public:

    Registry() : selector() {
    }
    void event_register(EventSource *s, Token token, Interest interest) {
        s->event_register(this, token, interest);
    }
    void event_reregister(EventSource *s, Token token, Interest interest) {
        s->event_reregister(this, token, interest);
    }
    void event_deregister(EventSource *s) {
        s->event_deregister(this);
    }
    void register_waker() {
        if (selector.register_waker()) {
            // TODO: exit
        }
    }

    sys::Selector &get_selector() {
        return selector;
    }

  private:
    sys::Selector selector;
};

class Poll {
  public:
    Poll() : registry() {}
    Registry *get_registry() {
        return &registry;
    }
    void poll(sys::EventList event, int timeout) {
        registry.get_selector().select(std::move(event), timeout);
    }
  private:
    Registry registry;
};

// sys::Selector &selector(Registry &registry) {
//     return registry.selector;
// }


}

#endif //SIMIO_INCLUDE_SIMIO_POLL_H_
