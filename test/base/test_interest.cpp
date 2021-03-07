//
// Created by Cheng on 2021/3/7.
//

#include "simio.h"
#include "gtest/gtest.h"

using simio::Interest;

TEST(TestInterest, is_tests) {
    ASSERT_TRUE(Interest::READABLE().is_readable());
    ASSERT_TRUE(Interest::WRITABLE().is_writable());

    ASSERT_FALSE(Interest::READABLE().is_writable());
    ASSERT_FALSE(Interest::WRITABLE().is_readable());
}

TEST(TestInterest, add_remove) {
    Interest interest = Interest::READABLE() | Interest::WRITABLE();
    ASSERT_TRUE(interest.is_readable());
    ASSERT_TRUE(interest.is_writable());

    interest = interest.remove(Interest::READABLE());
    ASSERT_FALSE(interest.is_readable());
    ASSERT_TRUE(interest.is_writable());

    interest = interest.remove(Interest::WRITABLE());
    ASSERT_FALSE(interest.is_readable());
    ASSERT_FALSE(interest.is_writable());

    interest = interest.add(Interest::WRITABLE());
    ASSERT_FALSE(interest.is_readable());
    ASSERT_TRUE(interest.is_writable());

}
