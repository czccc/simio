//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_WAKER_H_
#define SIMIO_INCLUDE_SIMIO_BASE_WAKER_H_

#include "simio/sys.h"
#include "simio/event.h"

namespace simio {

class Waker {
  public:
    Waker(const sys::Selector &s, Token token) : inner_(sys::Waker(s, token)) {}
    ~Waker() = default;
    bool wake() { return inner_.wake(); }

  private:
    sys::Waker inner_;
};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_WAKER_H_
