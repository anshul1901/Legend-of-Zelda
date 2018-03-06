#include "boat.h"
#include "main.h"
#include "ball.h"
#include "sail.h"

Boat::Boat(float x, float y, float z) {

    this->base = Ball(x, y, z, 0.7, 0.3, 0.5, COLOR_RED);
    this->sail1 = Sail(x, y+2, z, 3, 2, 0.125, COLOR_WHITE);
    this->sail2 = Sail(x, y+2, z, 3, -2, 0.125, COLOR_WHITE);
    this->cannon = Ball(x-0.5, y+2, z, 1.5, 0.05, 0.05, COLOR_BLACK);
    this->cannon.rotation_z = -45;
    this->fireball = Ball(0, 0, 0, 0.2, 0.2, 0.2, COLOR_BROWN);
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    speed = 1;
    this->health = 100;
}

void Boat::draw(glm::mat4 VP) {
    this->base.draw(VP, glm::vec3 (0, 1, 0));
    this->sail1.draw(VP);
    this->sail2.draw(VP);
    this->cannon.draw(VP, glm::vec3 (0, 1, 0));
    this->fireball.draw(VP, glm::vec3(1, 1, 1));
}

void Boat::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->base.set_position(x, y, z);
    this->sail1.set_position(x, y+2, z);
    this->sail2.set_position(x, y+2, z);
    this->cannon.set_position(x, y+2, z);
}

void Boat::tick() {

    this->base.tick();
    this->base.position = this->position;
    this->base.rotation = this->rotation;

    this->sail1.tick();
    this->sail1.position.x = this->position.x;
    this->sail1.position.y = this->position.y+2;
    this->sail1.position.z = this->position.z;
    // this->sail1.rotation += 1;

    this->sail2.tick();
    this->sail2.position.x = this->position.x;
    this->sail2.position.y = this->position.y+2;
    this->sail2.position.z = this->position.z;

    this->cannon.tick();
    this->cannon.position.x = this->position.x;
    this->cannon.position.y = this->position.y+1;
    this->cannon.position.z = this->position.z;
    this->cannon.rotation = this->rotation;

    this->fireball.tick();
    // this->fireball.position = this->position;
    // this->sail2.rotation += 1;
}

bounding_box_t Boat::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  float l = 0.7, w = 0.3, h = 5;
  bounding_box_t bbox =  {x, y, z, 4*l, 4*w, 4*h};
  return bbox;
}
