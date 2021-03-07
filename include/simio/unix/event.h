//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_UNIX_EVENT_H_
#define SIMIO_INCLUDE_SIMIO_UNIX_EVENT_H_

#include <sys/epoll.h>
#include <vector>
#include <atomic>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#include "simio/base/token.h"
#include "simio/base/interest.h"
#include "simio/base/socket_addr.h"

namespace simio {
namespace sys {

using Event = epoll_event;
using EventList = std::vector<Event>;

Token get_token(Event event);

bool is_readable(Event event);

bool is_writable(Event event);

bool is_priority(Event event);

bool is_error(Event event);

bool is_read_closed(Event event);

bool is_write_closed(Event event);

}
}

#endif //SIMIO_INCLUDE_SIMIO_UNIX_EVENT_H_
