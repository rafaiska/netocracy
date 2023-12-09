#include "game_object.hpp"

void GameObject::update(float delta) {
  for (auto component : components) {
    component.update(delta);
  }
  for (auto child : children) {
    child.update(delta);
  }
};