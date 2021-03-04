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
    explicit IOSource(T io) : state(), inner(io), selector_id(0) {}
    IOSource(IOSource<T> &&rhs) noexcept: state(rhs.state), inner(rhs.inner), selector_id(std::move(rhs.selector_id)) {}
    IOSource &operator=(IOSource<T> &&rhs) noexcept {
        if (this != &rhs) {
            state = rhs.state;
            inner = rhs.inner;
            selector_id = std::move(rhs.selector_id);
        }
        return *this;
    }

    T get_inner() { return inner; }

    using IOCallback = std::function<int(T)>;
    int do_io(const IOCallback &f) {
        state.do_io<T>(f, inner);
    }

    void event_register(Registry *registry, Token token, Interest interest) override {
        selector_id.associate(*registry);
        registry->get_selector().event_register(inner.as_raw_fd(), token, interest);
    }
    void event_reregister(Registry *registry, Token token, Interest interest) override {
        selector_id.check_association(*registry);
        registry->get_selector().event_reregister(inner.as_raw_fd(), token, interest);
    }
    void event_deregister(Registry *registry) override {
        selector_id.remove_association(*registry);
        registry->get_selector().event_deregister(inner.as_raw_fd());
    }

  private:
    sys::IOSourceState state{};
    T inner;
    SelectorID selector_id{0};
};

}

#endif //SIMIO__IO_SOURCE_H_
