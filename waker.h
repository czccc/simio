//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO__WAKER_H_
#define SIMIO__WAKER_H_

#include "sys/unix/waker.h"

namespace simio {

class Waker {
  public:
//    Waker() : inner(sys::Waker()) {}
//    ~Waker();
    bool wake() {
        return inner.wake();
    }
  private:
    sys::Waker inner;
};

}

#endif //SIMIO__WAKER_H_
