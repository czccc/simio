//
// Created by Cheng on 2021/2/28.
//

#ifndef SIMIO_EVENT_SOURCE_H_
#define SIMIO_EVENT_SOURCE_H_

#include "../poll.h"

namespace simio {

class EventSource {
  public:
    virtual void event_register(Registry &registry, Token token, Interest interest) = 0;
    virtual void event_reregister(Registry &registry, Token token, Interest interest) = 0;
    virtual void event_deregister(Registry &registry) = 0;
};

}

#endif //SIMIO_EVENT_SOURCE_H_
