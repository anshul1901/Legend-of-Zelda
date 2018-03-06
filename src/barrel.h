#include "main.h"
#include "ball.h"

#ifndef BARREL_H
#define BARREL_H


class Barrel {
public:
    Barrel() {}
    Barrel(float x, float y, float z);
    glm::vec3 position;
    Ball base;
    Ball coin;
    float rotation;
    void draw(glm::mat4 VP, glm::vec3 axis);
    void set_position(float x, float y, float z);
    void tick();
    void shoot(float theta, float power);
    glm::vec3 speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BARREL_H
