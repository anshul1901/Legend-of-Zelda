#include "ball.h"
#include "monster.h"
#include "main.h"

Monster::Monster(float x, float y, float z, float scale, int type, color_t color) {
    float size_of_fireball = 0;

    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->health = 100;
    this->size = scale*3;

    this->center = Ball(x, y, z, size, size, size, color);
    this->right = Ball(x, y, z - 1.5*size, size/2, size/2, size/2, color);
    this->left = Ball(x, y, z + 1.5*size, size/2, size/2, size/2, color);
    this->top = Ball(x, y + 1.5*size, z, size/2, size/2, size/2, color);

    this->number_of_fireballs = 0;

    if (type == 0) {
      number_of_fireballs = 2, size_of_fireball = 0.2;
    } else if (type == 1) {
      number_of_fireballs = 3, size_of_fireball = 0.3;
    } else {
      number_of_fireballs = 4, size_of_fireball = 0.4;
    }
    for (int i = 0; i < number_of_fireballs; i++) {
      this->fireballs[i] = Ball(x, y, z, size_of_fireball, size_of_fireball, size_of_fireball, COLOR_BLACK);
    }

    // speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
     const GLfloat vertex_buffer_data[] = {
    };
    // this->vertex_buffer_data = vertex_buffer_data;
    this->object = create3DObject(GL_TRIANGLES, 0, vertex_buffer_data, color, GL_FILL);
}

void Monster::draw(glm::mat4 VP, glm::vec3 axis) {
    this->center.draw(VP, axis);
    this->right.draw(VP, axis);
    this->left.draw(VP, axis);
    this->top.draw(VP, axis);

    for (int i = 0; i < number_of_fireballs; i++) {
      this->fireballs[i].draw(VP, glm::vec3 (1, 1, 1));
    }

    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), axis);
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    // Matrices.model *= (translate * rotate_z * (-translate));
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Monster::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Monster::tick() {
    // this->rotation += speed;
    // if (this->position.y > -1.68)
      // this->position.y += this->speed.y;
      // this->position.x += this->speed.x;
      // this->position.z += this->speed.z;
    for (int i = 0; i < number_of_fireballs; i++) {
      this->fireballs[i].tick();
      this->fireballs[i].rotation+=1;
    }
      if (this->health == 0) {
        this->position.y = -5;
      }
}

bounding_box_t Monster::bounding_box() {
  float x = this->position.x, y = this->position.y, z = this->position.z;
  float a = this->size;
  bounding_box_t bbox = {x, y, z, 2*a, 2*a, 2*a};
  return bbox;
}
