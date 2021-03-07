//
// Created by Cheng on 2021/3/4.
//

#ifndef SIMIO_INCLUDE_SIMIO_EVENT_H_
#define SIMIO_INCLUDE_SIMIO_EVENT_H_

#include "simio/base/token.h"
#include "simio/base/interest.h"

#include "simio/sys.h"

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

/// `Event` is a readiness state_ paired with a Token. It is returned by Poll::poll.
class Event {
  public:

    explicit Event(const sys::Event &ev) : inner_(ev) {}

    static Event from_sys_event(const sys::Event &ev) {
        return Event(ev);
    }

    Token token() { return sys::get_token(inner_); }

    bool is_readable() { return sys::is_readable(inner_); }

    bool is_writable() { return sys::is_writable(inner_); }

    bool is_priority() { return sys::is_priority(inner_); }

    bool is_error() { return sys::is_error(inner_); }

    bool is_read_closed() { return sys::is_read_closed(inner_); }

    bool is_write_closed() { return sys::is_write_closed(inner_); }

  private:
    sys::Event inner_;
};

/// A list of event returned by Poll
// using EventList = std::vector<Event>;

class EventList {

  public:

    explicit operator sys::EventList() {
        return inner_;
    }
    sys::EventList &as_vec() {
        return inner_;
    }
    explicit EventList(int capacity) : inner_(sys::EventList(capacity)) { inner_.clear(); }

    uint32_t capacity() { return inner_.capacity(); }

    bool is_empty() { return inner_.empty(); }

    void clear() { inner_.clear(); }

    sys::EventList &get_inner() { return inner_; }

  private:

    sys::EventList inner_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_EVENT_H_
