//
// Created by Cheng on 2021/3/5.
//

#include "simio.h"

simio::Token simio::sys::get_token(Event event) {
    return event.data.u64;
}

bool simio::sys::is_readable(Event event) {
    return (event.events & EPOLLIN) || (event.events & EPOLLPRI);
}

bool simio::sys::is_writable(Event event) {
    return event.events & EPOLLOUT;
}

bool simio::sys::is_priority(Event event) {
    return event.events & EPOLLPRI;
}

bool simio::sys::is_error(Event event) {
    return event.events & EPOLLERR;
}

bool simio::sys::is_read_closed(Event event) {
    return event.events & EPOLLHUP || (event.events & EPOLLIN && event.events & EPOLLHUP);
}

bool simio::sys::is_write_closed(Event event) {
    return event.events & EPOLLHUP || (event.events & EPOLLOUT && event.events & EPOLLERR) ||
        (event.events & EPOLLERR);
}