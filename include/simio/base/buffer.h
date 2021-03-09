//
// Created by Cheng on 2021/3/9.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_BUFFER_H_
#define SIMIO_INCLUDE_SIMIO_BASE_BUFFER_H_

#include <vector>

namespace simio {
class Buffer {
  public:
    Buffer(int size = 1024) : buffer_(1024) {}
    ~Buffer() = default;
  private:
    std::vector<char> buffer_;
};
}

#endif //SIMIO_INCLUDE_SIMIO_BASE_BUFFER_H_
