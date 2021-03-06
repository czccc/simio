//
// Created by Cheng on 2021/3/1.
//

#ifndef SIMIO__IO_SOURCE_H_
#define SIMIO__IO_SOURCE_H_

#include <atomic>
#include <stdexcept>

#include "base.h"
#include "unix.h"
#include "poll.h"

namespace simio {

class SelectorID {
  public:
    explicit SelectorID(int id = 0) : id(id) {}
    SelectorID(const SelectorID &rhs) : id(rhs.id.load()) {}
    SelectorID &operator=(const SelectorID &rhs) {
        if (this != &rhs) {
            id.store(rhs.id.load());
        }
        return *this;
    }
    SelectorID(SelectorID &&rhs) noexcept: id(rhs.id.load()) {}
    SelectorID &operator=(SelectorID &&rhs) noexcept {
        if (this != &rhs) {
            id.store(rhs.id.load());
        }
        return *this;
    }

    bool associate(Registry &registry) {
        int reg_id = registry.get_selector().get_id();
        int pre_id = id.exchange(reg_id);

        return pre_id == 0;
    }
    bool check_association(Registry &registry) {
        int reg_id = registry.get_selector().get_id();
        int pre_id = id.load();

        if (pre_id == 0 || pre_id != reg_id) {
            return false;
        } else {
            return true;
        }
    }
    bool remove_association(Registry &registry) {
        int reg_id = registry.get_selector().get_id();
        int pre_id = id.exchange(0);

        return reg_id == pre_id;
    }
  private:
    std::atomic<int> id;
};

template<typename T>
class IOSource : EventSource {
  public:
    explicit IOSource(T &io) : state_(), inner_ptr_(io), selector_id_(0) {}
    IOSource(const IOSource<T> &rhs) noexcept
        : state_(rhs.state_), inner_ptr_(rhs.inner_ptr_), selector_id_(std::move(rhs.selector_id_)) {}
    IOSource &operator=(const IOSource<T> &rhs) noexcept {
        if (this != &rhs) {
            state_ = rhs.state_;
            inner_ptr_ = rhs.inner_ptr_;
            selector_id_ = std::move(rhs.selector_id_);
        }
        return *this;
    }

    // int as_raw_fd() const { return inner_ptr_->as_raw_fd(); }
    T get_inner() const { return inner_ptr_; }

    // using IOCallback = std::function<int()>;
    template<typename IOCallback, typename RET>
    RET do_io(const IOCallback &f) {
        // return state_.do_io<IOCallback, T, RET>(f, inner_ptr_);
        return f(inner_ptr_);
    }

    void event_register(Registry *registry, Token token, Interest interest) override {
        selector_id_.associate(*registry);
        registry->get_selector().event_register(inner_ptr_->as_raw_fd(), token, interest);
    }
    void event_reregister(Registry *registry, Token token, Interest interest) override {
        selector_id_.check_association(*registry);
        registry->get_selector().event_reregister(inner_ptr_->as_raw_fd(), token, interest);
    }
    void event_deregister(Registry *registry) override {
        selector_id_.remove_association(*registry);
        registry->get_selector().event_deregister(inner_ptr_->as_raw_fd());
    }

  private:
    sys::IOSourceState state_{};
    T inner_ptr_;
    SelectorID selector_id_{0};
};

}

#endif //SIMIO__IO_SOURCE_H_
