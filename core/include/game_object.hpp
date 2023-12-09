#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <stdint.h>

#include <set>
#include <vector>

#include "game_component.hpp"

class GameObject {
  uint32_t object_id;
  std::vector<GameComponent> components;
  std::set<GameObject> children;

 public:
  void update(float delta);
};

#endif