//
// Created by Cheng on 2021/3/8.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_

#include <map>

#include "simio/reactor/event_loop.h"
#include "simio/reactor/task.h"

namespace simio {

class Worker {
  public:
    // Worker() : loop_();

    // void add_task(Task task);

    [[noreturn]] void start() {
        while (true) {
            poller_->poll(events_, 1000);
            for (auto &&ev : events_.as_vec()) {
                Event event = Event::from_sys_event(ev);
                auto item = token_map_.find(event.token());
                switch (Event::from_sys_event(ev).token()) {
                    case 0:
                        try {
                            // auto new_accept = listener.accept();
                            // IOStreamTask<TcpStream> task = build_io_task(new_accept);
                        } catch (std::system_error &e) {
                            std::cout << "server" << e.what() << std::endl;
                        }
                        break;
                    default:break;
                }
            }
        }
    }
  private:
    EventList events_{1024};
    std::unique_ptr<Poll> poller_{};

    std::map<Token, IOStreamTask < TcpStream>> token_map_{};
};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_WORKER_H_
