#include "cn_timer.hpp"

void CN_Timer::update(float delta) { counter += delta; }

void CN_Timer::reset() { counter = 0; }

uint32_t CN_Timer::get_ticks() { return counter; }
