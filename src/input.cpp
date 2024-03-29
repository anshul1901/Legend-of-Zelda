#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"

bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;
int view = 0;
extern int old_view;

using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_C:
          view = (view + 1) % 5;
          break;
        case GLFW_KEY_F:
          view = old_view;
          break;
        // case GLFW_KEY_X:
        // // do something ..
        // break;
        default:
          break;
        }
    } else if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        case GLFW_KEY_F:
            view = 1;
            break;
        default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar(GLFWwindow *window, unsigned int key) {
    switch (key) {
    case 'Q':
    case 'q':
        quit(window);
        break;
    default:
        break;
    }
}
extern int mouse_left;
extern glm::vec3 target, eye;
double cursor_x1, cursor_y1;
double cursor_x1_press, cursor_y1_press;

/* Executed when a mouse button is pressed/released */
void mouseButton(GLFWwindow *window, int button, int action, int mods) {
  glfwGetCursorPos(window, &cursor_x1, &cursor_y1);
  // cout<<cursor_x1<<" "<<cursor_y1<<endl;

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS) {

            return;
        } else if (action == GLFW_RELEASE) {
            // cout<<"release\n";
            cursor_x1_press = cursor_x1;
            cursor_y1_press = cursor_y1;
            // cout<<cursor_x1<<" release "<<cursor_y1<<endl;
        }
        break;
    // case GLFW_MOUSE_BUTTON_RIGHT:
    // if (action == GLFW_RELEASE) {
    // rectangle_rot_dir *= -1;
    // }
    // break;
    default:
        break;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // Do something
    if (view == 4) {
      // float x = target.x - eye.x;
      // float y = target.y - eye.y;
      // float z = target.z - eye.z;
      // float x_z = atan2(x, z) * 180.0f / M_PI;
      // float y_z = atan2(y, z) * 180.0f / M_PI;
      eye.y -= (yoffset)*1;
      // eye.x -= (yoffset)*cos(x_z);
      // eye.z -= (yoffset)*sin(x_z);
      // cout<<cos(x_z)<<" "<<sin(x_z)<<"\n";
      reset_screen();
  }
}
