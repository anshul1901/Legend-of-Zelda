#include "ball.h"
#include "main.h"

float l, w, h;
Ball::Ball(float x, float y, float z,  float length, float width, float height, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->rotation_z = 0;
    // speed = 1;
    l = length, w = width, h = height;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
     const GLfloat vertex_buffer_data[] = {
        -length,-width,-height, // triangle 1 : begin
        -length,-width, height,
        -length, width, height, // triangle 1 : end
         length, width,-height, // triangle 2 : begin
        -length,-width,-height,
        -length, width,-height, // triangle 2 : end
         length,-width, height,
        -length,-width,-height,
         length,-width,-height,
         length, width,-height,
         length,-width,-height,
        -length,-width,-height,
        -length,-width,-height,
        -length, width, height,
        -length, width,-height,
         length,-width, height,
        -length,-width, height,
        -length,-width,-height,
        -length, width, height,
        -length,-width, height,
         length,-width, height,
         length, width, height,
         length,-width,-height,
         length, width,-height,
         length,-width,-height,
         length, width, height,
         length,-width, height,
         length, width, height,
         length, width,-height,
        -length, width,-height,
         length, width, height,
        -length, width,-height,
        -length, width, height,
         length, width, height,
        -length, width, height,
         length,-width, height
    };
    // this->vertex_buffer_data = vertex_buffer_data;
    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP, glm::vec3 axis) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), axis);
    glm::mat4 rotate_z   = glm::rotate((float) (this->rotation_z * M_PI / 180.0f), glm::vec3 (0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate * rotate_z);
    // Matrices.model *= (translate * rotate_z * (-translate));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Ball::shoot(float theta, float power) {
    this->speed.x = -power*cos(theta * M_PI / 180.0f);
    this->speed.y =  power;
    this->speed.z =  power*sin(theta * M_PI / 180.0f);
}

void Ball::tick() {
    // this->rotation += speed;
    // if (this->position.y > -1.68)
      this->position.y += this->speed.y;
      this->position.x += this->speed.x;
      this->position.z += this->speed.z;
      this->set_position(this->position.x, this->position.y, this->position.z);
}

bounding_box_t Ball::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  bounding_box_t bbox = {x, y, z, 4*l, 4*w, 4*h};
  return bbox;
}
