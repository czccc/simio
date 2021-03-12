//
// Created by Cheng on 2021/3/7.
//

#ifndef SIMIO_INCLUDE_SIMIO_BASE_TOKEN_H_
#define SIMIO_INCLUDE_SIMIO_BASE_TOKEN_H_

#include <atomic>

namespace simio {

using Token = uint64_t;

static std::atomic<Token> global_next_token{1};

}
#endif //SIMIO_INCLUDE_SIMIO_BASE_TOKEN_H_
