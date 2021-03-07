//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_REGISTRY_H_
#define SIMIO_INCLUDE_SIMIO_BASE_REGISTRY_H_

#include "simio/sys.h"
#include "simio/event.h"

namespace simio {

class Registry {
  public:

    Registry() : selector_() {
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
        if (selector_.register_waker()) {
            // TODO: exit
        }
    }

    sys::Selector &get_selector() {
        return selector_;
    }

  private:
    sys::Selector selector_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_REGISTRY_H_
