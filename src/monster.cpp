#include "ball.h"
#include "monster.h"
#include "main.h"
#include "iostream"

Monster::Monster(float x, float y, float z, float scale, int type) {
    float size_of_fireball = 0;
    float size_of_gift = 0.3 ;
    color_t color;
    // this->position = glm::vec3(x, y, z);
    this->set_position(x, y, z);
    this->rotation = 0;
    this->size = scale*3;
    this->number_of_fireballs = 0;
    this->number_of_gifts = 0;
    if (type == 0) {
      number_of_fireballs = 2, size_of_fireball = 0.2;
      number_of_gifts = 8;
      color = COLOR_RED;
      this->health = 100;

    } else if (type == 1) {
      number_of_fireballs = 3, size_of_fireball = 0.3;
      number_of_gifts = 8;
      color = COLOR_GREEN;
      this->health = 200;
    } else {
      number_of_fireballs = 4, size_of_fireball = 0.4;
      number_of_gifts = 0;
      color = COLOR_BLACK;
      this->health = 300;
    }

    this->center = Ball(x, y, z, size, size, size, COLOR_BLACK);
    this->right = Ball(x, y, z - 1.5*size, size/2, size/2, size/2, color);
    this->left = Ball(x, y, z + 1.5*size, size/2, size/2, size/2, color);
    this->top = Ball(x, y + 1.5*size, z, size/2, size/2, size/2, color);




    for (int i = 0; i < number_of_fireballs; i++) {
      this->fireballs[i] = Ball(x, y, z, size_of_fireball, size_of_fireball, size_of_fireball, COLOR_YELLOW);
    }
    for (int i = 0; i < number_of_gifts; i++) {
      this->gifts[i] = Ball(x, y, z, size_of_gift, size_of_gift, size_of_gift, COLOR_PURPLE);
    }
}

void Monster::draw(glm::mat4 VP, glm::vec3 axis) {
    this->center.draw(VP, axis);
    this->right.draw(VP, axis);
    this->left.draw(VP, axis);
    this->top.draw(VP, axis);
    for (int i = 0; i < number_of_fireballs; i++) {
      this->fireballs[i].draw(VP, glm::vec3 (1, 1, 1));
    }
    for (int i = 0; i < number_of_gifts; i++) {
      this->gifts[i].draw(VP, glm::vec3 (1, 1, 1));
    }
}

void Monster::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Monster::tick() {
    this->position.x += this->speed.x;
    this->position.y += this->speed.y;
    this->position.z += this->speed.z;
    // this->set_position(this->position.x, this->position.y, this->position.z);
    this->center.set_position(this->position.x, this->position.y, this->position.z);
    this->left.set_position(this->position.x, this->position.y, this->position.z+1.5*size);
    this->right.set_position(this->position.x, this->position.y, this->position.z - 1.5*size);
    this->top.set_position(this->position.x, this->position.y+1.5*size, this->position.z);

    this->center.rotation = this->rotation;
    this->left.rotation = this->rotation;
    this->right.rotation = this->rotation;
    this->top.rotation = this->rotation;

    for (int i = 0; i < number_of_fireballs; i++) {
      this->fireballs[i].tick();
    }
    for (int i = 0; i < number_of_gifts; i++) {
      this->gifts[i].tick();
      if (this->gifts[i].speed.y != 0 && this->gifts[i].position.y > -3.1) {
        this->gifts[i].speed.y -= 0.07;
        // printf("%f %f %f\n", this->gifts[i].position.x, this->gifts[i].position.y, this->gifts[i].position.z);

      } else if (this->gifts[i].position.y < -1.7){
        this->gifts[i].speed = glm::vec3 (0, 0, 0);
        this->gifts[i].position.y = -1.7;
        this->gifts[i].tick();
        // this->gifts[i].position.x = 0;
        // this->gifts[i].position.z = 0;
        // printf("%f %f %f\n", this->gifts[i].position.x, this->gifts[i].position.y, this->gifts[i].position.z);
      }
    }
    if (this->health == 0) {
      this->speed.y = -0.2;
    }
}

bounding_box_t Monster::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  float a = this->size;
  bounding_box_t bbox = {x, y, z, 4*a, 4*a, 4*a};
  return bbox;
}
