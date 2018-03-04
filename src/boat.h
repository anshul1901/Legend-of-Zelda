#include "main.h"
#include "ball.h"
#include "sail.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float y, float z);
    Ball base;
    Sail sail1, sail2;
    int health;
    // Ball chhotu;
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, int is_wind);
    void set_position(float x, float y, float z);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // BOAT_H
