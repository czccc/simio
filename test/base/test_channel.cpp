//
// Created by Cheng on 2021/3/21.
//

#include "simio.h"
#include <thread>
#include <iostream>
#include "gtest/gtest.h"

TEST(TestChannel, create_tests) {
    auto pair = simio::unbounded_spsc_channel<int>();
    auto sender = std::move(pair.first);
    auto receiver = std::move(pair.second);
    std::thread t1([&]() {
        for (int i = 0; i < 10; i++) {
            sender->send(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < 10; i++) {
            while (receiver->size() == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            int val = receiver->recv();
            ASSERT_EQ(i, val);
        }
    });
    t1.join();
    t2.join();
}
TEST(TestChannel, blocking_tests) {
    auto pair = simio::unbounded_spsc_channel<int>();
    auto sender = std::move(pair.first);
    auto receiver = std::move(pair.second);
    std::thread t1([&]() {
        for (int i = 0; i < 10; i++) {
            sender->send(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
    std::thread t2([&]() {
        for (int i = 0; i < 10; i++) {
            int val = receiver->recv_blocking();
            ASSERT_EQ(i, val);
        }
    });
    t1.join();
    t2.join();
}
