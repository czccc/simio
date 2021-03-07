//
// Created by Cheng on 2021/3/7.
//


#include "simio.h"
#include "gtest/gtest.h"

using namespace simio;


TEST(TestEvent, events_all) {
    Poll poll{};
    EventList events(10);
    ASSERT_TRUE(events.as_vec().empty());
    ASSERT_EQ(10, events.as_vec().capacity());

    Token waker_token = 10;
    Waker waker(poll.get_registry()->get_selector(), waker_token);
    waker.wake();

    poll.poll(events, 100);

    ASSERT_FALSE(events.is_empty());

    for (auto &&sys_ev: events.as_vec()) {
        Event event(sys_ev);
        ASSERT_EQ(event.token(), waker_token);
        ASSERT_TRUE(event.is_readable());
    }

    events.clear();
    ASSERT_TRUE(events.as_vec().empty());
}
