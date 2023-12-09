#ifndef _CN_TIMER_H_
#define _CN_TIMER_H_

#include "game_component.hpp"

class CN_Timer : public GameComponent {
    uint32_t counter = 0;

    public:
        uint32_t type_id() { return 1;}
        void update(float delta);
        void reset();
        uint32_t get_ticks();
};

#endif
