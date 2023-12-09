#ifndef _GAME_COMPONENT_H_
#define _GAME_COMPONENT_H_

#include <cstdint>

class GameComponent {
    public:
        virtual uint32_t type_id() = 0;
        virtual void update(float delta) = 0;
};

#endif