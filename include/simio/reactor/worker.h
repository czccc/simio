//
// Created by Cheng on 2021/3/8.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_

#include <map>

#include "simio/reactor/task.h"

namespace simio {

class Worker {
  public:
    Worker() : poller_(new Poll()) {};

    void set_receiver(std::unique_ptr<SpscReceiver < Task>>
    receiver) {
        receiver_ = std::move(receiver);
    }

    void check_new_task() {
        while (receiver_->size() > 0) {
            Task new_task = receiver_->recv();
            poller_->get_registry()->event_register(&new_task.inner_, new_task.token_, new_task.interest_);
            streams_.insert({new_task.token_, new_task});
        }
    }

    [[noreturn]] void start() {
        while (true) {
            check_new_task();
            poller_->poll(events, 1000);
            for (auto &&ev : events.as_vec()) {
                Token token = Event::from_sys_event(ev).token();
                auto task_it = streams_.find(token);
                if (task_it == streams_.end()) {
                    continue;
                }
                Task task = task_it->second;
                task.process();
                if (task.need_reregister_) {
                    poller_->get_registry()->event_reregister(&task.inner_, task.token_, task.interest_);
                    task.need_reregister_ = false;
                }
                if (task.need_deregister_) {
                    poller_->get_registry()->event_deregister(&task.inner_);
                    streams_.erase(task_it);
                }
            }
        }
    }

  private:
    EventList events{1024};
    std::unique_ptr<Poll> poller_{nullptr};
    std::map<Token, Task> streams_{};
    std::unique_ptr<SpscReceiver < Task>> receiver_{ nullptr };

};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_
