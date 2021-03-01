//
// Created by Cheng on 2021/2/28.
//

#include "epoll.h"

#include <atomic>
#include <unistd.h>
#include <iostream>

namespace simio {

namespace sys {

unsigned int interests_to_epoll(Interest interests) {
    unsigned int kind = EPOLLET;
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
      id(next_id.fetch_add(1, std::memory_order_relaxed)) {
}

Selector::~Selector() {
    close(ep);
}

int Selector::select(EventList events, int timeout) const {
    int num_events = epoll_wait(ep,
                                &*events.begin(),
                                static_cast<int>(events.size()),
                                timeout);
    events.erase(events.begin() + num_events, events.end());
    return 0;
}

void Selector::event_register(int fd, Token token, const Interest &interest) const {
    epoll_event event{};
    event.events = interests_to_epoll(interest);
    event.data.u64 = token.get();

    if (epoll_ctl(ep, EPOLL_CTL_ADD, fd, &event) < 0) {
        std::cout << "Failed to insert handler to epoll" << std::endl;
    }
}

void Selector::event_reregister(int fd, Token token, const Interest &interest) const {
    epoll_event event{};
    event.events = interests_to_epoll(interest);
    event.data.u64 = token.get();

    if (epoll_ctl(ep, EPOLL_CTL_MOD, fd, &event) < 0) {
        std::cout << "Failed to insert handler to epoll" << std::endl;
    }
}

void Selector::event_deregister(int fd) const {
    epoll_ctl(ep, EPOLL_CTL_DEL, fd, nullptr);
}

bool Selector::register_waker() {
    has_waker.exchange(true, std::memory_order_acq_rel);
}

}

}