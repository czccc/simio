//
// Created by Cheng on 2021/3/21.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_SERVER_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_SERVER_H_

#include <utility>
#include <thread>

#include "simio.h"

namespace simio {

class TcpServer {
  public:
    explicit TcpServer(const SocketAddr &addr, int num_threads = 4) : acceptor_(addr) {
        auto acc_pair = unbounded_spsc_channel<Task>();
        acceptor_.set_sender(std::move(acc_pair.first));
        receiver_ = std::move(acc_pair.second);
        for (int i = 0; i < num_threads; ++i) {
            Worker worker{};
            auto work_pair = unbounded_spsc_channel<Task>();
            worker.set_receiver(std::move(work_pair.second));
            workers_.push_back(std::move(worker));
            senders_.push_back(std::move(work_pair.first));
        }
    }

    void set_accept_callback(std::function<Task(std::pair<simio::TcpStream, simio::SocketAddr>)> cb) {
        acceptor_.set_accept_callback(std::move(cb));
    }
    void set_stream_callback(std::function<void(Task * )> cb) {
        callback_ = std::move(cb);
    }

    [[noreturn]] void start() {
        std::thread acceptor_handle([&] { acceptor_.start(); });
        std::vector<std::thread> worker_handles{};
        for (auto &worker: workers_) {
            std::thread worker_handle([&] { worker.start(); });
            worker_handles.push_back(std::move(worker_handle));
        }
        while (true) {
            Task task = receiver_->recv_blocking();
            handle_task(task);
        }
    }

  private:
    void handle_task(Task task) {
        int i = rand() % workers_.size();
        task.callback_ = callback_;
        senders_[i]->send(task);
    }

    Acceptor acceptor_;
    std::unique_ptr<SpscReceiver < Task>> receiver_{};
    std::vector<Worker> workers_{};
    std::vector<std::unique_ptr<SpscSender < Task>>> senders_{};
    std::function<void(Task * )> callback_{};

};

}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_SERVER_H_
