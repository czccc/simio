//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_EVENT_EVENT_LIST_H_
#define SIMIO_EVENT_EVENT_LIST_H_

#include "../sys/unix/selector/epoll.h"

namespace simio {

class EventList {
  public:
    explicit EventList(int capacity) : inner(sys::EventList(capacity)) {}
    uint32_t capacity() { return inner.capacity(); }
    bool is_empty() { return inner.empty(); }
    void clear() { inner.clear(); }
    sys::EventList &get_inner() { return inner; }
  private:
    sys::EventList inner;
};

}

#endif //SIMIO_EVENT_EVENT_LIST_H_
