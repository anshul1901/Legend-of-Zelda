#include "sail.h"
#include "main.h"

Sail::Sail(float x, float y, float z, float h, float b, float w, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
     const GLfloat vertex_buffer_data[] = {
       0,  h/2,  0,
       0, -h/2, -b/2,
       0, -h/2,  0,

       w,  h/2,  0,
       w, -h/2, -b/2,
       w, -h/2,  0,

       0, h/2, 0,
       0, -h/2, -b/2,
       w, h/2, 0,

       w, -h/2, -b/2,
       0, -h/2, -b/2,
       w, h/2, 0,

       0, h/2, 0,
       0, -h/2, 0,
       w, h/2, 0,

       w, -h/2, 0,
       0, -h/2, 0,
       w, h/2, 0,

    };
    // this->vertex_buffer_data = vertex_buffer_data;
    this->object = create3DObject(GL_TRIANGLES, 18 , vertex_buffer_data, color, GL_FILL);
}

void Sail::draw(glm::mat4 VP) {
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

void Sail::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Sail::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
