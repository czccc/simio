//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_EVENT_EVENT_H_
#define SIMIO_EVENT_EVENT_H_

#include "../sys/unix/selector/epoll.h"

namespace simio {

class Event {
  public:
    Token token() { return sys::token(inner); }

    bool is_readable() { return sys::is_readable(inner); }

    bool is_writable() { return sys::is_writable(inner); }

    bool is_priority() { return sys::is_priority(inner); }

    bool is_error() { return sys::is_error(inner); }

    bool is_read_closed() { return sys::is_read_closed(inner); }

    bool is_write_closed() { return sys::is_write_closed(inner); }

  private:
    sys::Event inner;
};

}

#endif //SIMIO_EVENT_EVENT_H_
