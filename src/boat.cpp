#include "boat.h"
#include "main.h"
#include "ball.h"
#include "sail.h"

Boat::Boat(float x, float y, float z) {

    this->base = Ball(x, y, z, 0.7, 0.3, 0.5, COLOR_RED);
    this->sail1 = Sail(x, y+2, z, 3, 2, 0.125, COLOR_WHITE);
    this->sail2 = Sail(x, y+2, z, 3, -2, 0.125, COLOR_WHITE);

    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    speed = 1;
    this->health = 100;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    //  const GLfloat vertex_buffer_data[] = {
    //
    // };
    GLfloat vertex_buffer_data[] = {};

    this->object = create3DObject(GL_TRIANGLES, 0, vertex_buffer_data, COLOR_RED, GL_FILL);
}

void Boat::draw(glm::mat4 VP, int is_wind) {
    this->base.draw(VP);
    if (is_wind == 0) {
      this->sail1.draw(VP);
      this->sail2.draw(VP);
    }
    else if (is_wind == 1) {
      this->sail1.draw(VP);
    }
    else if (is_wind == 2) {
      this->sail2.draw(VP);
    }
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boat::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
    this->base.set_position(x, y, z);
    this->sail1.set_position(x, y+2, z);
    this->sail2.set_position(x, y+2, z);
}

void Boat::tick() {

    this->base.tick();
    this->base.position = this->position;
    this->base.rotation = this->rotation;

    this->sail1.tick();
    this->sail1.position.x = this->position.x;
    this->sail1.position.y = this->position.y+2;
    this->sail1.position.z = this->position.z;
    // this->sail1.rotation = this->rotation;
    this->sail1.rotation += 1;
    this->sail2.tick();
    this->sail2.position.x = this->position.x;
    this->sail2.position.y = this->position.y+2;
    this->sail2.position.z = this->position.z;
    // this->sail2.rotation = this->rotation;
    this->sail2.rotation += 1;
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
