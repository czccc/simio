//
// Created by Cheng on 2021/3/21.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_CHANNEL_H_
#define SIMIO_INCLUDE_SIMIO_BASE_CHANNEL_H_

#include <mutex>
#include <string>
#include <deque>
#include <utility>
#include <memory>
#include <condition_variable>

#include "simio/base/noncopyable.h"

namespace simio {

template<typename T>
class SpscSender : noncopyable {
  public:
    SpscSender(std::shared_ptr<std::mutex> mutex_,
               std::shared_ptr<std::condition_variable> cv,
               std::shared_ptr<std::deque<T>> queue_)
        : mutex_(std::move(mutex_)), cv_(cv), queue_(queue_) {};

    void send(const T &x) {
        std::unique_lock<std::mutex> lock(*mutex_);
        queue_->push_back(x);
        cv_->notify_all();
    }

    void send(T &&x) {
        std::unique_lock<std::mutex> lock(*mutex_);
        queue_->push_back(std::move(x));
        cv_->notify_all();
    }

  private:
    std::shared_ptr<std::mutex> mutex_{};
    std::shared_ptr<std::condition_variable> cv_{};
    std::shared_ptr<std::deque<T>> queue_{};
};

template<typename T>
class SpscReceiver : noncopyable {
  public:
    SpscReceiver(std::shared_ptr<std::mutex> mutex_,
                 std::shared_ptr<std::condition_variable> cv,
                 std::shared_ptr<std::deque<T>> queue_)
        : mutex_(std::move(mutex_)), cv_(cv), queue_(queue_) {};

    T recv() {
        std::unique_lock<std::mutex> lock(*mutex_);
        T front(std::move(queue_->front()));
        queue_->pop_front();
        return front;
    }

    T recv_blocking() {
        std::unique_lock<std::mutex> lock(*mutex_);
        cv_->wait(lock, [&]() { return queue_->size() > 0; });
        T front(std::move(queue_->front()));
        queue_->pop_front();
        return front;
    }

    size_t size() const {
        std::unique_lock<std::mutex> lock(*mutex_);
        return queue_->size();
    }

  private:
    std::shared_ptr<std::mutex> mutex_{};
    std::shared_ptr<std::condition_variable> cv_{};
    std::shared_ptr<std::deque<T>> queue_{};
};

template<typename T>
std::pair<std::unique_ptr<SpscSender<T>>, std::unique_ptr<SpscReceiver<T>>> unbounded_spsc_channel() {
    auto mutex = std::make_shared<std::mutex>();
    auto cv = std::make_shared<std::condition_variable>();
    auto deque = std::make_shared<std::deque<T>>();
    std::unique_ptr<SpscSender<T>>
        sender(new SpscSender<T>(mutex, cv, deque));
    std::unique_ptr<SpscReceiver<T>>
        receiver(new SpscReceiver<T>(mutex, cv, deque));
    return std::make_pair(std::move(sender), std::move(receiver));
}

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_CHANNEL_H_
