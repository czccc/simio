//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_INTEREST_H
#define SIMIO_INTEREST_H

namespace simio {

class Interest {
  public:
    unsigned char inner;

    explicit Interest(unsigned char val) : inner(val) {}
    ~Interest() = default;

    bool is_readable() const { return inner & READABLE_RAW; }
    bool is_writable() const { return inner & WRITABLE_RAW; }

    Interest add(const Interest &other) const { return Interest(inner | other.inner); }
    Interest remove(const Interest &other) const {
        // TODO: check inner is zero
        return Interest(inner & !other.inner);
    }

    static Interest READABLE() {
        return Interest(READABLE_RAW);
    }
    static Interest WRITABLE() {
        return Interest(WRITABLE_RAW);
    }

  private:

    // These must be unique.
    const static unsigned char READABLE_RAW = 0b0001;
    const static unsigned char WRITABLE_RAW = 0b0010;

};

}

#endif //SIMIO_INTEREST_H
