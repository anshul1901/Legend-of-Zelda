#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float z, float length, float width, float height, color_t color);
    glm::vec3 position;
    float rotation;
    float rotation_z;
    void draw(glm::mat4 VP, glm::vec3 axis);
    void set_position(float x, float y, float z);
    void tick();
    void shoot(float theta, float power);
    glm::vec3 speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
