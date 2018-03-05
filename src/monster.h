#include "main.h"
#include "ball.h"

#ifndef MONSTER_H
#define MONSTER_H


class Monster {
public:
    Monster() {}
    Monster(float x, float y, float z, float scale, int type, color_t color);
    Ball center, right, left, top;
    int number_of_fireballs;
    Ball fireballs[10], gifts[10];
    float health;
    glm::vec3 position;
    float rotation;
    float size;
    void draw(glm::mat4 VP, glm::vec3 axis);
    void set_position(float x, float y, float z);
    void tick();
    glm::vec3 speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // MONSTER_H
