#include "aim.h"
#include "main.h"

Aim::Aim(float x, float y, float z) {
    this->rotation = 0;
    this->position = glm::vec3(x, y, z);
    float l = 0.5, b = 0.05;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    const GLfloat vertex_buffer_data[] = {
        0,  l/2, -b/2,
        0,  l/2,  b/2,
        0, -l/2, -b/2,
        0, -l/2,  b/2,
        0,  l/2,  b/2,
        0, -l/2, -b/2,

        0,  b/2, -l/2,
        0,  b/2,  l/2,
        0, -b/2, -l/2,
        0, -b/2,  l/2,
        0,  b/2,  l/2,
        0, -b/2, -l/2,

    };
    // this->vertex_buffer_data = vertex_buffer_data;
    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, COLOR_BLACK, GL_FILL);
}

void Aim::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3 (0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Aim::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Aim::tick() {
      // this->set_position(this->position.x, this->position.y, this->position.z);
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}
