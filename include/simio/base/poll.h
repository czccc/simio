//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_POLL_H_
#define SIMIO_INCLUDE_SIMIO_BASE_POLL_H_

#include "simio/base/registry.h"
#include "simio/sys.h"
#include "simio/event.h"

namespace simio {

class Poll {
  public:
    Poll() : registry_() {}
    Registry *get_registry() { return &registry_; }
    void poll(EventList &events, int timeout_ms) {
        registry_.get_selector().select(reinterpret_cast<sys::EventList &>(events), timeout_ms);
    }
  private:
    Registry registry_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_POLL_H_
