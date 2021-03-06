//
// Created by Cheng on 2021/2/28.
//

#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>

#include "simio.h"

using namespace simio;
using simio::sys::Selector;

unsigned int interests_to_epoll(Interest interests) {
    // unsigned int kind = EPOLLET;
    unsigned int kind = 0;
    if (interests.is_readable()) {
        kind = kind | EPOLLIN | EPOLLRDHUP;
    }
    if (interests.is_writable()) {
        kind = kind | EPOLLOUT;
    }
    return kind;
}

std::atomic<int> Selector::next_id{1};

Selector::Selector()
    : ep(epoll_create1(EPOLL_CLOEXEC)),
      has_waker(false),
      id(next_id.fetch_add(1, std::memory_order_seq_cst)) {
    if (ep < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in epoll id: " << id << " create. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
}

Selector::~Selector() {
    close(ep);
}

// Selector::Selector(Selector &rhs)
//     : ep(rhs.ep),
//       has_waker(rhs.has_waker.load()),
//       id(rhs.id) {
//
// }
//
// Selector &Selector::operator=(Selector const &rhs) {
//     if (this != &rhs) {
//         ep = rhs.ep;
//         id = rhs.id;
//         has_waker.store(rhs.has_waker.load());
//     }
//     return *this;
// }
//
// Selector::Selector(Selector &&rhs) noexcept: has_waker(false) {
//     ep = rhs.ep;
//     id = rhs.id;
//     bool pre_has_waker = rhs.has_waker.load();
//     has_waker.store(pre_has_waker);
// }
//
// Selector &Selector::operator=(Selector &&rhs) noexcept {
//     if (this != &rhs) {
//         ep = rhs.ep;
//         id = rhs.id;
//         has_waker.store(rhs.has_waker.load());
//     }
//     return *this;
// }

int Selector::select(EventList &events, int timeout_ms) const {
    int num_events = epoll_wait(ep,
                                events.data(),
                                static_cast<int>(events.capacity()),
                                timeout_ms);

    if (num_events < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in epoll id: " << id << " select. " << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
    events.erase(events.begin() + num_events, events.end());
    return 0;
}

void Selector::event_register(int fd, Token token, const Interest &interest) const {
    epoll_event event{};
    event.events = interests_to_epoll(interest);
    event.data.u64 = token;

    int ret = ::epoll_ctl(ep, EPOLL_CTL_ADD, fd, &event) < 0;

    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in epoll id: " << id << " register fd: " << fd << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
}

void Selector::event_reregister(int fd, Token token, const Interest &interest) const {
    epoll_event event{};
    event.events = interests_to_epoll(interest);
    event.data.u64 = token;

    int ret = ::epoll_ctl(ep, EPOLL_CTL_MOD, fd, &event) < 0;

    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in epoll id: " << id << " reregister fd: " << fd << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
}

void Selector::event_deregister(int fd) const {
    int ret = ::epoll_ctl(ep, EPOLL_CTL_DEL, fd, nullptr);

    if (ret < 0) {
        std::ostringstream error_stream;
        error_stream << "Error in epoll id: " << id << " deregister fd: " << fd << "Returned errno " << errno;
        std::string error = error_stream.str();
        throw std::system_error(errno, std::system_category(), error);
    }
}

bool Selector::register_waker() {
    return has_waker.exchange(true, std::memory_order_acq_rel);
}
