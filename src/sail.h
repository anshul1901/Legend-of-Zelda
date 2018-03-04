#include "main.h"

#ifndef SAIL_H
#define SAIL_H

class Sail {
public:
    Sail() {}
    Sail(float x, float y, float z, float base, float height, float width, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    double speed;
private:
    VAO *object;
};

#endif // SAIL_H
