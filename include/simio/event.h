//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_EVENT_H_
#define SIMIO_INCLUDE_SIMIO_EVENT_H_

#include "base.h"
#include "unix.h"

namespace simio {

class Registry;

/// An EventSource that may be registered with Registry.
///
/// Types that implement EventSource can be registered with `Registry`.
///
/// Users should not use the EventSource class directly.
/// Instead, the equivalent functions on Registry should be used.
class EventSource {
  public:

    virtual void event_register(Registry *registry, Token token, Interest interest) = 0;
    virtual void event_reregister(Registry *registry, Token token, Interest interest) = 0;
    virtual void event_deregister(Registry *registry) = 0;
};

/// `Event` is a readiness state paired with a Token. It is returned by Poll::poll.
class Event {
  protected:

    static Event from_sys_event(const sys::Event &ev) {
        return Event(ev);
    }

  public:

    Token token() { return sys::get_token(inner); }

    bool is_readable() { return sys::is_readable(inner); }

    bool is_writable() { return sys::is_writable(inner); }

    bool is_priority() { return sys::is_priority(inner); }

    bool is_error() { return sys::is_error(inner); }

    bool is_read_closed() { return sys::is_read_closed(inner); }

    bool is_write_closed() { return sys::is_write_closed(inner); }

  private:
    explicit Event(const sys::Event &ev) : inner(ev) {}
    sys::Event inner;
};

/// A list of event returned by Poll
using EventList = std::vector<Event>;

// class EventList {
//
//   public:
//
//     explicit EventList(int capacity) : inner(sys::EventList(capacity)) {}
//
//     uint32_t capacity() { return inner.capacity(); }
//
//     bool is_empty() { return inner.empty(); }
//
//     void clear() { inner.clear(); }
//
//     sys::EventList &get_inner() { return inner; }
//
//   private:
//
//     sys::EventList inner;
// };

}

#endif //SIMIO_INCLUDE_SIMIO_EVENT_H_