//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_TOKEN_H
#define SIMIO_TOKEN_H

#include <cstdint>

namespace simio {

class Token {
  public:
    uint64_t inner;

    Token(uint64_t val) : inner(val) {};

    ~Token() = default;

    uint64_t get() {
        return inner;
    }

};

}

#endif //SIMIO_TOKEN_H
