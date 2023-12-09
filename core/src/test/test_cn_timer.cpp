#include <gtest/gtest.h> // googletest header file

#include "cn_timer.hpp"

const char *actualValTrue  = "hello gtest";
const char *actualValFalse = "hello world";
const char *expectVal      = "hello gtest";

TEST(TimerComponent, TimerHappyDay) {
    CN_Timer timer = CN_Timer();
    timer.update(20.0);
    EXPECT_FLOAT_EQ(20.0, timer.get_ticks());
    timer.reset();
    EXPECT_FLOAT_EQ(0.0, timer.get_ticks());
    timer.update(1.0);
    EXPECT_FLOAT_EQ(1.0, timer.get_ticks());
}