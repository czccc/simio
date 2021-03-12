//
// Created by Cheng on 2021/3/9.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_BUFFER_H_
#define SIMIO_INCLUDE_SIMIO_BASE_BUFFER_H_

#include <vector>

namespace simio {
template<typename T>
class Buffer {
  public:
    explicit Buffer(T inner, int size = 1024) : inner_(inner), buffer_(1024), pos_(0), cap_(0) {}
    ~Buffer() = default;

    int read(std::vector<char> &buf) {
        if (pos_ == cap_ && buf.capacity() >= buffer_.capacity()) {
            pos_ = 0;
            cap_ = 0;
            return inner_.read(buf);
        }
    }

    int fill_buf() {
        if (pos_ == cap_) {
            cap_ = inner_.read(buffer_);
            pos_ = 0;
        }
    }
  private:
    std::vector<char> fill_buf() {
    }

    T inner_;
    std::vector<char> buffer_;
    int pos_;
    int cap_;
};
}

#endif //SIMIO_INCLUDE_SIMIO_BASE_BUFFER_H_
