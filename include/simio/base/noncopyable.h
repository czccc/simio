//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_NONCOPYABLE_H_
#define SIMIO_INCLUDE_SIMIO_BASE_NONCOPYABLE_H_

namespace simio {

class noncopyable {
  public:
    noncopyable(const noncopyable &) = delete;
    void operator=(const noncopyable &) = delete;

  protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_NONCOPYABLE_H_
