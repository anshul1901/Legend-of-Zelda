#include "barrel.h"
#include "main.h"

Barrel::Barrel(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->base = Ball (x, y, z, 0.5, 0.5, 0.5, COLOR_WHITE);
    this->coin = Ball(x, y-0.4, z, 0.4, 0.4, 0.4, COLOR_RED);
}

void Barrel::draw(glm::mat4 VP, glm::vec3 axis) {
  this->base.draw(VP, axis);
  this->coin.draw(VP, axis);
}

void Barrel::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}


void Barrel::tick() {
    this->base.set_position(this->position.x, this->position.y , this->position.z);
    this->coin.set_position(this->position.x, this->position.y - 1 , this->position.z);
}

bounding_box_t Barrel::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  bounding_box_t bbox = {x, y, z, 0.6, 0.6, 0.6};
  return bbox;
}
