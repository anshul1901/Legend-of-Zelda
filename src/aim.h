#include "main.h"

#ifndef AIM
#define AIM

class Aim {
public:
    Aim() {}
    Aim(float x, float y, float z);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
private:
    VAO *object;
};

#endif // AIM
