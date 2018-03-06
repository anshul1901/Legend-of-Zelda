#include "main.h"
#include "ball.h"
#include "sail.h"
// #include "cannon.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z);
    Ball base;
    Sail sail1, sail2;
    Ball cannon;
    Ball fireball;
    int health;
    // Ball chhotu;
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, int is_sail);
    void set_position(float x, float y, float z);
    void tick();
    glm::vec3 speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BOAT_H
