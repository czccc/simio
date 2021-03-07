//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_INTEREST_H_
#define SIMIO_INCLUDE_SIMIO_BASE_INTEREST_H_

#include <cinttypes>

namespace simio {

class Interest {
  public:
    uint8_t inner;

    explicit Interest(uint8_t val) : inner(val) {}
    ~Interest() = default;

    bool is_readable() const { return inner & READABLE_RAW; }
    bool is_writable() const { return inner & WRITABLE_RAW; }

    Interest add(const Interest &other) const { return Interest(inner | other.inner); }
    Interest remove(const Interest &other) const {
        // TODO: check inner_ptr_ is zero
        uint8_t t = inner & ~other.inner;
        return Interest(t);
    }

    Interest operator|(const Interest &rhs) const { return add(rhs); }

    static Interest READABLE() { return Interest(READABLE_RAW); }
    static Interest WRITABLE() { return Interest(WRITABLE_RAW); }

  private:

    const static unsigned char READABLE_RAW = 0b0001;
    const static unsigned char WRITABLE_RAW = 0b0010;

};

}

#endif //SIMIO_INCLUDE_SIMIO_BASE_INTEREST_H_
