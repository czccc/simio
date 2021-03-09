//
// Created by Cheng on 2021/3/9.
//

#ifndef SIMIO_INCLUDE_SIMIO_REACTOR_ACCEPTOR_H_
#define SIMIO_INCLUDE_SIMIO_REACTOR_ACCEPTOR_H_

#include "simio/net/tcp_listener.h"
#include "simio/net/tcp_stream.h"

#include "simio/reactor/task.h"

namespace simio {
class Acceptor : public Task {
  public:
    void on_readable() override;
    void on_writable() override;

  private:
    TcpListener listener_;
};
}

#endif //SIMIO_INCLUDE_SIMIO_REACTOR_ACCEPTOR_H_
