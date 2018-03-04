#include "main.h"
#include "timer.h"
#include "ball.h"
#include "boat.h"
#include "math.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

// Ball boat;

float random_number(float min, float max) {
  return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
glm::vec3 eye (8, 2, 0);
glm::vec3 target (0, 0, 0);
glm::vec3 up (0, 1, 0);
extern int view;
int is_wind = 0;
double cursor_x, cursor_y;
const int number_of_rocks = 100;
time_t start = 0, total_time;

Ball rocks[number_of_rocks];
Ball water;
Boat boat;


Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // eye = camera_position;
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // target = camera_target;
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);
    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    water.draw(VP);
    boat.draw(VP);
    for (int i = 0; i < number_of_rocks; i++) {
      rocks[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int up  = glfwGetKey(window, GLFW_KEY_UP);
    int down  = glfwGetKey(window, GLFW_KEY_DOWN);
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    glfwGetCursorPos(window, &cursor_x, &cursor_y);
    int mouse_left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (left) {
      boat.rotation += 1;
    }
    if (right) {
      boat.rotation -= 1;
    }
    if (up) {
      boat.position.x -= 0.1*cos(boat.rotation * M_PI / 180.0f);
      boat.position.z += 0.1*sin(boat.rotation * M_PI / 180.0f);
    }
    if (down) {
      boat.position.x += 0.1*cos(boat.rotation * M_PI / 180.0f);
      boat.position.z -= 0.1*sin(boat.rotation * M_PI / 180.0f);
    }
    if (mouse_left) {
      // cout<<"mouse_left\n";
    }
    if (space && boat.position.y <= -1.68) {
      boat.position.y += 2;
    }

}
int rock = 0;
unsigned long long int j = 0;
unsigned long long int time_count = 0;
void tick_elements() {
    boat.tick();
    // water.tick();
    if ((time_count++) % 500 == 0) {
      is_wind = (int)random_number(0, 4);
      // cout<<time_count<<" "<<is_wind<<endl;
    }
    if (is_wind == 0) {                                     // No wind
      boat.sail1.rotation = boat.rotation;
      boat.sail2.rotation = boat.rotation;
    }
    if (is_wind == 1 || is_wind == 2) {
      if (boat.sail1.rotation > 0) {
        if (boat.sail1.rotation != 0) {
          boat.sail1.rotation -= 1;
        }
        if (boat.sail2.rotation != 0) {
          boat.sail2.rotation -= 1;
        }
      }
      else {
        if (boat.sail1.rotation != 0) {
            boat.sail1.rotation += 1;
          }
        if (boat.sail2.rotation != 0) {
          boat.sail2.rotation += 1;
        }
      }
      if (is_wind == 1)
        boat.position.x += 0.01;
      else boat.position.x -= 0.01;
    }

    if (is_wind == 3 || is_wind == 4) {
      if (boat.sail1.rotation > 90) {
        if (boat.sail1.rotation != 90) {
          boat.sail1.rotation -= 1;
        }
        if (boat.sail2.rotation != 90) {
          boat.sail2.rotation -= 1;
        }
      }
      else {
        if (boat.sail1.rotation != 90) {
            boat.sail1.rotation += 1;
          }
        if (boat.sail2.rotation != 90) {
          boat.sail2.rotation += 1;
        }
      }
      if (is_wind == 3)
        boat.position.z -= 0.01;
      else boat.position.z += 0.01;
    }

    camera_rotation_angle += 1;
    // view = 1;
    if (view == 0) {
      up = glm::vec3(0, 1, 0);                                              //follow_view
      target.x = boat.position.x;
      target.z = boat.position.z;
      eye.x = boat.position.x + 5*cos(boat.rotation * M_PI / 180.0f);
      eye.y = 2;
      eye.z = boat.position.z - 5*sin(boat.rotation * M_PI / 180.0f);
    }
    if (view == 1) {
      up = glm::vec3(0, 1, 0);                                              //follow_view
      eye = boat.position - glm::vec3(0, -0.5, 0);
      target = boat.position - glm::vec3 (100, 0, 0);
      target.x = boat.position.x - 2*cos(boat.rotation * M_PI / 180.0f);
      target.z = boat.position.z + 2*sin(boat.rotation * M_PI / 180.0f);
      eye.x = boat.position.x - 0.5*cos(boat.rotation * M_PI / 180.0f);
      // eye.y = -0.5;
      eye.z = boat.position.z + 0.5*sin(boat.rotation * M_PI / 180.0f);
    }
    if (view == 2) {                                                //top_view
      eye = boat.position + glm::vec3(0, 20, 0);
      target = boat.position;
      up.x = -cos(boat.rotation * M_PI / 180.0f);
      up.z = sin(boat.rotation * M_PI / 180.0f);
      // eye.y = 2;
    }
    if (view == 3) {
      up = glm::vec3(0, 1, 0);                                              //follow_view
      eye = glm::vec3 (0, 10, 10);
      // eye.y = 2;
      target.x = boat.position.x;
      target.z = boat.position.z;
    }
    if (boat.position.y > -1.68) {
      boat.position.y -= 0.1;
    }
    else boat.position.y += 0.02*sin((++j)/7);
    // cout<<cursor_x<<" "<<cursor_y<<endl;
    // cout<<boat.position.x<<" "<<boat.position.y<<" "<<boat.position.z<<endl;
    for (int i = 0; i < number_of_rocks; i++) {
      if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())) {
        boat.position.x += 2*cos(boat.rotation*M_PI / 180.0f);
        boat.position.z -= 2*sin(boat.rotation*M_PI / 180.0f);
        boat.health -= 1;
      }
    }
    char s[100];
    total_time = time(0) - start;
    sprintf(s, "Health: %d | Time: %d", boat.health, (int)total_time);
    glfwSetWindowTitle(window, s);

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // boat       = Ball(1, -1.7, 0, 0.7, 0.2, 0.5, COLOR_RED);
    boat       = Boat(0, -1.7, 0);
    water       = Ball(0, -3, 0, 1000, 1, 1000, COLOR_BLUE);
    int count_rocks = 0;
    for (int i = 0; i < number_of_rocks; i++) {
        float x = random_number(-500, 500);
        float z = random_number(-500, 500);
        rocks[count_rocks++] = Ball(x, -1.7, z, 0.5, 0.5, 0.5, COLOR_BLACK);
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1920;
    int height = 1080;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    start = time(0);
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.length + b.length)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height)) &&
           (abs(a.z - b.z) * 2 < (a.width + b.width));
}

void reset_screen() {
    float near = 0.01;
    float far = 500;
    float angle = M_PI/2;
    float aspect_ratio = float(16/9);
    // Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    Matrices.projection = glm::perspective(angle, aspect_ratio, near, far);
}
